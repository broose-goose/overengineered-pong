
#include "application/application.h"

#include "infrastructure/network/service.h"
#include "infrastructure/client/service.h"

#include "domain/status/service.h"

struct application application_instance;
K_THREAD_STACK_DEFINE(network_thread_stack, 1024);
K_THREAD_STACK_DEFINE(client_thread_stack, 4096);


void ApplicationStartup() {
    /* setup modules */
    NetworkSetup();
    ClientSetup();
    StatusSetup();
    /* run modules */
    k_thread_create(
        &application_instance.network_thread_, network_thread_stack, K_THREAD_STACK_SIZEOF(network_thread_stack),
		(k_thread_entry_t) NetworkServiceEntry, NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT
    );
    k_thread_create(
        &application_instance.client_thread_, client_thread_stack, K_THREAD_STACK_SIZEOF(client_thread_stack),
		(k_thread_entry_t) ClientServiceEntry, NULL, NULL, NULL, K_PRIO_COOP(6), 0, K_NO_WAIT
    );
}