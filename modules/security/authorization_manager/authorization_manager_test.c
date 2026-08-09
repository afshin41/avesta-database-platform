#include <stdio.h>

#include "authorization_manager.h"
#include "authorization_manager_statistics.h"

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
    uint32_t user1;
    uint32_t user2;

    authorization_manager_statistics_t*
        statistics;

    printf("===============================\n");
    printf(" Authorization Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        authorization_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        authorization_manager_add_user(
            1,
            &user1
        ) != 0,
        "Add User Before Initialize"
    );

    check(
        authorization_manager_init() == 0,
        "Initialize"
    );

    check(
        authorization_manager_get_state() ==
        AUTHORIZATION_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        authorization_manager_set_max_users(2) == 0,
        "Configure Maximum Users"
    );

    check(
        authorization_manager_start() == 0,
        "Start"
    );

    check(
        authorization_manager_get_state() ==
        AUTHORIZATION_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        authorization_manager_start() == 0,
        "Repeated Start"
    );

    check(
        authorization_manager_add_user(
            0x03,
            &user1
        ) == 0,
        "Add User 1"
    );

    check(
        authorization_manager_add_user(
            0x01,
            &user2
        ) == 0,
        "Add User 2"
    );

    check(
        authorization_manager_get_context()->
            active_users == 2,
        "Active User Count"
    );

    check(
        authorization_manager_add_user(
            0x01,
            &user1
        ) != 0,
        "Reject User Above Limit"
    );

    check(
        authorization_manager_check_permission(
            user1,
            0x01
        ) == 0,
        "Grant Permission"
    );

    check(
        authorization_manager_check_permission(
            user2,
            0x02
        ) != 0,
        "Reject Permission"
    );

    check(
        authorization_manager_remove_user(user1) == 0,
        "Remove User 1"
    );

    check(
        authorization_manager_remove_user(user1) != 0,
        "Reject Repeated Remove"
    );

    check(
        authorization_manager_add_user(
            0x04,
            &user1
        ) == 0,
        "Reuse User Slot"
    );

    check(
        authorization_manager_stop() == 0,
        "Stop"
    );

    check(
        authorization_manager_get_state() ==
        AUTHORIZATION_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        authorization_manager_add_user(
            0x01,
            &user2
        ) != 0,
        "Reject Add After Stop"
    );

    check(
        authorization_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        authorization_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        authorization_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        authorization_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        authorization_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        authorization_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        authorization_manager_remove_user(user2) != 0,
        "Remove After Shutdown"
    );

    check(
        authorization_manager_get_state() ==
        AUTHORIZATION_MANAGER_ERROR,
        "Final Error State"
    );

    statistics =
        authorization_manager_get_statistics();

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
        statistics->user_add_count == 3,
        "User Add Statistics"
    );

    check(
        statistics->user_remove_count == 1,
        "User Remove Statistics"
    );

    check(
        statistics->permission_grant_count == 1,
        "Permission Grant Statistics"
    );

    check(
        statistics->permission_denied_count == 1,
        "Permission Denied Statistics"
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
