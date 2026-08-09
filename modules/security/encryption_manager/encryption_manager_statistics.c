#include "encryption_manager_statistics.h"

static encryption_manager_statistics_t statistics;

void encryption_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;
    statistics.key_create_count = 0;
    statistics.key_destroy_count = 0;
    statistics.encrypt_count = 0;
    statistics.decrypt_count = 0;
}

encryption_manager_statistics_t *
encryption_manager_get_statistics(void)
{
    return &statistics;
}
