
#include "infrastructure/network/service.h"

#include "infrastructure/network/entity.h"

void NetworkSetup() {
    networkStartup();
}

void NetworkServiceEntry(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);
    
    networkRun();
}
