
#include "api/api.h"

#include "api/common.pb.h"

#include <zephyr.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(api, LOG_LEVEL_DBG);

#include <pb_encode.h>
#include <pb_decode.h>


PongBackend_v1_embeded_EmbededRequest *GeneratePongMessage(bool *status) {
    
    PongBackend_v1_common_Pong pong_msg = PongBackend_v1_common_Pong_init_zero;

    PongBackend_v1_embeded_EmbededRequest api_msg = PongBackend_v1_embeded_EmbededRequest_init_zero;
    api_msg.which_request = PongBackend_v1_embeded_EmbededRequest_pong_tag;
    api_msg.request.pong = pong_msg;

    
    size_t size = sizeof(PongBackend_v1_embeded_EmbededRequest);
	PongBackend_v1_embeded_EmbededRequest *api_ptr = k_malloc(size);
	if (api_ptr == 0) {
        *status = false;
        LOG_ERR("Malloc failed; heap full");
        return NULL;
    }
    
    memcpy(api_ptr, &pong_msg, size);

    return api_ptr;
}

PongBackend_v1_embeded_EmbededRequest *GeneratePositionMessage(uint32_t position, bool *status) {
    
    PongBackend_v1_common_Position position_msg = PongBackend_v1_common_Position_init_zero;

    PongBackend_v1_embeded_EmbededRequest api_msg = PongBackend_v1_embeded_EmbededRequest_init_zero;
    api_msg.which_request = PongBackend_v1_embeded_EmbededRequest_position_tag;
    api_msg.request.position = position_msg;

    size_t size = sizeof(PongBackend_v1_embeded_EmbededRequest);
	PongBackend_v1_embeded_EmbededRequest *api_ptr = k_malloc(size);
	if (api_ptr == 0) {
        *status = false;
        LOG_ERR("Malloc failed; heap full");
        return NULL;
    }
    
    memcpy(api_ptr, &position_msg, size);

    return api_ptr;
}

bool EncodeRequest(
    uint8_t *buffer, size_t buffer_size, size_t *out_length,
    PongBackend_v1_embeded_EmbededRequest *request
) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);
    
    bool status = pb_encode(&stream, PongBackend_v1_embeded_EmbededRequest_fields, request);

	if (status == false) {
		LOG_ERR("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return false;
	}
    	
    *out_length = stream.bytes_written;

    return status;
}

PongBackend_v1_embeded_EmbededResponse *DecodeResponse(bool *status, uint8_t *buffer, size_t message_length) {

    size_t size = sizeof(PongBackend_v1_embeded_EmbededResponse);
	PongBackend_v1_embeded_EmbededResponse *api_ptr = k_malloc(size);
	if (api_ptr == 0) {
        *status = false;
        LOG_ERR("Malloc failed; heap full");
        return NULL;
    }

    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    *status = pb_decode(&stream, PongBackend_v1_embeded_EmbededResponse_fields, api_ptr);

    if (*status == false) {
		LOG_ERR("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        k_free(api_ptr);
        return NULL;
	}

    return api_ptr;
}