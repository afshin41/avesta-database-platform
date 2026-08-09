#ifndef SESSION_MANAGER_STATISTICS_H
#define SESSION_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;
    uint64_t session_open_count;
    uint64_t session_close_count;
} session_manager_statistics_t;

void session_manager_statistics_init(void);

session_manager_statistics_t*
session_manager_get_statistics(void);

#endif
