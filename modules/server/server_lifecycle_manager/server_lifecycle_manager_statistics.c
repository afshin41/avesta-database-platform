#include "server_lifecycle_manager_statistics.h"


static server_lifecycle_manager_statistics_t statistics;


void server_lifecycle_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
}


server_lifecycle_manager_statistics_t*
server_lifecycle_manager_get_statistics(void)
{
    return &statistics;
}
