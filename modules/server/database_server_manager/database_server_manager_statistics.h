#ifndef DATABASE_SERVER_MANAGER_STATISTICS_H
#define DATABASE_SERVER_MANAGER_STATISTICS_H

#include <stdint.h>

typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;

} database_server_manager_statistics_t;


void database_server_manager_statistics_init(void);

database_server_manager_statistics_t*
database_server_manager_get_statistics(void);

#endif
