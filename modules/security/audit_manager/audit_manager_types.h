#ifndef AUDIT_MANAGER_TYPES_H
#define AUDIT_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    AUDIT_MANAGER_CREATED,
    AUDIT_MANAGER_INITIALIZED,
    AUDIT_MANAGER_RUNNING,
    AUDIT_MANAGER_STOPPED,
    AUDIT_MANAGER_ERROR
} audit_manager_state_t;

typedef enum
{
    AUDIT_EVENT_INFO,
    AUDIT_EVENT_WARNING,
    AUDIT_EVENT_SECURITY,
    AUDIT_EVENT_ERROR
} audit_event_type_t;

typedef struct
{
    uint64_t event_id;
    audit_event_type_t type;
    uint64_t timestamp;
    uint32_t user_id;
    uint32_t session_id;
    uint32_t code;
} audit_event_t;

typedef struct
{
    audit_manager_state_t state;
    uint32_t max_events;
    uint32_t active_events;
    uint64_t next_event_id;
    int initialized;
    int running;
    int shutdown;
} audit_manager_context_t;

#endif
