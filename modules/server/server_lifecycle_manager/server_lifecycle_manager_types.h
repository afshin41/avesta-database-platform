#ifndef SERVER_LIFECYCLE_MANAGER_TYPES_H
#define SERVER_LIFECYCLE_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    SERVER_LIFECYCLE_MANAGER_CREATED,
    SERVER_LIFECYCLE_MANAGER_INITIALIZED,
    SERVER_LIFECYCLE_MANAGER_RUNNING,
    SERVER_LIFECYCLE_MANAGER_STOPPED,
    SERVER_LIFECYCLE_MANAGER_ERROR

} server_lifecycle_manager_state_t;


typedef struct
{
    server_lifecycle_manager_state_t state;
    uint8_t initialized;
    uint8_t running;
    uint8_t shutdown;

} server_lifecycle_manager_context_t;


#endif
