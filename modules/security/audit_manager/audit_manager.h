#ifndef AUDIT_MANAGER_H
#define AUDIT_MANAGER_H

#include <stdint.h>

#include "audit_manager_types.h"
#include "audit_manager_statistics.h"

int audit_manager_init(void);
int audit_manager_start(void);
int audit_manager_stop(void);
int audit_manager_shutdown(void);

int audit_manager_set_max_events(uint32_t max_events);

int audit_manager_record_event(
    audit_event_type_t type,
    uint32_t user_id,
    uint32_t session_id,
    uint32_t code,
    uint64_t timestamp,
    uint64_t *event_id
);

int audit_manager_get_event(
    uint64_t event_id,
    audit_event_t *event
);

int audit_manager_clear_event(uint64_t event_id);

audit_manager_state_t
audit_manager_get_state(void);

audit_manager_context_t *
audit_manager_get_context(void);

#endif
