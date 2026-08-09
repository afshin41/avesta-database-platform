#ifndef SESSION_SECURITY_MANAGER_H
#define SESSION_SECURITY_MANAGER_H

#include "session_security_manager_types.h"

int session_security_manager_init(void);

int session_security_manager_start(void);

int session_security_manager_stop(void);

int session_security_manager_shutdown(void);

int session_security_manager_set_max_sessions(
    uint32_t max_sessions
);

int session_security_manager_create(
    uint32_t user_id,
    uint32_t lifetime,
    uint32_t* session_id,
    uint64_t* token
);

int session_security_manager_validate(
    uint32_t session_id,
    uint64_t token
);

int session_security_manager_destroy(
    uint32_t session_id
);

int session_security_manager_expire(
    uint32_t session_id
);

session_security_manager_state_t
session_security_manager_get_state(void);

session_security_manager_context_t*
session_security_manager_get_context(void);

#endif
