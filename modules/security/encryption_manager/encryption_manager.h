#ifndef ENCRYPTION_MANAGER_H
#define ENCRYPTION_MANAGER_H

#include <stdint.h>

#include "encryption_manager_types.h"
#include "encryption_manager_statistics.h"

int encryption_manager_init(void);
int encryption_manager_start(void);
int encryption_manager_stop(void);
int encryption_manager_shutdown(void);

int encryption_manager_set_max_keys(uint32_t max_keys);

int encryption_manager_create_key(
    const uint8_t *key_data,
    uint32_t key_length,
    uint32_t *key_id
);

int encryption_manager_destroy_key(uint32_t key_id);

int encryption_manager_encrypt(
    uint32_t key_id,
    const uint8_t *input,
    uint32_t input_length,
    uint8_t *output,
    uint32_t output_size,
    uint32_t *output_length
);

int encryption_manager_decrypt(
    uint32_t key_id,
    const uint8_t *input,
    uint32_t input_length,
    uint8_t *output,
    uint32_t output_size,
    uint32_t *output_length
);

encryption_manager_state_t
encryption_manager_get_state(void);

encryption_manager_context_t *
encryption_manager_get_context(void);

#endif
