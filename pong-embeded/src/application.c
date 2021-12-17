
#include "application.h"
#include "connection.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(application, LOG_LEVEL_DBG);

#include <net/net_if.h>

struct Application MainApplication;

void applicationIfaceCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
) {
	if (mgmt_event == NET_EVENT_IF_UP) {
		ApplicationSendEvent(EVENT_ETHERNET_UP);
	} else if (mgmt_event == NET_EVENT_IF_DOWN) {
		ApplicationSendEvent(EVENT_ETHERNET_DOWN);
	}
}

void applicationDhcpCallback(
    struct net_mgmt_event_callback *cb,
	uint32_t mgmt_event,
	struct net_if *iface
) {
	if (mgmt_event == NET_EVENT_IPV4_DHCP_BOUND) {
		ApplicationSendEvent(EVENT_DHCP_UP);
	} else if (mgmt_event == NET_EVENT_IPV4_DHCP_STOP) {
		ApplicationSendEvent(EVENT_DHCP_DOWN);
	}
}

void ApplicationStartup() {

    LOG_INF("Waiting for ethernet to initialize if present");
    k_sleep(K_SECONDS(5));
    LOG_INF("Running startup sequence");
    
    /* intialize members */
    k_mutex_init(&MainApplication.state_change_lock_);
    k_fifo_init(&MainApplication.bus_);

    /* setup listeners */
    net_mgmt_init_event_callback(
        &MainApplication.iface_callback_, applicationIfaceCallback, NET_EVENT_IF_UP | NET_EVENT_IF_DOWN
    );
    net_mgmt_init_event_callback(
        &MainApplication.dhcp_callback_, applicationDhcpCallback, NET_EVENT_IPV4_DHCP_BOUND | NET_EVENT_IPV4_DHCP_STOP
    );
    net_mgmt_add_event_callback(&MainApplication.iface_callback_);
    net_mgmt_add_event_callback(&MainApplication.dhcp_callback_);

    /* choose startup state */
	struct net_if *iface;
    iface = net_if_get_default();

	bool is_up = net_if_is_up(iface);
	if (is_up) {
		ApplicationSendEvent(EVENT_ETHERNET_UP);
	} else {
		ApplicationSendEvent(EVENT_ETHERNET_DOWN);
	}

}

void ApplicationRun() {
    while (1) {
        applicationProcessEvents();
        k_sleep(K_MSEC(100));
    }
}

void ApplicationSendEvent(ApplicationEvent e) {

    struct ApplicationEventMessage tx_event = { .application_event_ = e };
    
    size_t size = sizeof(struct ApplicationEventMessage);
	char *mem_ptr = k_malloc(size);
	__ASSERT_NO_MSG(mem_ptr != 0);

	memcpy(mem_ptr, &tx_event, size);

    k_fifo_put(&MainApplication.bus_, mem_ptr);

}

void applicationProcessEvents() {
    while (1) {
        struct ApplicationEventMessage *rx_event = k_fifo_get(
            &MainApplication.bus_, K_NO_WAIT
        );
        if (rx_event == NULL) return;
        applicationHandleEvent(rx_event->application_event_);
        k_free(rx_event);
    }
}

void applicationHandleEvent(ApplicationEvent e) {
    LOG_INF("recieved event: %d", e);
    if (e == EVENT_ETHERNET_DOWN) {
        if (MainApplication.state_ == APP_RUNNING) {
            applicationStopRunningState();
            applicationStopDhcpState();
        } else if (MainApplication.state_ == APP_WAITING_DHCP) {
            applicationStopDhcpState();
        }
        MainApplication.state_ = APP_WAITING_NET;
    } else if (e == EVENT_ETHERNET_UP) {
        if (MainApplication.state_ != APP_WAITING_NET) {
            return;
        }
        applicationStartDhcpState();
        MainApplication.state_ = APP_WAITING_DHCP;
    } else if (e == EVENT_DHCP_DOWN) {
        if (MainApplication.state_ == APP_WAITING_NET) {
            return;
        } else if (MainApplication.state_ == APP_RUNNING) {
            applicationStopRunningState();
        }
        applicationResetDhcpState();
        MainApplication.state_ = APP_WAITING_DHCP;
    } else if (e == EVENT_DHCP_UP) {
        if (MainApplication.state_ != APP_WAITING_DHCP) {
            return;
        }
        applicationStartRunningState();
        MainApplication.state_ = APP_RUNNING;
    } else if (e == EVENT_RUNNING_ERROR) {
        if (MainApplication.state_ != APP_RUNNING) {
            return;
        }
        applicationResetRunningState();
    }
}

void applicationStopDhcpState() {
    LOG_INF("Stop dhcp");
    struct net_if *iface;
    iface = net_if_get_default();
    net_dhcpv4_stop(iface);
}
void applicationStartDhcpState() {
    LOG_INF("Start dhcp");
    struct net_if *iface;
    iface = net_if_get_default();
    net_dhcpv4_start(iface);
}
void applicationResetDhcpState() {
    LOG_INF("Reset dhcp");
    applicationStopDhcpState();
    k_sleep(K_MSEC(10));
    applicationStartDhcpState();
}


void applicationStopRunningState() {
    LOG_INF("Stop running");
    ConnectionStop();
}
void applicationStartRunningState() {
    LOG_INF("Start running");
    ConnectionStart();
}
void applicationResetRunningState() {
    LOG_INF("Reset running");
    applicationStopRunningState();
    k_sleep(K_MSEC(10));
    applicationStartRunningState();
}