
#include "infrastructure/bus/client.h"

#include "domain/status/service.h"
#include "infrastructure/client/service.h"
#include "api/api.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(bus_client, LOG_LEVEL_DBG);

void HandleClientIsUp() {
    StatusClientUp();
}

void HandleClientIsDown() {
    StatusClientDown();
}

void HandleClientConnecting() {
    StatusClientConnecting();
}

void handleClientSendPong() {
    bool status = false;
    PongBackend_v1_embeded_EmbededRequest *pong_request = GeneratePongMessage(&status);
    if (!status) {
        return;
    }
    ClientSendMessage(pong_request);
    k_free(pong_request);
}

void HandleClientResponse(PongBackend_v1_embeded_EmbededResponse *response) {
    LOG_INF(
        "Handling client response of type: %d",
        response->which_response, PongBackend_v1_embeded_EmbededResponse_ping_tag
    );
    switch(response->which_response) {
        case PongBackend_v1_embeded_EmbededResponse_ping_tag:
            handleClientSendPong();
            break;
    }
}