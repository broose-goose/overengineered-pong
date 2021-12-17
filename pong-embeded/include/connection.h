
#ifndef CONNECTION_H
#define CONNECTION_H

#include <zephyr.h>

#define EXTRA_BUF_SPACE 30

typedef struct Connection {
    bool run_connection_;
    bool is_connected_;
    int socket_handle_;
    struct k_thread connection_thread_;
};

void ConnectionStart();
void ConnectionStop();

void connectionRun();
void tryConnecting();
void doConnectionRun();

#endif /* CONNECTION_H */