
#include "application.h"

struct Application MainApplication;

void ApplicationStartup() {
    
    /* intialize members */
    k_mutex_init(&MainApplication.state_change_lock_);

    /* choose startup state */

}

void applicationStateTransition(ApplicationState new_state) {
    k_mutex_lock(&MainApplication.state_change_lock_, K_FOREVER);

    applicationEnterState();
    k_mutex_unlock(&MainApplication.state_change_lock_);
}

void applicationEnterState() {

}