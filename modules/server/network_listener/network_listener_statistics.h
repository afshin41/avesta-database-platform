#ifndef NETWORK_LISTENER_STATISTICS_H
#define NETWORK_LISTENER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;

} network_listener_statistics_t;


void network_listener_statistics_init(void);

void network_listener_statistics_record_start(void);

void network_listener_statistics_record_stop(void);

void network_listener_statistics_record_error(void);

network_listener_statistics_t*
network_listener_get_statistics(void);


#endif
