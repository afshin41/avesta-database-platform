#ifndef DATABASE_SERVER_MANAGER_H
#define DATABASE_SERVER_MANAGER_H

#include "database_server_manager_types.h"

int database_server_manager_init(void);

int database_server_manager_start(void);

int database_server_manager_stop(void);

int database_server_manager_shutdown(void);

database_server_manager_state_t
database_server_manager_get_state(void);

const database_server_manager_context_t*
database_server_manager_get_context(void);

#endif
