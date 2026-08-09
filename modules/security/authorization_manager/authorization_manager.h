#ifndef AUTHORIZATION_MANAGER_H
#define AUTHORIZATION_MANAGER_H

#include "authorization_manager_types.h"

int authorization_manager_init(void);

int authorization_manager_start(void);

int authorization_manager_stop(void);

int authorization_manager_shutdown(void);

int authorization_manager_set_max_users(uint32_t max_users);

int authorization_manager_add_user(
    uint32_t permissions,
    uint32_t* user_id
);

int authorization_manager_remove_user(
    uint32_t user_id
);

int authorization_manager_check_permission(
    uint32_t user_id,
    uint32_t permission
);

authorization_manager_state_t
authorization_manager_get_state(void);

authorization_manager_context_t*
authorization_manager_get_context(void);

#endif
