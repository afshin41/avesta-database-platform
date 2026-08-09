#ifndef SERVER_LIFECYCLE_MANAGER_H
#define SERVER_LIFECYCLE_MANAGER_H

#include "server_lifecycle_manager_types.h"

int server_lifecycle_manager_init(void);

int server_lifecycle_manager_start(void);

int server_lifecycle_manager_stop(void);

int server_lifecycle_manager_shutdown(void);

server_lifecycle_manager_state_t
server_lifecycle_manager_get_state(void);

server_lifecycle_manager_context_t*
server_lifecycle_manager_get_context(void);

#endif
