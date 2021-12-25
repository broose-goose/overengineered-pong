
#include "infrastructure/network/net.h"
#include "infrastructure/network/entity.h"

void networkIfaceCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
) {
	if (mgmt_event == NET_EVENT_IF_UP) {
		networkSendEvent(NETWORK_EVENT_ETHERNET_UP);
	} else if (mgmt_event == NET_EVENT_IF_DOWN) {
		networkSendEvent(NETWORK_EVENT_ETHERNET_DOWN);
	}
}

void networkDhcpCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
) {
	if (mgmt_event == NET_EVENT_IPV4_DHCP_BOUND) {
		networkSendEvent(NETWORK_EVENT_DHCP_UP);
	} else if (mgmt_event == NET_EVENT_IPV4_DHCP_STOP) {
		networkSendEvent(NETWORK_EVENT_DHCP_DOWN);
	}
}
