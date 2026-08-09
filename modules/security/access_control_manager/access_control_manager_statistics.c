#include "access_control_manager_statistics.h"

static access_control_manager_statistics_t statistics;

void access_control_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
    statistics.user_add_count = 0;
    statistics.user_remove_count = 0;
    statistics.permission_grant_count = 0;
    statistics.permission_revoke_count = 0;
    statistics.permission_denied_count = 0;
    statistics.access_allow_count = 0;
    statistics.access_deny_count = 0;
}

access_control_manager_statistics_t*
access_control_manager_get_statistics(void)
{
    return &statistics;
}
