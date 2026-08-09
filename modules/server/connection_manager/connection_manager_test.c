#include <stdio.h>

#include "connection_manager.h"
#include "connection_manager_statistics.h"

static int failures = 0;

static void check(int condition, const char* name)
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
    uint32_t id1;
    uint32_t id2;
    connection_manager_statistics_t* statistics;

    printf("===============================\n");
    printf(" Connection Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        connection_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        connection_manager_open(&id1) != 0,
        "Open Before Initialize"
    );

    check(
        connection_manager_init() == 0,
        "Initialize"
    );

    check(
        connection_manager_get_state() ==
        CONNECTION_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        connection_manager_set_max_connections(2) == 0,
        "Configure Maximum Connections"
    );

    check(
        connection_manager_start() == 0,
        "Start"
    );

    check(
        connection_manager_get_state() ==
        CONNECTION_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        connection_manager_start() == 0,
        "Repeated Start"
    );

    check(
        connection_manager_open(&id1) == 0,
        "Open Connection 1"
    );

    check(
        connection_manager_open(&id2) == 0,
        "Open Connection 2"
    );

    check(
        connection_manager_get_context()->active_connections == 2,
        "Active Connection Count"
    );

    check(
        connection_manager_open(&id1) != 0,
        "Reject Connection Above Limit"
    );

    check(
        connection_manager_close(id1) == 0,
        "Close Connection 1"
    );

    check(
        connection_manager_close(id1) != 0,
        "Reject Repeated Close"
    );

    check(
        connection_manager_open(&id1) == 0,
        "Reuse Connection Slot"
    );

    check(
        connection_manager_stop() == 0,
        "Stop"
    );

    check(
        connection_manager_get_state() ==
        CONNECTION_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        connection_manager_open(&id1) != 0,
        "Reject Open After Stop"
    );

    check(
        connection_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        connection_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        connection_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        connection_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        connection_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        connection_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        connection_manager_close(id2) != 0,
        "Close After Shutdown"
    );

    check(
        connection_manager_get_state() ==
        CONNECTION_MANAGER_ERROR,
        "Final Error State"
    );

    statistics = connection_manager_get_statistics();

    check(
        statistics->start_count == 1,
        "Start Statistics"
    );

    check(
        statistics->stop_count == 1,
        "Stop Statistics"
    );

    check(
        statistics->connection_open_count == 3,
        "Connection Open Statistics"
    );

    check(
        statistics->connection_close_count == 1,
        "Connection Close Statistics"
    );

    check(
        statistics->error_count == 8,
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
