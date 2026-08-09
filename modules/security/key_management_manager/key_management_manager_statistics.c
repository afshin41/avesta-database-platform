#include "key_management_manager_statistics.h"

static key_management_manager_statistics_t statistics;

void key_management_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
    statistics.key_create_count = 0;
    statistics.key_destroy_count = 0;
    statistics.key_get_count = 0;
    statistics.key_get_failure_count = 0;
}

key_management_manager_statistics_t *
key_management_manager_get_statistics(void)
{
    return &statistics;
}
