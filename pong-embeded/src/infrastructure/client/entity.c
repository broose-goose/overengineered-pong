
#include "infrastructure/client/entity.h"

#include "infrastructure/client/socket.h"

#include "api/api.h"
#include "api/embeded.pb.h"

#include "infrastructure/bus/client.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(client_entity, LOG_LEVEL_DBG);

#include <net/websocket.h>



// this should be somekind of config
#define SERVER_ADDRESS "192.168.1.30"
#define SERVER_PORT 9001

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

static void clientCleanupSocket() {
    if (client_instance.socket_handle_ >= 0) {
		close(client_instance.socket_handle_);
        client_instance.socket_handle_ = -1;
	}
    HandleClientIsDown();
    client_instance.state_ = CLIENT_STATE_WAITING_NET;
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
		LOG_ERR("Cannot connect to %s:%d", SERVER_ADDRESS, SERVER_PORT);
		close(socket);
        client_instance.socket_handle_ = -1;
        return;
	}

    HandleClientIsUp();
    client_instance.state_ = CLIENT_STATE_RUNNING;
}

static void clientDispatchRequest(PongBackend_v1_embeded_EmbededRequest *request) {
    size_t written_length = 0;
    bool status = EncodeRequest(
        io_buf, PongBackend_v1_embeded_EmbededRequest_size, &written_length, request
    );
    if (status == false) {
        LOG_ERR("Failed to encode request");
        return;
    }
    sendSocketMsg(client_instance.socket_handle_, io_buf, written_length);
}

static void clientDispatchRequests() {
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
        LOG_ERR("Failed to decode response");
        return;
    }
    HandleClientResponse(response);
    k_free(response);
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
            case SOCKET_BAD_RESPONSE:
                break;
            case SOCKET_FATAL_ERROR:
                // probably disconnected or something jank happend; reset
                clientCleanupSocket();
                HandleClientConnecting();
                client_instance.state_ = CLIENT_STATE_CONNECTING;
                return;
            case SOCKET_HANDLED:
                clientHandleResponse(read_amount);
                break;
            case SOCKET_EMPTY:
                k_sleep(K_MSEC(250));
                return;
        }
    }
}

static void clientRunStateRunning() {
    clientDispatchRequests();
    clientAcceptResponses();
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