#include <stdio.h>

#include "authentication_manager.h"
#include "authentication_manager_statistics.h"

static int failures = 0;

static void check(int condition, const char* name)
{
    if (condition)
        printf("%s: PASS\n", name);
    else
    {
        printf("%s: FAIL\n", name);
        failures++;
    }
}

int main(void)
{
    authentication_manager_statistics_t* statistics;

    printf("===============================\n");
    printf(" Authentication Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        authentication_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        authentication_manager_authenticate(1, "admin", "password", 1) != 0,
        "Authenticate Before Initialize"
    );

    check(
        authentication_manager_init() == 0,
        "Initialize"
    );

    check(
        authentication_manager_get_state() ==
        AUTHENTICATION_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        authentication_manager_set_credentials(
            1, "admin", "password"
        ) == 0,
        "Configure Credentials"
    );

    check(
        authentication_manager_start() == 0,
        "Start"
    );

    check(
        authentication_manager_get_state() ==
        AUTHENTICATION_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        authentication_manager_start() == 0,
        "Repeated Start"
    );

    check(
        authentication_manager_authenticate(
            1, "wrong", "wrong", 100
        ) == AUTHENTICATION_RESULT_INVALID_CREDENTIALS,
        "Reject Invalid Credentials"
    );

    check(
        authentication_manager_is_authenticated() == 0,
        "Remain Unauthenticated"
    );

    check(
        authentication_manager_authenticate(
            1, "admin", "password", 100
        ) == AUTHENTICATION_RESULT_SUCCESS,
        "Authenticate Valid Credentials"
    );

    check(
        authentication_manager_is_authenticated() == 1,
        "Authenticated Flag"
    );

    check(
        authentication_manager_authenticate(
            1, "admin", "password", 101
        ) != 0,
        "Reject Repeated Authentication"
    );

    check(
        authentication_manager_logout() == 0,
        "Logout"
    );

    check(
        authentication_manager_is_authenticated() == 0,
        "Logout Flag"
    );

    check(
        authentication_manager_logout() != 0,
        "Reject Repeated Logout"
    );

    check(
        authentication_manager_stop() == 0,
        "Stop"
    );

    check(
        authentication_manager_get_state() ==
        AUTHENTICATION_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        authentication_manager_authenticate(
            1, "admin", "password", 200
        ) != 0,
        "Reject Authentication After Stop"
    );

    check(
        authentication_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        authentication_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        authentication_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        authentication_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        authentication_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        authentication_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        authentication_manager_logout() != 0,
        "Logout After Shutdown"
    );

    check(
        authentication_manager_get_state() ==
        AUTHENTICATION_MANAGER_ERROR,
        "Final Error State"
    );

    statistics = authentication_manager_get_statistics();

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
        statistics->authentication_success_count == 1,
        "Authentication Success Statistics"
    );

    check(
        statistics->authentication_failure_count == 1,
        "Authentication Failure Statistics"
    );

    check(
        statistics->logout_count == 1,
        "Logout Statistics"
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
