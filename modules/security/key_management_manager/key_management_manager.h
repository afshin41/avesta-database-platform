#ifndef KEY_MANAGEMENT_MANAGER_H
#define KEY_MANAGEMENT_MANAGER_H

#include <stdint.h>

#include "key_management_manager_types.h"

int key_management_manager_init(void);
int key_management_manager_start(void);
int key_management_manager_stop(void);
int key_management_manager_shutdown(void);

int key_management_manager_set_max_keys(uint32_t max_keys);

int key_management_manager_create_key(
    const uint8_t *key_data,
    uint32_t key_length,
    uint32_t *key_id
);

int key_management_manager_destroy_key(uint32_t key_id);

int key_management_manager_get_key(
    uint32_t key_id,
    uint8_t *key_data,
    uint32_t key_size,
    uint32_t *key_length
);

key_management_manager_state_t
key_management_manager_get_state(void);

key_management_manager_context_t *
key_management_manager_get_context(void);

#endif
