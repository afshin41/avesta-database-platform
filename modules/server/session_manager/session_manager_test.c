#include <stdio.h>

#include "session_manager.h"
#include "session_manager_statistics.h"

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
    uint32_t id1 = 0;
    uint32_t id2 = 0;

    session_manager_statistics_t* statistics;

    printf("===============================\n");
    printf(" Session Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        session_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        session_manager_open(&id1) != 0,
        "Open Before Initialize"
    );

    check(
        session_manager_init() == 0,
        "Initialize"
    );

    check(
        session_manager_get_state() ==
        SESSION_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        session_manager_set_max_sessions(2) == 0,
        "Configure Maximum Sessions"
    );

    check(
        session_manager_start() == 0,
        "Start"
    );

    check(
        session_manager_get_state() ==
        SESSION_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        session_manager_start() == 0,
        "Repeated Start"
    );

    check(
        session_manager_open(&id1) == 0,
        "Open Session 1"
    );

    check(
        session_manager_open(&id2) == 0,
        "Open Session 2"
    );

    check(
        session_manager_get_context()->active_sessions == 2,
        "Active Session Count"
    );

    check(
        session_manager_open(&id1) != 0,
        "Reject Session Above Limit"
    );

    check(
        session_manager_close(id1) == 0,
        "Close Session 1"
    );

    check(
        session_manager_close(id1) != 0,
        "Reject Repeated Close"
    );

    check(
        session_manager_open(&id1) == 0,
        "Reuse Session Slot"
    );

    check(
        session_manager_stop() == 0,
        "Stop"
    );

    check(
        session_manager_get_state() ==
        SESSION_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        session_manager_open(&id1) != 0,
        "Reject Open After Stop"
    );

    check(
        session_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        session_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        session_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        session_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        session_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        session_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        session_manager_close(id2) != 0,
        "Close After Shutdown"
    );

    check(
        session_manager_get_state() ==
        SESSION_MANAGER_ERROR,
        "Final Error State"
    );

    statistics = session_manager_get_statistics();

    printf(
        "Actual Error Count: %llu\n",
        (unsigned long long)statistics->error_count
    );

    check(
        statistics->start_count == 1,
        "Start Statistics"
    );

    check(
        statistics->stop_count == 1,
        "Stop Statistics"
    );

    check(
        statistics->session_open_count == 3,
        "Session Open Statistics"
    );

    check(
        statistics->session_close_count == 1,
        "Session Close Statistics"
    );

    check(
        statistics->error_count == 6,
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
