#include "network_listener.h"
#include "network_listener_statistics.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


#define NETWORK_LISTENER_DEFAULT_PORT 0U
#define NETWORK_LISTENER_DEFAULT_BACKLOG 16U


static network_listener_context_t context;


static int network_listener_error(void)
{
    context.state = NETWORK_LISTENER_ERROR;

    network_listener_statistics_record_error();

    return -1;
}


static void network_listener_close_socket(void)
{
    if (context.socket_fd >= 0)
    {
        close(context.socket_fd);
        context.socket_fd = -1;
    }

    context.running = 0;
    context.active_port = 0;
}


int network_listener_init(void)
{
    if (context.shutdown)
    {
        return network_listener_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    context.state = NETWORK_LISTENER_INITIALIZED;
    context.socket_fd = -1;

    context.address_family = NETWORK_LISTENER_IPV4;

    context.configured_port = NETWORK_LISTENER_DEFAULT_PORT;
    context.active_port = 0;

    context.backlog = NETWORK_LISTENER_DEFAULT_BACKLOG;

    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    return 0;
}


int network_listener_set_address_family(
    network_listener_address_family_t family
)
{
    if (!context.initialized || context.shutdown)
    {
        return network_listener_error();
    }

    if (context.running)
    {
        return network_listener_error();
    }

    if (family != NETWORK_LISTENER_IPV4 &&
        family != NETWORK_LISTENER_IPV6)
    {
        return network_listener_error();
    }

    context.address_family = family;

    return 0;
}


int network_listener_set_port(uint16_t port)
{
    if (!context.initialized || context.shutdown)
    {
        return network_listener_error();
    }

    if (context.running)
    {
        return network_listener_error();
    }

    context.configured_port = port;

    return 0;
}


int network_listener_start(void)
{
    int socket_fd;
    int reuse = 1;

    struct sockaddr_storage address;
    socklen_t address_length;


    if (!context.initialized || context.shutdown)
    {
        return network_listener_error();
    }

    if (context.running)
    {
        return 0;
    }


    if (context.address_family == NETWORK_LISTENER_IPV4)
    {
        struct sockaddr_in* ipv4;

        socket_fd = socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

        if (socket_fd < 0)
        {
            return network_listener_error();
        }

        ipv4 = (struct sockaddr_in*)&address;

        ipv4->sin_family = AF_INET;
        ipv4->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        ipv4->sin_port = htons(context.configured_port);

        address_length = sizeof(*ipv4);
    }
    else
    {
        struct sockaddr_in6* ipv6;

        socket_fd = socket(
            AF_INET6,
            SOCK_STREAM,
            0
        );

        if (socket_fd < 0)
        {
            return network_listener_error();
        }

        ipv6 = (struct sockaddr_in6*)&address;

        ipv6->sin6_family = AF_INET6;
        ipv6->sin6_addr = in6addr_loopback;
        ipv6->sin6_port = htons(context.configured_port);

        address_length = sizeof(*ipv6);
    }


    if (setsockopt(
            socket_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &reuse,
            sizeof(reuse)
        ) < 0)
    {
        close(socket_fd);

        return network_listener_error();
    }


    if (bind(
            socket_fd,
            (struct sockaddr*)&address,
            address_length
        ) < 0)
    {
        close(socket_fd);

        return network_listener_error();
    }


    if (listen(
            socket_fd,
            (int)context.backlog
        ) < 0)
    {
        close(socket_fd);

        return network_listener_error();
    }


    address_length = sizeof(address);


    if (getsockname(
            socket_fd,
            (struct sockaddr*)&address,
            &address_length
        ) < 0)
    {
        close(socket_fd);

        return network_listener_error();
    }


    if (context.address_family == NETWORK_LISTENER_IPV4)
    {
        struct sockaddr_in* ipv4 =
            (struct sockaddr_in*)&address;

        context.active_port =
            ntohs(ipv4->sin_port);
    }
    else
    {
        struct sockaddr_in6* ipv6 =
            (struct sockaddr_in6*)&address;

        context.active_port =
            ntohs(ipv6->sin6_port);
    }


    context.socket_fd = socket_fd;
    context.running = 1;
    context.state = NETWORK_LISTENER_RUNNING;


    network_listener_statistics_record_start();


    return 0;
}


int network_listener_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return network_listener_error();
    }

    if (!context.running)
    {
        return 0;
    }

    network_listener_close_socket();

    context.state = NETWORK_LISTENER_STOPPED;

    network_listener_statistics_record_stop();

    return 0;
}


int network_listener_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        network_listener_close_socket();
    }

    context.shutdown = 1;
    context.initialized = 0;
    context.state = NETWORK_LISTENER_STOPPED;

    return 0;
}


network_listener_address_family_t
network_listener_get_address_family(void)
{
    return context.address_family;
}


uint16_t network_listener_get_port(void)
{
    return context.active_port;
}


int network_listener_get_socket_fd(void)
{
    return context.socket_fd;
}


network_listener_state_t
network_listener_get_state(void)
{
    return context.state;
}


network_listener_context_t*
network_listener_get_context(void)
{
    return &context;
}
