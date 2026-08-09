#ifndef AUTHORIZATION_MANAGER_STATISTICS_H
#define AUTHORIZATION_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;

    uint64_t user_add_count;
    uint64_t user_remove_count;

    uint64_t permission_grant_count;
    uint64_t permission_denied_count;

} authorization_manager_statistics_t;

void authorization_manager_statistics_init(void);

authorization_manager_statistics_t*
authorization_manager_get_statistics(void);

#endif
