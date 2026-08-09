#ifndef SERVER_LIFECYCLE_MANAGER_STATISTICS_H
#define SERVER_LIFECYCLE_MANAGER_STATISTICS_H

#include <stdint.h>


typedef struct
{
    uint64_t start_count;
    uint64_t stop_count;
    uint64_t error_count;

} server_lifecycle_manager_statistics_t;


void server_lifecycle_manager_statistics_init(void);

server_lifecycle_manager_statistics_t*
server_lifecycle_manager_get_statistics(void);


#endif
