#include <stdlib.h>
#include <string.h>

#include "key_management_manager.h"
#include "key_management_manager_statistics.h"

#define KEY_MANAGEMENT_MANAGER_DEFAULT_MAX_KEYS 16U
#define KEY_MANAGEMENT_MANAGER_MAX_KEY_LENGTH 32U

static key_management_manager_context_t context;
static managed_key_t *keys = NULL;

static int key_management_manager_error(void)
{
    key_management_manager_get_statistics()->error_count++;
    return -1;
}

static int key_management_manager_fatal_error(void)
{
    context.state = KEY_MANAGEMENT_MANAGER_ERROR;
    key_management_manager_get_statistics()->error_count++;
    return -1;
}

static managed_key_t *find_key(uint32_t key_id)
{
    uint32_t index;

    if (keys == NULL || key_id == 0)
        return NULL;

    for (index = 0; index < context.max_keys; ++index)
    {
        if (keys[index].active && keys[index].key_id == key_id)
            return &keys[index];
    }

    return NULL;
}

int key_management_manager_init(void)
{
    if (context.shutdown)
        return key_management_manager_error();

    if (context.initialized)
        return key_management_manager_error();

    keys = calloc(
        KEY_MANAGEMENT_MANAGER_DEFAULT_MAX_KEYS,
        sizeof(managed_key_t)
    );

    if (keys == NULL)
        return key_management_manager_error();

    context.state = KEY_MANAGEMENT_MANAGER_INITIALIZED;
    context.max_keys = KEY_MANAGEMENT_MANAGER_DEFAULT_MAX_KEYS;
    context.active_keys = 0;
    context.next_key_id = 1;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    key_management_manager_statistics_init();

    return 0;
}

int key_management_manager_start(void)
{
    if (!context.initialized ||
        context.shutdown ||
        context.running ||
        context.state != KEY_MANAGEMENT_MANAGER_INITIALIZED)
        return key_management_manager_error();

    context.running = 1;
    context.state = KEY_MANAGEMENT_MANAGER_RUNNING;

    key_management_manager_get_statistics()->start_count++;

    return 0;
}

int key_management_manager_stop(void)
{
    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        context.state != KEY_MANAGEMENT_MANAGER_RUNNING)
        return key_management_manager_error();

    context.running = 0;
    context.state = KEY_MANAGEMENT_MANAGER_STOPPED;

    key_management_manager_get_statistics()->stop_count++;

    return 0;
}

int key_management_manager_shutdown(void)
{
    if (context.shutdown)
        return key_management_manager_fatal_error();

    context.shutdown = 1;
    context.running = 0;
    context.initialized = 0;
    context.state = KEY_MANAGEMENT_MANAGER_STOPPED;

    free(keys);
    keys = NULL;

    return 0;
}

int key_management_manager_set_max_keys(uint32_t max_keys)
{
    managed_key_t *new_keys;

    if (!context.initialized ||
        context.shutdown ||
        context.running ||
        max_keys == 0)
        return key_management_manager_error();

    if (context.active_keys != 0)
        return key_management_manager_error();

    new_keys = calloc(max_keys, sizeof(managed_key_t));

    if (new_keys == NULL)
        return key_management_manager_error();

    free(keys);
    keys = new_keys;
    context.max_keys = max_keys;

    return 0;
}

int key_management_manager_create_key(
    const uint8_t *key_data,
    uint32_t key_length,
    uint32_t *key_id)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        key_data == NULL ||
        key_id == NULL ||
        key_length == 0 ||
        key_length > KEY_MANAGEMENT_MANAGER_MAX_KEY_LENGTH)
        return key_management_manager_error();

    if (context.active_keys >= context.max_keys)
        return key_management_manager_error();

    for (index = 0; index < context.max_keys; ++index)
    {
        if (!keys[index].active)
        {
            keys[index].key_id = context.next_key_id++;
            keys[index].active = 1;
            keys[index].key_length = key_length;

            memcpy(
                keys[index].key,
                key_data,
                key_length
            );

            context.active_keys++;
            *key_id = keys[index].key_id;

            key_management_manager_get_statistics()
                ->key_create_count++;

            return 0;
        }
    }

    return key_management_manager_error();
}

int key_management_manager_destroy_key(uint32_t key_id)
{
    managed_key_t *key;

    if (!context.initialized ||
        context.shutdown ||
        !context.running)
        return key_management_manager_error();

    key = find_key(key_id);

    if (key == NULL)
        return key_management_manager_error();

    memset(key, 0, sizeof(*key));

    context.active_keys--;

    key_management_manager_get_statistics()
        ->key_destroy_count++;

    return 0;
}

int key_management_manager_get_key(
    uint32_t key_id,
    uint8_t *key_data,
    uint32_t key_size,
    uint32_t *key_length)
{
    managed_key_t *key;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        key_data == NULL ||
        key_length == NULL)
    {
        key_management_manager_get_statistics()
            ->key_get_failure_count++;

        return key_management_manager_error();
    }

    key = find_key(key_id);

    if (key == NULL || key_size < key->key_length)
    {
        key_management_manager_get_statistics()
            ->key_get_failure_count++;

        return key_management_manager_error();
    }

    memcpy(key_data, key->key, key->key_length);
    *key_length = key->key_length;

    key_management_manager_get_statistics()
        ->key_get_count++;

    return 0;
}

key_management_manager_state_t
key_management_manager_get_state(void)
{
    return context.state;
}

key_management_manager_context_t *
key_management_manager_get_context(void)
{
    return &context;
}
