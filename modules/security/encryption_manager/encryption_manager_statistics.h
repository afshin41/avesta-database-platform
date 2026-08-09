#ifndef ENCRYPTION_MANAGER_STATISTICS_H
#define ENCRYPTION_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;
    uint64_t key_create_count;
    uint64_t key_destroy_count;
    uint64_t encrypt_count;
    uint64_t decrypt_count;
} encryption_manager_statistics_t;

void encryption_manager_statistics_init(void);

encryption_manager_statistics_t *
encryption_manager_get_statistics(void);

#endif
