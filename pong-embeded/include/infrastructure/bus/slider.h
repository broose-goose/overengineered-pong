
#include "api/api.h"
#include "infrastructure/client/service.h"

void HandleSliderPositionUpdate(uint32_t position) {
    bool status = false;
    PongBackend_v1_embeded_EmbededRequest *position_request = GeneratePositionMessage(position, &status);
    if (!status) {
        return;
    }
    ClientSendMessage(position_request);
    k_free(position_request);
}