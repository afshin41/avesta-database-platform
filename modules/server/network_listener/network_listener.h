#ifndef NETWORK_LISTENER_H
#define NETWORK_LISTENER_H

#include <stdint.h>

#include "network_listener_types.h"


int network_listener_init(void);

int network_listener_start(void);

int network_listener_stop(void);

int network_listener_shutdown(void);


int network_listener_set_address_family(
    network_listener_address_family_t family
);

int network_listener_set_port(uint16_t port);


network_listener_address_family_t
network_listener_get_address_family(void);

uint16_t network_listener_get_port(void);

int network_listener_get_socket_fd(void);


network_listener_state_t
network_listener_get_state(void);


network_listener_context_t*
network_listener_get_context(void);


#endif
