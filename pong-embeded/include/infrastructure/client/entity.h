
#ifndef CLIENT_ENTITY_H
#define CLIENT_ENTITY_H

#include <zephyr.h>
#include "api/embeded.pb.h"

typedef struct ClientApiRequest {
    void *fifo_reserved; /* 1st word reserved for use by fifo */
    PongBackend_v1_embeded_EmbededRequest api_request_;
};

typedef enum {
    CLIENT_STATE_WAITING_NET = 0,
    CLIENT_STATE_CONNECTING = 1,
    CLIENT_STATE_RUNNING = 2,
} clientState;

typedef struct client {
    struct k_fifo bus_;    
    bool network_up_;
    clientState state_;
    int socket_handle_;
};

extern struct client client_instance;

void clientStartup();
void clientRun();

#endif /* CLIENT_ENTITY_H */