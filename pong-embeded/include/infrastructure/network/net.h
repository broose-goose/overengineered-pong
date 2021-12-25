
#ifndef NETWORK_NET_H
#define NETWORK_NET_H

#include <net/net_mgmt.h>

void networkIfaceCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
);

void networkDhcpCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
);

#endif /* NETWORK_NET_H */