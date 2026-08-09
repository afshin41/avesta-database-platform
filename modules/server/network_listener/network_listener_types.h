#ifndef NETWORK_LISTENER_TYPES_H
#define NETWORK_LISTENER_TYPES_H

#include <stdint.h>

typedef enum
{
    NETWORK_LISTENER_CREATED,
    NETWORK_LISTENER_INITIALIZED,
    NETWORK_LISTENER_RUNNING,
    NETWORK_LISTENER_STOPPED,
    NETWORK_LISTENER_ERROR

} network_listener_state_t;


typedef enum
{
    NETWORK_LISTENER_IPV4 = 4,
    NETWORK_LISTENER_IPV6 = 6

} network_listener_address_family_t;


typedef struct
{
    network_listener_state_t state;

    int socket_fd;

    network_listener_address_family_t address_family;

    uint16_t configured_port;
    uint16_t active_port;

    uint32_t backlog;

    uint8_t initialized;
    uint8_t running;
    uint8_t shutdown;

} network_listener_context_t;


#endif
