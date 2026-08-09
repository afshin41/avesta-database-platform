#ifndef ACCESS_CONTROL_MANAGER_H
#define ACCESS_CONTROL_MANAGER_H

#include "access_control_manager_types.h"

int access_control_manager_init(void);
int access_control_manager_start(void);
int access_control_manager_stop(void);
int access_control_manager_shutdown(void);

int access_control_manager_set_max_users(uint32_t max_users);

int access_control_manager_add_user(uint32_t user_id);
int access_control_manager_remove_user(uint32_t user_id);

int access_control_manager_grant_permission(
    uint32_t user_id,
    uint32_t permission
);

access_control_result_t access_control_manager_check_access(
    uint32_t user_id,
    uint32_t permission
);

int access_control_manager_revoke_permission(
    uint32_t user_id,
    uint32_t permission
);

access_control_manager_state_t
access_control_manager_get_state(void);

access_control_manager_context_t*
access_control_manager_get_context(void);

#endif
