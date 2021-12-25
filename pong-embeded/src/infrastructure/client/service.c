
#include "infrastructure/client/service.h"

#include "infrastructure/client/entity.h"

void ClientSetup() {
    clientStartup();
}

void ClientServiceEntry(void * p1, void * p2, void * p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    clientRun();
}

void ClientSendMessage(PongBackend_v1_embeded_EmbededRequest *request) {
    struct ClientApiRequest tx_event = { .api_request_ = *request };
    
    size_t size = sizeof(struct ClientApiRequest);
	char *mem_ptr = k_malloc(size);
	__ASSERT_NO_MSG(mem_ptr != 0);

	memcpy(mem_ptr, &tx_event, size);

    k_fifo_put(&client_instance.bus_, mem_ptr);
}

void ClientNetworkDown() {
    client_instance.network_up_ = false;
}

void ClientNetworkUp() {
    client_instance.network_up_ = true;
}