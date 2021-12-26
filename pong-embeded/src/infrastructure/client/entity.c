
#include "infrastructure/client/entity.h"

#include "infrastructure/client/socket.h"

#include "api/api.h"
#include "api/embeded.pb.h"

#include "infrastructure/bus/client.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(client_entity, LOG_LEVEL_WRN);


// this should be somekind of config
#define SERVER_ADDRESS "192.168.1.82"
#define SERVER_PORT 9001

#define KEEP_ALIVE_TIMEOUT 10000
#define PING_PONG_TIMEOUT 5000

static uint8_t io_buf[PongBackend_v1_embeded_EmbededRequest_size];
static uint8_t response_buf[PongBackend_v1_embeded_EmbededResponse_size];

#define EXTRA_BUF_SPACE 30
static uint8_t temp_recv_buf[PongBackend_v1_embeded_EmbededResponse_size + EXTRA_BUF_SPACE];

struct client client_instance;

void clientStartup() {
    k_fifo_init(&client_instance.bus_);
    client_instance.network_up_ = false;
    client_instance.state_ = CLIENT_STATE_WAITING_NET;
    client_instance.socket_handle_ = -1;
}

static void clientDispatchOp(socketSendOp req_type) {
    int status = sendSocketOp(client_instance.socket_handle_, req_type);
    if (status < 0) {
        LOG_ERR("Opcode failed with status %d", status);
    }
}

static void clientCleanupSocket() {
    if (client_instance.socket_handle_ >= 0) {
        clientDispatchOp(SOCKET_SEND_CLOSE);
		close(client_instance.socket_handle_);
        client_instance.socket_handle_ = -1;
	}
    HandleClientIsDown();
    client_instance.state_ = CLIENT_STATE_WAITING_NET;
}

static void clientHandleConnected() {
    client_instance.ping_tracker_.last_timestamp_ = k_uptime_get();
    client_instance.ping_tracker_.state_ = KA_STATE_SUCCESS_TIMEOUT;
    HandleClientIsUp();
    client_instance.state_ = CLIENT_STATE_RUNNING;
}

static void clientRunStateConnecting() {
    int socket = -1;
    struct sockaddr_in address;

    (void)connectSocket(
		AF_INET, SERVER_ADDRESS, SERVER_PORT,
		&socket, (struct sockaddr *)&address,
		sizeof(address)
	);

    if (socket < 0) {
		LOG_ERR("Cannot create HTTP connection.");
        k_sleep(K_SECONDS(5));
        return;
	}

	struct websocket_request req;
	memset(&req, 0, sizeof(req));

    req.host = SERVER_ADDRESS;
	req.url = "/";
	req.tmp_buf = temp_recv_buf;
	req.tmp_buf_len = sizeof(temp_recv_buf);


	client_instance.socket_handle_ = websocket_connect(socket, &req, 5 * MSEC_PER_SEC, "IPv4");
	if (client_instance.socket_handle_ < 0) {
		LOG_WRN("Cannot connect to %s:%d", SERVER_ADDRESS, SERVER_PORT);
		close(socket);
        client_instance.socket_handle_ = -1;
        return;
	}

    clientHandleConnected();
}

static void clientDispatchRequest(PongBackend_v1_embeded_EmbededRequest *request) {
    size_t encoded_length = 0, written_length = 0;
    bool status = EncodeRequest(
        io_buf, PongBackend_v1_embeded_EmbededRequest_size, &encoded_length, request
    );
    if (status == false) {
        LOG_WRN("Failed to encode request");
        return;
    }
    written_length = sendSocketData(client_instance.socket_handle_, io_buf, encoded_length);
    if (written_length != encoded_length) {
        LOG_WRN("Was unable to send some data? %d of %d", encoded_length, written_length);
    }
}

static void clientDispatchRequests() {
    static int status;
    if (client_instance.need_pong_) {
        status = sendSocketOp(client_instance.socket_handle_, SOCKET_SEND_PONG);
        if (status < 0) {
            LOG_WRN("PONG opcode failed with status %d", status);
        } else {
            LOG_DBG("PONG sent");
            client_instance.need_pong_ = false;
        }
    }
    if (client_instance.ping_tracker_.state_ == KA_STATE_PING) {
        status = sendSocketOp(client_instance.socket_handle_, SOCKET_SEND_PING);
        if (status < 0) {
            LOG_WRN("PING opcode failed with status %d", status);
        } else {
            LOG_DBG("PING sent");
            client_instance.ping_tracker_.state_ = KA_STATE_WAITING_FOR_PONG;
            client_instance.ping_tracker_.last_timestamp_ = k_uptime_get();
        }
    }
    while (1) {
        struct ClientApiRequest *rx_event = k_fifo_get(
            &client_instance.bus_, K_NO_WAIT
        );
        if (rx_event == NULL) return;
        clientDispatchRequest(&rx_event->api_request_);
        k_free(rx_event);
    }
}

static void clientHandleResponse(size_t data_size) {
    bool status = false;
    PongBackend_v1_embeded_EmbededResponse *response = DecodeResponse(
        &status, response_buf, data_size
    );
    if (status == false) {
        LOG_WRN("Failed to decode response");
        return;
    }
    HandleClientResponse(response);
    k_free(response);
}

static void clientResetConnection() {
    clientCleanupSocket();
    HandleClientConnecting();
    client_instance.state_ = CLIENT_STATE_CONNECTING;
}

static void clientAcceptResponses() {
    size_t read_amount = 0;
    while (1) {
        socketRecvResponse response_type = recvSocketMsg(
            client_instance.socket_handle_,
            response_buf, PongBackend_v1_embeded_EmbededResponse_size,
            &read_amount
        );
        switch (response_type) {
            case SOCKET_RECV_BAD_RESPONSE:
                LOG_DBG("RECEIVED BAD RESPONSE");
                break;
            case SOCKET_RECV_CLOSE:
                LOG_DBG("RECEIVED CLOSE");
                clientResetConnection();
                return;
            case SOCKET_RECV_FATAL_ERROR:
                LOG_DBG("RECEIVED FATAL ERROR");
                clientResetConnection();
                return;
            case SOCKET_RECV_PING:
                LOG_DBG("RECEIVED PING");
                client_instance.need_pong_ = true;
                break;
            case SOCKET_RECV_PONG:
                LOG_DBG("RECEIVED PONG");
                if (client_instance.ping_tracker_.state_ == KA_STATE_WAITING_FOR_PONG) {
                    client_instance.ping_tracker_.last_timestamp_ = k_uptime_get();
                    client_instance.ping_tracker_.state_ = KA_STATE_SUCCESS_TIMEOUT;
                }
                break;
            case SOCKET_RECV_HANDLED:
                LOG_DBG("RECEIVED HANDLED");
                clientHandleResponse(read_amount);
                break;
            case SOCKET_RECV_EMPTY:
                return;
        }
    }
}

static bool clientCheckKeepAlive() {
    const int64_t current_timestamp = k_uptime_get();
    const int64_t elapsed = current_timestamp - client_instance.ping_tracker_.last_timestamp_;
    const keepAliveState state = client_instance.ping_tracker_.state_;

    if (state == KA_STATE_PING || state == KA_STATE_WAITING_FOR_PONG) {
        if (elapsed > PING_PONG_TIMEOUT) {
            LOG_WRN("Server not available");
            return false;
        }
    } else {
        if (elapsed > KEEP_ALIVE_TIMEOUT) {
            LOG_INF("Time to request PING");
            client_instance.ping_tracker_.state_ = KA_STATE_PING;
            client_instance.ping_tracker_.last_timestamp_ = current_timestamp;
        }
    }
    return true;
}

static void clientRunStateRunning() {
    bool connection_is_alive = clientCheckKeepAlive();
    if (!connection_is_alive) {
        clientResetConnection();
        return;
    }
    clientDispatchRequests();
    clientAcceptResponses();
    k_sleep(K_MSEC(500));
}

void clientRun() {
    while (1) {
        switch(client_instance.state_) {
            case CLIENT_STATE_WAITING_NET:
                if (client_instance.network_up_ == true) {
                    HandleClientConnecting();
                    client_instance.state_ = CLIENT_STATE_CONNECTING;
                }
                break;
            case CLIENT_STATE_CONNECTING:
                if (client_instance.network_up_ == false) {
                    clientCleanupSocket();
                } else {
                    clientRunStateConnecting();
                }
                break;
            case CLIENT_STATE_RUNNING:
                if (client_instance.network_up_ == false) {
                    clientCleanupSocket();
                } else {
                    clientRunStateRunning();
                }
                break;    
        }
        k_sleep(K_MSEC(50));
    }
}