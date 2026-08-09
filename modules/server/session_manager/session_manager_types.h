#ifndef SESSION_MANAGER_TYPES_H
#define SESSION_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    SESSION_MANAGER_CREATED,
    SESSION_MANAGER_INITIALIZED,
    SESSION_MANAGER_RUNNING,
    SESSION_MANAGER_STOPPED,
    SESSION_MANAGER_ERROR
} session_manager_state_t;

typedef struct
{
    uint32_t session_id;
    uint8_t active;
} session_manager_session_t;

typedef struct
{
    session_manager_state_t state;
    uint32_t max_sessions;
    uint32_t active_sessions;
    uint32_t next_session_id;
    int initialized;
    int running;
    int shutdown;
} session_manager_context_t;

#endif
