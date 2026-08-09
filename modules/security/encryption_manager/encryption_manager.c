#include "encryption_manager.h"

#include <stdlib.h>
#include <string.h>

#define ENCRYPTION_MANAGER_DEFAULT_MAX_KEYS 64U
#define ENCRYPTION_MANAGER_FIRST_KEY_ID 1U
#define ENCRYPTION_MANAGER_KEY_SIZE 32U

static encryption_manager_context_t context;
static encryption_key_t *keys = NULL;

static int encryption_manager_error(void)
{
    context.state = ENCRYPTION_MANAGER_ERROR;
    encryption_manager_get_statistics()->error_count++;
    return -1;
}

static void encryption_manager_release_keys(void)
{
    free(keys);
    keys = NULL;
}

static encryption_key_t *encryption_manager_find_key(uint32_t key_id)
{
    uint32_t index;

    if (keys == NULL || key_id == 0)
    {
        return NULL;
    }

    for (index = 0; index < context.max_keys; ++index)
    {
        if (keys[index].active &&
            keys[index].key_id == key_id)
        {
            return &keys[index];
        }
    }

    return NULL;
}

int encryption_manager_init(void)
{
    if (context.shutdown)
    {
        return encryption_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    keys = calloc(
        ENCRYPTION_MANAGER_DEFAULT_MAX_KEYS,
        sizeof(encryption_key_t)
    );

    if (keys == NULL)
    {
        return encryption_manager_error();
    }

    context.state = ENCRYPTION_MANAGER_INITIALIZED;
    context.max_keys = ENCRYPTION_MANAGER_DEFAULT_MAX_KEYS;
    context.active_keys = 0;
    context.next_key_id = ENCRYPTION_MANAGER_FIRST_KEY_ID;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    encryption_manager_statistics_init();

    return 0;
}

int encryption_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return encryption_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;
    context.state = ENCRYPTION_MANAGER_RUNNING;

    encryption_manager_get_statistics()->start_count++;

    return 0;
}

int encryption_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return encryption_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;
    context.state = ENCRYPTION_MANAGER_STOPPED;

    encryption_manager_get_statistics()->stop_count++;

    return 0;
}

int encryption_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;
        encryption_manager_get_statistics()->stop_count++;
    }

    encryption_manager_release_keys();

    context.active_keys = 0;
    context.initialized = 0;
    context.shutdown = 1;
    context.state = ENCRYPTION_MANAGER_STOPPED;

    return 0;
}

int encryption_manager_set_max_keys(uint32_t max_keys)
{
    if (!context.initialized || context.shutdown)
    {
        return encryption_manager_error();
    }

    if (context.running)
    {
        return encryption_manager_error();
    }

    if (max_keys == 0 ||
        max_keys > ENCRYPTION_MANAGER_DEFAULT_MAX_KEYS)
    {
        return encryption_manager_error();
    }

    context.max_keys = max_keys;

    return 0;
}

int encryption_manager_create_key(
    const uint8_t *key,
    uint32_t key_length,
    uint32_t *key_id
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        key == NULL ||
        key_id == NULL ||
        key_length != ENCRYPTION_MANAGER_KEY_SIZE)
    {
        return encryption_manager_error();
    }

    if (context.active_keys >= context.max_keys)
    {
        return encryption_manager_error();
    }

    for (index = 0; index < context.max_keys; ++index)
    {
        if (!keys[index].active)
        {
            keys[index].active = 1;
            keys[index].key_id = context.next_key_id++;

            memcpy(
                keys[index].key,
                key,
                ENCRYPTION_MANAGER_KEY_SIZE
            );

            context.active_keys++;
            *key_id = keys[index].key_id;

            encryption_manager_get_statistics()->key_create_count++;

            return 0;
        }
    }

    return encryption_manager_error();
}

int encryption_manager_destroy_key(uint32_t key_id)
{
    encryption_key_t *key;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        key_id == 0)
    {
        return encryption_manager_error();
    }

    key = encryption_manager_find_key(key_id);

    if (key == NULL)
    {
        return encryption_manager_error();
    }

    memset(key->key, 0, sizeof(key->key));

    key->key_id = 0;
    key->active = 0;

    context.active_keys--;

    encryption_manager_get_statistics()->key_destroy_count++;

    return 0;
}

static int encryption_manager_xor(
    uint32_t key_id,
    const uint8_t *input,
    uint32_t input_length,
    uint8_t *output,
    uint32_t output_size,
    uint32_t *output_length,
    int decrypt
)
{
    encryption_key_t *key;
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        input == NULL ||
        output == NULL ||
        output_length == NULL ||
        input_length == 0 ||
        output_size < input_length)
    {
        return encryption_manager_error();
    }

    key = encryption_manager_find_key(key_id);

    if (key == NULL)
    {
        return encryption_manager_error();
    }

    for (index = 0; index < input_length; ++index)
    {
        output[index] =
            input[index] ^
            key->key[index % ENCRYPTION_MANAGER_KEY_SIZE];
    }

    *output_length = input_length;

    if (decrypt)
    {
        encryption_manager_get_statistics()->decrypt_count++;
    }
    else
    {
        encryption_manager_get_statistics()->encrypt_count++;
    }

    return 0;
}

int encryption_manager_encrypt(
    uint32_t key_id,
    const uint8_t *input,
    uint32_t input_length,
    uint8_t *output,
    uint32_t output_size,
    uint32_t *output_length
)
{
    return encryption_manager_xor(
        key_id,
        input,
        input_length,
        output,
        output_size,
        output_length,
        0
    );
}

int encryption_manager_decrypt(
    uint32_t key_id,
    const uint8_t *input,
    uint32_t input_length,
    uint8_t *output,
    uint32_t output_size,
    uint32_t *output_length
)
{
    return encryption_manager_xor(
        key_id,
        input,
        input_length,
        output,
        output_size,
        output_length,
        1
    );
}

encryption_manager_state_t encryption_manager_get_state(void)
{
    return context.state;
}

encryption_manager_context_t *encryption_manager_get_context(void)
{
    return &context;
}
