#include "connection_manager_statistics.h"

static connection_manager_statistics_t statistics;

void connection_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.connection_open_count = 0;
    statistics.connection_close_count = 0;
    statistics.error_count = 0;
}

connection_manager_statistics_t*
connection_manager_get_statistics(void)
{
    return &statistics;
}
