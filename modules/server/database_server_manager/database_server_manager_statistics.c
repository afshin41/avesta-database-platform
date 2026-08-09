#include "database_server_manager_statistics.h"

static database_server_manager_statistics_t statistics;


void database_server_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
}


database_server_manager_statistics_t*
database_server_manager_get_statistics(void)
{
    return &statistics;
}
