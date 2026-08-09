#ifndef ACCESS_CONTROL_MANAGER_STATISTICS_H
#define ACCESS_CONTROL_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;
    uint64_t user_add_count;
    uint64_t user_remove_count;
    uint64_t permission_grant_count;
    uint64_t permission_revoke_count;
    uint64_t permission_denied_count;
    uint64_t access_allow_count;
    uint64_t access_deny_count;

} access_control_manager_statistics_t;

void access_control_manager_statistics_init(void);

access_control_manager_statistics_t*
access_control_manager_get_statistics(void);

#endif
