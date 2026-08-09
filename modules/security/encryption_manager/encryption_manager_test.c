#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "encryption_manager.h"
#include "encryption_manager_statistics.h"

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
    static const uint8_t key[32] = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,
        0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
        0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20
    };

    static const uint8_t plaintext[] = "Avesta Encryption Test";

    uint8_t encrypted[128];
    uint8_t decrypted[128];

    uint32_t key1;
    uint32_t key2;
    uint32_t output_length;
    encryption_manager_statistics_t *statistics;

    printf("===============================\n");
    printf(" Encryption Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        encryption_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        encryption_manager_create_key(key, sizeof(key), &key1) != 0,
        "Create Key Before Initialize"
    );

    check(
        encryption_manager_init() == 0,
        "Initialize"
    );

    check(
        encryption_manager_get_state() ==
        ENCRYPTION_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        encryption_manager_set_max_keys(2) == 0,
        "Configure Maximum Keys"
    );

    check(
        encryption_manager_start() == 0,
        "Start"
    );

    check(
        encryption_manager_get_state() ==
        ENCRYPTION_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        encryption_manager_start() == 0,
        "Repeated Start"
    );

    check(
        encryption_manager_create_key(
            key,
            sizeof(key),
            &key1
        ) == 0,
        "Create Key 1"
    );

    check(
        encryption_manager_create_key(
            key,
            sizeof(key),
            &key2
        ) == 0,
        "Create Key 2"
    );

    check(
        encryption_manager_get_context()->active_keys == 2,
        "Active Key Count"
    );

    check(
        encryption_manager_create_key(
            key,
            sizeof(key),
            &key2
        ) != 0,
        "Reject Key Above Limit"
    );

    check(
        encryption_manager_encrypt(
            key1,
            plaintext,
            (uint32_t)(sizeof(plaintext) - 1),
            encrypted,
            sizeof(encrypted),
            &output_length
        ) == 0,
        "Encrypt Data"
    );

    check(
        output_length == sizeof(plaintext) - 1 &&
        memcmp(
            encrypted,
            plaintext,
            sizeof(plaintext) - 1
        ) != 0,
        "Encrypted Data Changed"
    );

    check(
        encryption_manager_decrypt(
            key1,
            encrypted,
            output_length,
            decrypted,
            sizeof(decrypted),
            &output_length
        ) == 0,
        "Decrypt Data"
    );

    check(
        output_length == sizeof(plaintext) - 1 &&
        memcmp(
            decrypted,
            plaintext,
            sizeof(plaintext) - 1
        ) == 0,
        "Decrypted Data Matches"
    );

    check(
        encryption_manager_encrypt(
            999999,
            plaintext,
            (uint32_t)(sizeof(plaintext) - 1),
            encrypted,
            sizeof(encrypted),
            &output_length
        ) != 0,
        "Reject Invalid Key"
    );

    check(
        encryption_manager_destroy_key(key1) == 0,
        "Destroy Key 1"
    );

    check(
        encryption_manager_destroy_key(key1) != 0,
        "Reject Repeated Destroy"
    );

    check(
        encryption_manager_create_key(
            key,
            sizeof(key),
            &key1
        ) == 0,
        "Reuse Key Slot"
    );

    check(
        encryption_manager_stop() == 0,
        "Stop"
    );

    check(
        encryption_manager_get_state() ==
        ENCRYPTION_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        encryption_manager_encrypt(
            key1,
            plaintext,
            (uint32_t)(sizeof(plaintext) - 1),
            encrypted,
            sizeof(encrypted),
            &output_length
        ) != 0,
        "Reject Encrypt After Stop"
    );

    check(
        encryption_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        encryption_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        encryption_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        encryption_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        encryption_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        encryption_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        encryption_manager_destroy_key(key2) != 0,
        "Destroy After Shutdown"
    );

    check(
        encryption_manager_get_state() ==
        ENCRYPTION_MANAGER_ERROR,
        "Final Error State"
    );

    statistics = encryption_manager_get_statistics();

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
        statistics->key_create_count == 3,
        "Key Create Statistics"
    );

    check(
        statistics->key_destroy_count == 1,
        "Key Destroy Statistics"
    );

    check(
        statistics->encrypt_count == 1,
        "Encrypt Statistics"
    );

    check(
        statistics->decrypt_count == 1,
        "Decrypt Statistics"
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
