
#include "connection.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(connection, LOG_LEVEL_DBG);

#include <net/socket.h>
#include <net/websocket.h>

#include "embeded.pb.h"

#define SERVER_ADDRESS "192.168.1.20"
#define SERVER_PORT 9001

#define CONNECTION_THREAD_STACK 2048

struct Connection ws_connection;
K_THREAD_STACK_DEFINE(connection_thread_stack, 2048);

#define EXTRA_BUF_SPACE 30
static uint8_t temp_recv_buf[PongBackend_v1_embeded_EmbededResponse_size + EXTRA_BUF_SPACE];

static int setupSocket(sa_family_t family, const char *server, int port,
			int *sock, struct sockaddr *addr, socklen_t addr_len)
{
	const char *family_str = family == AF_INET ? "IPv4" : "IPv6";
	int ret = 0;

	memset(addr, 0, addr_len);

	net_sin(addr)->sin_family = AF_INET;
	net_sin(addr)->sin_port = htons(port);
	inet_pton(family, server, &net_sin(addr)->sin_addr);


	*sock = socket(family, SOCK_STREAM, IPPROTO_TCP);

	if (*sock < 0) {
		LOG_ERR("Failed to create %s HTTP socket (%d)", family_str,
			-errno);
	}

	return ret;

fail:
	if (*sock >= 0) {
		close(*sock);
		*sock = -1;
	}

	return ret;
}

static int connectSocket(sa_family_t family, const char *server, int port,
			  int *sock, struct sockaddr *addr, socklen_t addr_len)
{
	int ret;

	ret = setupSocket(family, server, port, sock, addr, addr_len);
	if (ret < 0 || *sock < 0) {
		return -1;
	}

	ret = connect(*sock, addr, addr_len);
	if (ret < 0) {
		LOG_ERR("Cannot connect to %s remote (%d)",
			family == AF_INET ? "IPv4" : "IPv6",
			-errno);
		ret = -errno;
	}

	return ret;
}


void ConnectionStart() {
    if (ws_connection.run_connection_ = true) {
        LOG_ERR("Connection is already running?");
    }
    LOG_INF("Starting WS Connection");
    // setup
    ws_connection.run_connection_ = true;
    ws_connection.is_connected_ = false;
    ws_connection.socket_handle_ = -1;
    // run
	k_thread_create(
        &ws_connection.connection_thread_, connection_thread_stack, K_THREAD_STACK_SIZEOF(connection_thread_stack),
		(k_thread_entry_t) connectionRun, NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT
    );
}

void ConnectionStop() {
    LOG_INF("Stoping WS Connection");
    ws_connection.run_connection_ = false;
    k_thread_join(&ws_connection.connection_thread_, K_FOREVER);
}

void connectionRun() {
    while (1) {
        if (!ws_connection.run_connection_) {
            break;
        }
        if (!ws_connection.is_connected_) {
            tryConnecting();
        } else {
            doConnectionRun();
        }
    }
    // cleanup
    if (ws_connection.socket_handle_ >= 0) {
		close(ws_connection.socket_handle_);
	}
}

void tryConnecting() {
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


	ws_connection.socket_handle_ = websocket_connect(socket, &req, 5 * MSEC_PER_SEC, "IPv4");
	if (ws_connection.socket_handle_ < 0) {
		LOG_ERR("Cannot connect to %s:%d", SERVER_ADDRESS, SERVER_PORT);
		close(socket);
        return;
	}

    ws_connection.is_connected_ = true;
}

void doConnectionRun() {
    LOG_INF("Welp, i got here");
    k_sleep(K_SECONDS(1));
}