#include "authentication_manager_statistics.h"

static authentication_manager_statistics_t statistics;

void authentication_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.authentication_success_count = 0;
    statistics.authentication_failure_count = 0;
    statistics.logout_count = 0;
    statistics.error_count = 0;
}

authentication_manager_statistics_t*
authentication_manager_get_statistics(void)
{
    return &statistics;
}
