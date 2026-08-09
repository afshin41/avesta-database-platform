#ifndef AUTHENTICATION_MANAGER_STATISTICS_H
#define AUTHENTICATION_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t authentication_success_count;
    uint64_t authentication_failure_count;
    uint64_t logout_count;
    uint64_t error_count;
} authentication_manager_statistics_t;

void authentication_manager_statistics_init(void);

authentication_manager_statistics_t*
authentication_manager_get_statistics(void);

#endif
