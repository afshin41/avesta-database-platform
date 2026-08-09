#include "audit_manager_statistics.h"


static audit_manager_statistics_t statistics;


void audit_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
}


audit_manager_statistics_t*
audit_manager_get_statistics(void)
{
    return &statistics;
}
