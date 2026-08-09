#ifndef KEY_MANAGEMENT_MANAGER_STATISTICS_H
#define KEY_MANAGEMENT_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;
    uint64_t key_create_count;
    uint64_t key_destroy_count;
    uint64_t key_get_count;
    uint64_t key_get_failure_count;
} key_management_manager_statistics_t;

void key_management_manager_statistics_init(void);

key_management_manager_statistics_t *
key_management_manager_get_statistics(void);

#endif
