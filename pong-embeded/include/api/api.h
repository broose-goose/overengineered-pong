
#ifndef API_H
#define API_H

#include "api/embeded.pb.h"

PongBackend_v1_embeded_EmbededRequest *GeneratePongMessage(bool *status);

PongBackend_v1_embeded_EmbededRequest *GeneratePositionMessage(uint32_t position, bool *status);

bool EncodeRequest(
    uint8_t *buffer, size_t buffer_size, size_t *out_length,
    PongBackend_v1_embeded_EmbededRequest *request
);

PongBackend_v1_embeded_EmbededResponse *DecodeResponse(bool *status, uint8_t *buffer, size_t message_length);

#endif