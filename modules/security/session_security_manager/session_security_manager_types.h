#ifndef SESSION_SECURITY_MANAGER_TYPES_H
#define SESSION_SECURITY_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    SESSION_SECURITY_MANAGER_CREATED,
    SESSION_SECURITY_MANAGER_INITIALIZED,
    SESSION_SECURITY_MANAGER_RUNNING,
    SESSION_SECURITY_MANAGER_STOPPED,
    SESSION_SECURITY_MANAGER_ERROR
} session_security_manager_state_t;

typedef struct
{
    uint32_t session_id;
    uint64_t token;
    uint32_t user_id;
    uint32_t lifetime;
    uint32_t age;
    int active;
} session_security_manager_session_t;

typedef struct
{
    session_security_manager_state_t state;

    int initialized;
    int running;
    int shutdown;

    uint32_t max_sessions;
    uint32_t active_sessions;
    uint32_t next_session_id;

} session_security_manager_context_t;

#endif
