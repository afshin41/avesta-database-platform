#ifndef AUDIT_MANAGER_STATISTICS_H
#define AUDIT_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;
    uint64_t event_record_count;
    uint64_t event_read_count;
    uint64_t event_clear_count;
} audit_manager_statistics_t;

void audit_manager_statistics_init(void);

audit_manager_statistics_t *
audit_manager_get_statistics(void);

#endif
