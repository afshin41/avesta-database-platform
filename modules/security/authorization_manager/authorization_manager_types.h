#ifndef AUTHORIZATION_MANAGER_TYPES_H
#define AUTHORIZATION_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    AUTHORIZATION_MANAGER_CREATED,
    AUTHORIZATION_MANAGER_INITIALIZED,
    AUTHORIZATION_MANAGER_RUNNING,
    AUTHORIZATION_MANAGER_STOPPED,
    AUTHORIZATION_MANAGER_ERROR
} authorization_manager_state_t;

typedef struct
{
    uint32_t user_id;
    uint32_t permissions;
    int active;
} authorization_manager_user_t;

typedef struct
{
    authorization_manager_state_t state;
    int initialized;
    int running;
    int shutdown;

    uint32_t max_users;
    uint32_t active_users;

    uint32_t next_user_id;

} authorization_manager_context_t;

#endif
