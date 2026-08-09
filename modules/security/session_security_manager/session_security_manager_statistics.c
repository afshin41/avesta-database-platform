#include "session_security_manager_statistics.h"

static session_security_manager_statistics_t statistics;

void session_security_manager_statistics_init(void)
{
    statistics.start_count = 0;
    statistics.stop_count = 0;
    statistics.error_count = 0;

    statistics.session_create_count = 0;
    statistics.session_destroy_count = 0;

    statistics.validation_success_count = 0;
    statistics.validation_failure_count = 0;

    statistics.expiration_count = 0;
}

session_security_manager_statistics_t*
session_security_manager_get_statistics(void)
{
    return &statistics;
}
