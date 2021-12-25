
#ifndef BUS_CLIENT_H
#define BUS_CLIENT_H

#include "api/embeded.pb.h"

void HandleClientIsUp();

void HandleClientIsDown();

void HandleClientConnecting();

void handleClientSendPong();

void HandleClientResponse(PongBackend_v1_embeded_EmbededResponse *response);

#endif /* BUS_CLIENT_H */