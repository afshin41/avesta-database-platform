#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "key_management_manager.h"
#include "key_management_manager_statistics.h"

static int failures = 0;

static void check(const char *name, int condition)
{
    if (condition)
        printf("%-42s: PASS\n", name);
    else
    {
        printf("%-42s: FAIL\n", name);
        failures++;
    }
}

int main(void)
{
    uint8_t key1[16] = {
        1,2,3,4,5,6,7,8,
        9,10,11,12,13,14,15,16
    };

    uint8_t key2[16] = {
        16,15,14,13,12,11,10,9,
        8,7,6,5,4,3,2,1
    };

    uint8_t output[32];
    uint32_t key_id1 = 0;
    uint32_t key_id2 = 0;
    uint32_t key_id3 = 0;
    uint32_t output_length = 0;

    key_management_manager_statistics_t *statistics;

    printf("===============================\n");
    printf(" Key Management Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        "Start Before Initialize",
        key_management_manager_start() != 0
    );

    check(
        "Create Key Before Initialize",
        key_management_manager_create_key(
            key1, sizeof(key1), &key_id1
        ) != 0
    );

    check(
        "Get Key Before Initialize",
        key_management_manager_get_key(
            1, output, sizeof(output), &output_length
        ) != 0
    );

    check(
        "Initialize",
        key_management_manager_init() == 0
    );

    check(
        "State INITIALIZED",
        key_management_manager_get_state() ==
        KEY_MANAGEMENT_MANAGER_INITIALIZED
    );

    check(
        "Configure Maximum Keys",
        key_management_manager_set_max_keys(2) == 0
    );

    check(
        "Start",
        key_management_manager_start() == 0
    );

    check(
        "State RUNNING",
        key_management_manager_get_state() ==
        KEY_MANAGEMENT_MANAGER_RUNNING
    );

    check(
        "Repeated Start",
        key_management_manager_start() != 0
    );

    check(
        "Create Key 1",
        key_management_manager_create_key(
            key1, sizeof(key1), &key_id1
        ) == 0
    );

    check(
        "Create Key 2",
        key_management_manager_create_key(
            key2, sizeof(key2), &key_id2
        ) == 0
    );

    check(
        "Active Key Count",
        key_management_manager_get_context()->active_keys == 2
    );

    check(
        "Reject Key Above Limit",
        key_management_manager_create_key(
            key1, sizeof(key1), &key_id3
        ) != 0
    );

    memset(output, 0, sizeof(output));
    output_length = 0;

    check(
        "Get Key 1",
        key_management_manager_get_key(
            key_id1,
            output,
            sizeof(output),
            &output_length
        ) == 0
    );

    check(
        "Validate Key 1",
        output_length == sizeof(key1) &&
        memcmp(output, key1, sizeof(key1)) == 0
    );

    check(
        "Reject Invalid Key",
        key_management_manager_get_key(
            999999,
            output,
            sizeof(output),
            &output_length
        ) != 0
    );

    check(
        "Reject Small Output Buffer",
        key_management_manager_get_key(
            key_id2,
            output,
            4,
            &output_length
        ) != 0
    );

    check(
        "Destroy Key 1",
        key_management_manager_destroy_key(key_id1) == 0
    );

    check(
        "Reject Repeated Destroy",
        key_management_manager_destroy_key(key_id1) != 0
    );

    check(
        "Reuse Key Slot",
        key_management_manager_create_key(
            key1, sizeof(key1), &key_id3
        ) == 0
    );

    check(
        "Active Key Count After Reuse",
        key_management_manager_get_context()->active_keys == 2
    );

    check(
        "Get Reused Key",
        key_management_manager_get_key(
            key_id3,
            output,
            sizeof(output),
            &output_length
        ) == 0
    );

    check(
        "Validate Reused Key",
        output_length == sizeof(key1) &&
        memcmp(output, key1, sizeof(key1)) == 0
    );

    check(
        "Stop",
        key_management_manager_stop() == 0
    );

    check(
        "State STOPPED",
        key_management_manager_get_state() ==
        KEY_MANAGEMENT_MANAGER_STOPPED
    );

    check(
        "Reject Create After Stop",
        key_management_manager_create_key(
            key1, sizeof(key1), &key_id3
        ) != 0
    );

    check(
        "Reject Get After Stop",
        key_management_manager_get_key(
            key_id2,
            output,
            sizeof(output),
            &output_length
        ) != 0
    );

    check(
        "Reject Destroy After Stop",
        key_management_manager_destroy_key(key_id2) != 0
    );

    check(
        "Repeated Stop",
        key_management_manager_stop() != 0
    );

    check(
        "Shutdown",
        key_management_manager_shutdown() == 0
    );

    check(
        "Shutdown Flag",
        key_management_manager_get_context()->shutdown == 1
    );

    check(
        "Repeated Shutdown",
        key_management_manager_shutdown() != 0
    );

    check(
        "Initialize After Shutdown",
        key_management_manager_init() != 0
    );

    check(
        "Start After Shutdown",
        key_management_manager_start() != 0
    );

    check(
        "Create After Shutdown",
        key_management_manager_create_key(
            key1, sizeof(key1), &key_id3
        ) != 0
    );

    check(
        "Get After Shutdown",
        key_management_manager_get_key(
            key_id2,
            output,
            sizeof(output),
            &output_length
        ) != 0
    );

    check(
        "Destroy After Shutdown",
        key_management_manager_destroy_key(key_id2) != 0
    );

    check(
        "Final Error State",
        key_management_manager_get_state() ==
        KEY_MANAGEMENT_MANAGER_ERROR
    );

    statistics = key_management_manager_get_statistics();

    printf(
        "Actual Error Count: %llu\n",
        (unsigned long long)statistics->error_count
    );

    check(
        "Start Statistics",
        statistics->start_count == 1
    );

    check(
        "Stop Statistics",
        statistics->stop_count == 1
    );

    check(
        "Key Create Statistics",
        statistics->key_create_count == 3
    );

    check(
        "Key Destroy Statistics",
        statistics->key_destroy_count == 1
    );

    check(
        "Key Get Statistics",
        statistics->key_get_count == 2
    );

    check(
        "Key Get Failure Statistics",
        statistics->key_get_failure_count == 4
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
