#include "session_manager_statistics.h"

static session_manager_statistics_t statistics;

void session_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
    statistics.session_open_count = 0;
    statistics.session_close_count = 0;
}

session_manager_statistics_t*
session_manager_get_statistics(void)
{
    return &statistics;
}
