#include <stdio.h>

#include "database_server_manager.h"
#include "database_server_manager_statistics.h"


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
    printf(" Database Server Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");


    /*
     * Invalid operation before initialization
     */

    check(
        database_server_manager_start() != 0,
        "Start Before Initialize"
    );


    check(
        database_server_manager_stop() != 0,
        "Stop Before Initialize"
    );


    /*
     * Initialize
     */

    check(
        database_server_manager_init() == 0,
        "Initialize"
    );


    check(
        database_server_manager_get_state() ==
        DATABASE_SERVER_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );


    /*
     * Repeated initialization
     */

    check(
        database_server_manager_init() == 0,
        "Repeated Initialize"
    );


    /*
     * Start
     */

    check(
        database_server_manager_start() == 0,
        "Start"
    );


    check(
        database_server_manager_get_state() ==
        DATABASE_SERVER_MANAGER_RUNNING,
        "State RUNNING"
    );


    /*
     * Repeated start
     */

    check(
        database_server_manager_start() == 0,
        "Repeated Start"
    );


    /*
     * Stop
     */

    check(
        database_server_manager_stop() == 0,
        "Stop"
    );


    check(
        database_server_manager_get_state() ==
        DATABASE_SERVER_MANAGER_STOPPED,
        "State STOPPED"
    );


    /*
     * Repeated stop
     */

    check(
        database_server_manager_stop() == 0,
        "Repeated Stop"
    );


    /*
     * Shutdown
     */

    check(
        database_server_manager_shutdown() == 0,
        "Shutdown"
    );


    check(
        database_server_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );


    /*
     * Repeated shutdown
     */

    check(
        database_server_manager_shutdown() == 0,
        "Repeated Shutdown"
    );


    /*
     * Operations after shutdown
     */

    check(
        database_server_manager_init() != 0,
        "Initialize After Shutdown"
    );


    check(
        database_server_manager_start() != 0,
        "Start After Shutdown"
    );


    check(
        database_server_manager_stop() != 0,
        "Stop After Shutdown"
    );


    /*
     * Final state
     */

    check(
        database_server_manager_get_state() ==
        DATABASE_SERVER_MANAGER_ERROR,
        "Final Error State"
    );


    /*
     * Statistics
     */

    database_server_manager_statistics_t* statistics =
        database_server_manager_get_statistics();


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
