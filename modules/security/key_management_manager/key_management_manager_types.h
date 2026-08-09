#ifndef KEY_MANAGEMENT_MANAGER_TYPES_H
#define KEY_MANAGEMENT_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    KEY_MANAGEMENT_MANAGER_CREATED,
    KEY_MANAGEMENT_MANAGER_INITIALIZED,
    KEY_MANAGEMENT_MANAGER_RUNNING,
    KEY_MANAGEMENT_MANAGER_STOPPED,
    KEY_MANAGEMENT_MANAGER_ERROR
} key_management_manager_state_t;

typedef struct
{
    uint32_t key_id;
    uint8_t active;
    uint8_t key[32];
    uint32_t key_length;
} managed_key_t;

typedef struct
{
    key_management_manager_state_t state;
    uint32_t max_keys;
    uint32_t active_keys;
    uint32_t next_key_id;
    uint8_t initialized;
    uint8_t running;
    uint8_t shutdown;
} key_management_manager_context_t;

#endif
