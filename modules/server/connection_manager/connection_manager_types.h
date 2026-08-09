#ifndef CONNECTION_MANAGER_TYPES_H
#define CONNECTION_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    CONNECTION_MANAGER_CREATED,
    CONNECTION_MANAGER_INITIALIZED,
    CONNECTION_MANAGER_RUNNING,
    CONNECTION_MANAGER_STOPPED,
    CONNECTION_MANAGER_ERROR

} connection_manager_state_t;

typedef struct
{
    uint32_t connection_id;
    uint8_t active;

} connection_manager_connection_t;

typedef struct
{
    connection_manager_state_t state;
    uint32_t max_connections;
    uint32_t active_connections;
    uint32_t next_connection_id;
    uint8_t initialized;
    uint8_t running;
    uint8_t shutdown;

} connection_manager_context_t;

#endif
