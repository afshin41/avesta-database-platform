#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "connection_manager_types.h"

int connection_manager_init(void);

int connection_manager_start(void);

int connection_manager_stop(void);

int connection_manager_shutdown(void);

int connection_manager_set_max_connections(uint32_t max_connections);

int connection_manager_open(uint32_t* connection_id);

int connection_manager_close(uint32_t connection_id);

connection_manager_state_t
connection_manager_get_state(void);

connection_manager_context_t*
connection_manager_get_context(void);

#endif
