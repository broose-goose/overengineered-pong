
#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

#include "infrastructure/client/entity.h"

void ClientSetup();

void ClientServiceEntry(void *, void *, void *);

void ClientNetworkDown();
void ClientNetworkUp();

void ClientSendMessage(PongBackend_v1_embeded_EmbededRequest *request);

#endif /* CLIENT_SERVICE_H */