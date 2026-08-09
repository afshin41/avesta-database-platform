#ifndef SESSION_SECURITY_MANAGER_STATISTICS_H
#define SESSION_SECURITY_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;

    uint64_t session_create_count;
    uint64_t session_destroy_count;

    uint64_t validation_success_count;
    uint64_t validation_failure_count;

    uint64_t expiration_count;

} session_security_manager_statistics_t;

void session_security_manager_statistics_init(void);

session_security_manager_statistics_t*
session_security_manager_get_statistics(void);

#endif
