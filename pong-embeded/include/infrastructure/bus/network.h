
#ifndef BUS_NETWORK_H
#define BUS_NETWORK_H

#include "infrastructure/client/service.h"

#include "domain/status/service.h"

void HandleNetworkIsUp() {
    ClientNetworkUp();
    StatusNetworkUp();
}

void HandleNetworkIsDown() {
    ClientNetworkDown();
    StatusNetworkDown();
}

void HandleNetworkConnecting() {
    ClientNetworkDown();
    StatusNetworkConnecting();
}


#endif /* BUS_NETWORK_H */