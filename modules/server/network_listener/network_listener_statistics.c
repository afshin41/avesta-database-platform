#include "network_listener_statistics.h"


static network_listener_statistics_t statistics =
{
    0,
    0,
    0
};


void network_listener_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
}


void network_listener_statistics_record_start(void)
{
    statistics.start_count++;
}


void network_listener_statistics_record_stop(void)
{
    statistics.stop_count++;
}


void network_listener_statistics_record_error(void)
{
    statistics.error_count++;
}


network_listener_statistics_t*
network_listener_get_statistics(void)
{
    return &statistics;
}
