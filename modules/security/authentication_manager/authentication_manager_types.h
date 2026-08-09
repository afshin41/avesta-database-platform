#ifndef AUTHENTICATION_MANAGER_TYPES_H
#define AUTHENTICATION_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    AUTHENTICATION_MANAGER_CREATED,
    AUTHENTICATION_MANAGER_INITIALIZED,
    AUTHENTICATION_MANAGER_RUNNING,
    AUTHENTICATION_MANAGER_STOPPED,
    AUTHENTICATION_MANAGER_ERROR
} authentication_manager_state_t;

typedef enum
{
    AUTHENTICATION_RESULT_SUCCESS = 0,
    AUTHENTICATION_RESULT_INVALID_ARGUMENT = -1,
    AUTHENTICATION_RESULT_NOT_INITIALIZED = -2,
    AUTHENTICATION_RESULT_NOT_RUNNING = -3,
    AUTHENTICATION_RESULT_INVALID_CREDENTIALS = -4,
    AUTHENTICATION_RESULT_ALREADY_AUTHENTICATED = -5,
    AUTHENTICATION_RESULT_NOT_AUTHENTICATED = -6,
    AUTHENTICATION_RESULT_LIMIT_REACHED = -7,
    AUTHENTICATION_RESULT_SHUTDOWN = -8
} authentication_result_t;

typedef struct
{
    uint32_t user_id;
    uint32_t session_id;
    uint32_t authenticated;
    uint32_t failed_attempts;
} authentication_identity_t;

typedef struct
{
    authentication_manager_state_t state;
    uint32_t initialized;
    uint32_t running;
    uint32_t shutdown;
    uint32_t authenticated;
    uint32_t failed_attempts;
    uint32_t max_failed_attempts;
    uint32_t active_session_id;
} authentication_manager_context_t;

#endif
