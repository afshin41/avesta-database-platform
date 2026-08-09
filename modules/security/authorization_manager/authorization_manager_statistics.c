#include "authorization_manager_statistics.h"

static authorization_manager_statistics_t statistics;

void authorization_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;

    statistics.user_add_count = 0;
    statistics.user_remove_count = 0;

    statistics.permission_grant_count = 0;
    statistics.permission_denied_count = 0;
}

authorization_manager_statistics_t*
authorization_manager_get_statistics(void)
{
    return &statistics;
}
