
#include <zephyr.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(net_websocket_client_sample, LOG_LEVEL_DBG);

#include "common.h"
#include "run-pid.h"
#include "set-target.h"

#include <net/net_if.h>
#include <net/net_core.h>
#include <net/net_context.h>
#include <net/net_mgmt.h>

#include <kernel.h>

struct net_mgmt_event_callback iface_callback;

static struct net_mgmt_event_callback mgmt_cb;

static void handler(struct net_mgmt_event_callback *cb,
		    uint32_t mgmt_event,
		    struct net_if *iface
) {
	if (mgmt_event == NET_EVENT_IF_UP) {
		LOG_INF("UP :D");
	} else if (mgmt_event == NET_EVENT_IF_DOWN) {
		LOG_INF("DOWN D:");
	}
}

void main(void)
{

	k_sleep(K_SECONDS(5));

	struct net_if *iface;

    net_mgmt_init_event_callback(&iface_callback, handler, NET_EVENT_IF_UP | NET_EVENT_IF_DOWN);
    net_mgmt_add_event_callback(&iface_callback);

	iface = net_if_get_default();

	bool is_up = net_if_is_up(iface);
	if (is_up) {
		LOG_INF("UP :D");
	} else {
		LOG_INF("DOWN D:");
	}

	net_dhcpv4_start(iface);
}