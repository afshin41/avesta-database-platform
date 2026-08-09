#ifndef ACCESS_CONTROL_MANAGER_TYPES_H
#define ACCESS_CONTROL_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    ACCESS_CONTROL_MANAGER_CREATED,
    ACCESS_CONTROL_MANAGER_INITIALIZED,
    ACCESS_CONTROL_MANAGER_RUNNING,
    ACCESS_CONTROL_MANAGER_STOPPED,
    ACCESS_CONTROL_MANAGER_ERROR

} access_control_manager_state_t;

typedef enum
{
    ACCESS_CONTROL_DENIED = 0,
    ACCESS_CONTROL_ALLOWED = 1

} access_control_result_t;

typedef struct
{
    uint32_t user_id;
    uint32_t permissions;
    int active;

} access_control_entry_t;

typedef struct
{
    access_control_manager_state_t state;
    uint32_t max_users;
    uint32_t active_users;
    int initialized;
    int running;
    int shutdown;

} access_control_manager_context_t;

#endif
