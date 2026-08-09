#include <stdio.h>

#include "server_lifecycle_manager.h"
#include "server_lifecycle_manager_statistics.h"


static int failures = 0;


static void check(
    int condition,
    const char* name
)
{
    if (condition)
    {
        printf("%s: PASS\n", name);
    }
    else
    {
        printf("%s: FAIL\n", name);
        failures++;
    }
}


int main(void)
{
    printf("===============================\n");
    printf(" Server Lifecycle Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");


    check(
        server_lifecycle_manager_start() != 0,
        "Start Before Initialize"
    );


    check(
        server_lifecycle_manager_stop() != 0,
        "Stop Before Initialize"
    );


    check(
        server_lifecycle_manager_init() == 0,
        "Initialize"
    );


    check(
        server_lifecycle_manager_get_state() ==
        SERVER_LIFECYCLE_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );


    check(
        server_lifecycle_manager_get_context()->initialized == 1,
        "Initialized Flag"
    );


    check(
        server_lifecycle_manager_start() == 0,
        "Start"
    );


    check(
        server_lifecycle_manager_get_state() ==
        SERVER_LIFECYCLE_MANAGER_RUNNING,
        "State RUNNING"
    );


    check(
        server_lifecycle_manager_get_context()->running == 1,
        "Running Flag"
    );


    check(
        server_lifecycle_manager_start() == 0,
        "Repeated Start"
    );


    check(
        server_lifecycle_manager_stop() == 0,
        "Stop"
    );


    check(
        server_lifecycle_manager_get_state() ==
        SERVER_LIFECYCLE_MANAGER_STOPPED,
        "State STOPPED"
    );


    check(
        server_lifecycle_manager_get_context()->running == 0,
        "Running Flag After Stop"
    );


    check(
        server_lifecycle_manager_stop() == 0,
        "Repeated Stop"
    );


    check(
        server_lifecycle_manager_shutdown() == 0,
        "Shutdown"
    );


    check(
        server_lifecycle_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );


    check(
        server_lifecycle_manager_shutdown() == 0,
        "Repeated Shutdown"
    );


    check(
        server_lifecycle_manager_init() != 0,
        "Initialize After Shutdown"
    );


    check(
        server_lifecycle_manager_start() != 0,
        "Start After Shutdown"
    );


    check(
        server_lifecycle_manager_stop() != 0,
        "Stop After Shutdown"
    );


    check(
        server_lifecycle_manager_get_state() ==
        SERVER_LIFECYCLE_MANAGER_ERROR,
        "Final Error State"
    );


    server_lifecycle_manager_statistics_t* statistics =
        server_lifecycle_manager_get_statistics();


    check(
        statistics->start_count == 1,
        "Start Statistics"
    );


    check(
        statistics->stop_count == 1,
        "Stop Statistics"
    );


    check(
        statistics->error_count == 3,
        "Error Statistics"
    );


    if (failures == 0)
    {
        printf("\nRESULT: ALL EDGE CASES PASSED\n");
        return 0;
    }


    printf(
        "\nRESULT: FAILED (%d failures)\n",
        failures
    );


    return 1;
}
