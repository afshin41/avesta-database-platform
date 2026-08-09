#include <stdio.h>

#include "access_control_manager.h"
#include "access_control_manager_statistics.h"

static int failures = 0;

static void check(int condition, const char *name)
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
    access_control_manager_statistics_t *statistics;

    const uint32_t user1 = 1001U;
    const uint32_t user2 = 1002U;
    const uint32_t user3 = 1003U;
    const uint32_t permission_read = 0x01U;
    const uint32_t permission_write = 0x02U;

    printf("===============================\n");
    printf(" Access Control Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        access_control_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        access_control_manager_add_user(user1) != 0,
        "Add User Before Initialize"
    );

    check(
        access_control_manager_init() == 0,
        "Initialize"
    );

    check(
        access_control_manager_get_state() ==
        ACCESS_CONTROL_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        access_control_manager_set_max_users(2) == 0,
        "Configure Maximum Users"
    );

    check(
        access_control_manager_start() == 0,
        "Start"
    );

    check(
        access_control_manager_get_state() ==
        ACCESS_CONTROL_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        access_control_manager_start() == 0,
        "Repeated Start"
    );

    check(
        access_control_manager_add_user(user1) == 0,
        "Add User 1"
    );

    check(
        access_control_manager_add_user(user2) == 0,
        "Add User 2"
    );

    check(
        access_control_manager_get_context()->active_users == 2,
        "Active User Count"
    );

    check(
        access_control_manager_add_user(user3) != 0,
        "Reject User Above Limit"
    );

    check(
        access_control_manager_grant_permission(
            user1,
            permission_read | permission_write
        ) == 0,
        "Grant Permissions"
    );

    check(
        access_control_manager_check_access(
            user1,
            permission_read
        ) == ACCESS_CONTROL_ALLOWED,
        "Allow Valid Permission"
    );

    check(
        access_control_manager_check_access(
            user1,
            0x04U
        ) == ACCESS_CONTROL_DENIED,
        "Deny Missing Permission"
    );

    check(
        access_control_manager_revoke_permission(
            user1,
            permission_write
        ) == 0,
        "Revoke Permission"
    );

    check(
        access_control_manager_check_access(
            user1,
            permission_write
        ) == ACCESS_CONTROL_DENIED,
        "Deny Revoked Permission"
    );

    check(
        access_control_manager_remove_user(user1) == 0,
        "Remove User 1"
    );

    check(
        access_control_manager_remove_user(user1) != 0,
        "Reject Repeated Remove"
    );

    check(
        access_control_manager_add_user(user3) == 0,
        "Reuse User Slot"
    );

    check(
        access_control_manager_stop() == 0,
        "Stop"
    );

    check(
        access_control_manager_get_state() ==
        ACCESS_CONTROL_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        access_control_manager_add_user(2000U) != 0,
        "Reject Add After Stop"
    );

    check(
        access_control_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        access_control_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        access_control_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        access_control_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        access_control_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        access_control_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        access_control_manager_remove_user(user2) != 0,
        "Remove After Shutdown"
    );

    check(
        access_control_manager_get_state() ==
        ACCESS_CONTROL_MANAGER_ERROR,
        "Final Error State"
    );

    statistics = access_control_manager_get_statistics();

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
        statistics->permission_revoke_count == 1,
        "Permission Revoke Statistics"
    );

    check(
        statistics->access_allow_count == 1,
        "Access Allow Statistics"
    );

    check(
        statistics->access_deny_count == 2,
        "Access Deny Statistics"
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
