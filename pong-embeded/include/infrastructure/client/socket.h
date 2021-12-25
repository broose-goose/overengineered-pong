
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <net/socket.h>

int setupSocket(
    sa_family_t family, const char *server, int port,
	int *sock, struct sockaddr *addr, socklen_t addr_len
);

int connectSocket(
    sa_family_t family, const char *server, int port,
	int *sock, struct sockaddr *addr, socklen_t addr_len
);

ssize_t sendSocketMsg(int sock, const void *buf, size_t len);

typedef enum {
    SOCKET_BAD_RESPONSE = -2,
    SOCKET_FATAL_ERROR = -1,
    SOCKET_HANDLED = 0,
    SOCKET_EMPTY = 1,
} socketRecvResponse;

socketRecvResponse recvSocketMsg(int sock, size_t amount, uint8_t *buf, size_t buf_len);

#endif /* CLIENT_SOCKET_H */