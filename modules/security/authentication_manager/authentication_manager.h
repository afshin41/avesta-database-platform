#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include <stdint.h>
#include "authentication_manager_types.h"

int authentication_manager_init(void);
int authentication_manager_start(void);
int authentication_manager_stop(void);
int authentication_manager_shutdown(void);

int authentication_manager_set_credentials(
    uint32_t user_id,
    const char* username,
    const char* password
);

int authentication_manager_authenticate(
    uint32_t user_id,
    const char* username,
    const char* password,
    uint32_t session_id
);

int authentication_manager_logout(void);

authentication_manager_state_t
authentication_manager_get_state(void);

authentication_manager_context_t*
authentication_manager_get_context(void);

int authentication_manager_is_authenticated(void);

#endif
