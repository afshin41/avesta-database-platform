#include <stdio.h>

#include "session_security_manager.h"
#include "session_security_manager_statistics.h"

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
    uint32_t session1;
    uint32_t session2;

    uint64_t token1;
    uint64_t token2;

    session_security_manager_statistics_t*
        statistics;

    printf("===============================\n");
    printf(" Session Security Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        session_security_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        session_security_manager_create(
            1,
            100,
            &session1,
            &token1
        ) != 0,
        "Create Before Initialize"
    );

    check(
        session_security_manager_init() == 0,
        "Initialize"
    );

    check(
        session_security_manager_get_state() ==
        SESSION_SECURITY_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        session_security_manager_set_max_sessions(2) == 0,
        "Configure Maximum Sessions"
    );

    check(
        session_security_manager_start() == 0,
        "Start"
    );

    check(
        session_security_manager_get_state() ==
        SESSION_SECURITY_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        session_security_manager_start() == 0,
        "Repeated Start"
    );

    check(
        session_security_manager_create(
            100,
            100,
            &session1,
            &token1
        ) == 0,
        "Create Session 1"
    );

    check(
        session_security_manager_validate(
            session1,
            token1
        ) == 0,
        "Validate Session 1"
    );

    check(
        session_security_manager_validate(
            session1,
            token1 + 1
        ) != 0,
        "Reject Invalid Token"
    );

    check(
        session_security_manager_create(
            200,
            200,
            &session2,
            &token2
        ) == 0,
        "Create Session 2"
    );

    check(
        session_security_manager_get_context()->
            active_sessions == 2,
        "Active Session Count"
    );

    check(
        session_security_manager_create(
            300,
            300,
            &session1,
            &token1
        ) != 0,
        "Reject Session Above Limit"
    );

    check(
        session_security_manager_expire(session2) == 0,
        "Expire Session 2"
    );

    check(
        session_security_manager_validate(
            session2,
            token2
        ) != 0,
        "Reject Expired Session"
    );

    check(
        session_security_manager_destroy(session1) == 0,
        "Destroy Session 1"
    );

    check(
        session_security_manager_destroy(session1) != 0,
        "Reject Repeated Destroy"
    );

    check(
        session_security_manager_create(
            400,
            400,
            &session1,
            &token1
        ) == 0,
        "Reuse Session Slot"
    );

    check(
        session_security_manager_stop() == 0,
        "Stop"
    );

    check(
        session_security_manager_get_state() ==
        SESSION_SECURITY_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        session_security_manager_create(
            500,
            500,
            &session2,
            &token2
        ) != 0,
        "Reject Create After Stop"
    );

    check(
        session_security_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        session_security_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        session_security_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        session_security_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        session_security_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        session_security_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        session_security_manager_destroy(session1) != 0,
        "Destroy After Shutdown"
    );

    check(
        session_security_manager_get_state() ==
        SESSION_SECURITY_MANAGER_ERROR,
        "Final Error State"
    );

    statistics =
        session_security_manager_get_statistics();

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
        statistics->session_create_count == 3,
        "Session Create Statistics"
    );

    check(
        statistics->session_destroy_count == 1,
        "Session Destroy Statistics"
    );

    check(
        statistics->validation_success_count == 1,
        "Validation Success Statistics"
    );

    check(
        statistics->validation_failure_count == 2,
        "Validation Failure Statistics"
    );

    check(
        statistics->expiration_count == 1,
        "Expiration Statistics"
    );

    check(
        statistics->error_count == 6,
        "Error Statistics"
    );

    if (failures == 0)
    {
        printf(
            "\nRESULT: ALL EDGE CASES PASSED\n"
        );

        return 0;
    }

    printf(
        "\nRESULT: FAILED (%d failures)\n",
        failures
    );

    return 1;
}
