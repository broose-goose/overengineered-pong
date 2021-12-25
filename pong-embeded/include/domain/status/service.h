
#ifndef STATUS_SERVICE_H
#define STATUS_SERVICE_H

#include "domain/status/entity.h"

void StatusSetup();

void StatusClientUp();
void StatusClientDown();
void StatusClientConnecting();

void StatusNetworkUp();
void StatusNetworkDown();
void StatusNetworkConnecting();


#endif /* STATUS_SERVICE_H */