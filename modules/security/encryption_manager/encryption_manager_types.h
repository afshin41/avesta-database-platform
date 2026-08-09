#ifndef ENCRYPTION_MANAGER_TYPES_H
#define ENCRYPTION_MANAGER_TYPES_H

#include <stdint.h>

typedef enum
{
    ENCRYPTION_MANAGER_CREATED,
    ENCRYPTION_MANAGER_INITIALIZED,
    ENCRYPTION_MANAGER_RUNNING,
    ENCRYPTION_MANAGER_STOPPED,
    ENCRYPTION_MANAGER_ERROR
} encryption_manager_state_t;

typedef struct
{
    uint32_t key_id;
    uint8_t active;
    uint8_t key[32];
    uint32_t key_length;
} encryption_key_t;

typedef struct
{
    encryption_manager_state_t state;
    uint32_t max_keys;
    uint32_t active_keys;
    uint32_t next_key_id;
    uint8_t initialized;
    uint8_t running;
    uint8_t shutdown;
} encryption_manager_context_t;

#endif
