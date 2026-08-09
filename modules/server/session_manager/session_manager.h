#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "session_manager_types.h"

int session_manager_init(void);
int session_manager_start(void);
int session_manager_stop(void);
int session_manager_shutdown(void);

int session_manager_set_max_sessions(uint32_t max_sessions);

int session_manager_open(uint32_t* session_id);
int session_manager_close(uint32_t session_id);

session_manager_state_t session_manager_get_state(void);
session_manager_context_t* session_manager_get_context(void);

#endif
