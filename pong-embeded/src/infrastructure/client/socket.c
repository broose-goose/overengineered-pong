
#include "infrastructure/client/socket.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(client_socket, LOG_LEVEL_DBG);

#include <net/websocket.h>

/* graciously copied from zephyr / samples / net / websocket */

int setupSocket(
    sa_family_t family, const char *server, int port,
	int *sock, struct sockaddr *addr, socklen_t addr_len
) {
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

int connectSocket(
    sa_family_t family, const char *server, int port,
	int *sock, struct sockaddr *addr, socklen_t addr_len
) {
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

ssize_t sendSocketMsg(int sock, const void *buf, size_t len)
{
	return websocket_send_msg(
		sock, buf, len, WEBSOCKET_OPCODE_DATA_BINARY,
		true, true, 5 * MSEC_PER_SEC
	);
}


socketRecvResponse recvSocketMsg(int sock, size_t amount, uint8_t *buf, size_t buf_len) {
	
	uint64_t remaining = ULLONG_MAX;
	int total_read;
	uint32_t message_type;
	int ret, read_pos;

	read_pos = 0;
	total_read = 0;

	while (remaining > 0) {
		ret = websocket_recv_msg(
			sock, buf + read_pos,
			buf_len - read_pos,
			&message_type,
			&remaining,
			0
		);
		if (ret <= 0) {
			if (ret == -EAGAIN) {
				return SOCKET_EMPTY;
			} else {
				LOG_DBG(
					"socket connection closed... or something (%d/%d)", ret, errno
				);
				return SOCKET_FATAL_ERROR;
			}

		}
		read_pos += ret;
		total_read += ret;
	}

	if (remaining != 0) {
		LOG_DBG("Socket returned some bullshit");
		return SOCKET_BAD_RESPONSE;
	} 
	if (total_read != amount) {
		LOG_DBG("Socket probably messed up");
	}
	return SOCKET_HANDLED;
}