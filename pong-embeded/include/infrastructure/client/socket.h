
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <net/socket.h>
#include <net/websocket.h>

int setupSocket(
    sa_family_t family, const char *server, int port,
	int *sock, struct sockaddr *addr, socklen_t addr_len
);

int connectSocket(
    sa_family_t family, const char *server, int port,
	int *sock, struct sockaddr *addr, socklen_t addr_len
);

ssize_t sendSocketData(int sock, const void *buf, size_t len);

typedef enum {
    SOCKET_SEND_PING = WEBSOCKET_OPCODE_PING,
    SOCKET_SEND_PONG = WEBSOCKET_OPCODE_PONG,
    SOCKET_SEND_CLOSE = WEBSOCKET_OPCODE_CLOSE
} socketSendOp;

ssize_t sendSocketOp(int sock, socketSendOp req_type);

typedef enum {
    SOCKET_RECV_BAD_RESPONSE = -2,
    SOCKET_RECV_FATAL_ERROR = -1,
    SOCKET_RECV_HANDLED = 0,
    SOCKET_RECV_EMPTY = 1,
    SOCKET_RECV_PING = 2,
    SOCKET_RECV_PONG = 3,
    SOCKET_RECV_CLOSE = 4,
} socketRecvResponse;

socketRecvResponse recvSocketMsg(int sock, uint8_t *buf, size_t buf_len, size_t *read_amount);

#endif /* CLIENT_SOCKET_H */