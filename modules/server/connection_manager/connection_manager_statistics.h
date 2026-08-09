#ifndef CONNECTION_MANAGER_STATISTICS_H
#define CONNECTION_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t connection_open_count;
    uint64_t connection_close_count;
    uint64_t error_count;

} connection_manager_statistics_t;

void connection_manager_statistics_init(void);

connection_manager_statistics_t*
connection_manager_get_statistics(void);

#endif
