#ifndef DATABASE_SERVER_MANAGER_TYPES_H
#define DATABASE_SERVER_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    DATABASE_SERVER_MANAGER_CREATED = 0,
    DATABASE_SERVER_MANAGER_INITIALIZED,
    DATABASE_SERVER_MANAGER_RUNNING,
    DATABASE_SERVER_MANAGER_STOPPED,
    DATABASE_SERVER_MANAGER_ERROR

} database_server_manager_state_t;


typedef struct
{
    database_server_manager_state_t state;
    uint8_t initialized;
    uint8_t running;
    uint8_t shutdown;

} database_server_manager_context_t;


#endif
