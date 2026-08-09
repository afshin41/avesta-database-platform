#include <stdio.h>

#include "network_listener.h"
#include "network_listener_statistics.h"


static int failures = 0;


static void check(
    int condition,
    const char* name
)
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


static void test_ipv4(void)
{
    printf("\n--- IPv4 Verification ---\n");


    check(
        network_listener_set_address_family(
            NETWORK_LISTENER_IPV4
        ) == 0,
        "Configure IPv4"
    );


    check(
        network_listener_set_port(0) == 0,
        "Configure IPv4 Ephemeral Port"
    );


    check(
        network_listener_start() == 0,
        "IPv4 Start"
    );


    check(
        network_listener_get_state() ==
        NETWORK_LISTENER_RUNNING,
        "IPv4 State RUNNING"
    );


    check(
        network_listener_get_socket_fd() >= 0,
        "IPv4 Valid Socket"
    );


    check(
        network_listener_get_port() > 0,
        "IPv4 Active Port"
    );


    check(
        network_listener_stop() == 0,
        "IPv4 Stop"
    );


    check(
        network_listener_get_socket_fd() == -1,
        "IPv4 Socket Closed"
    );
}


static void test_ipv6(void)
{
    printf("\n--- IPv6 Verification ---\n");


    check(
        network_listener_set_address_family(
            NETWORK_LISTENER_IPV6
        ) == 0,
        "Configure IPv6"
    );


    check(
        network_listener_set_port(0) == 0,
        "Configure IPv6 Ephemeral Port"
    );


    check(
        network_listener_start() == 0,
        "IPv6 Start"
    );


    check(
        network_listener_get_state() ==
        NETWORK_LISTENER_RUNNING,
        "IPv6 State RUNNING"
    );


    check(
        network_listener_get_socket_fd() >= 0,
        "IPv6 Valid Socket"
    );


    check(
        network_listener_get_port() > 0,
        "IPv6 Active Port"
    );


    check(
        network_listener_stop() == 0,
        "IPv6 Stop"
    );


    check(
        network_listener_get_socket_fd() == -1,
        "IPv6 Socket Closed"
    );
}


int main(void)
{
    network_listener_statistics_t* statistics;


    printf("===============================\n");
    printf(" Network Listener Test\n");
    printf(" IPv4 / IPv6 Edge Verification\n");
    printf("===============================\n");


    check(
        network_listener_start() != 0,
        "Start Before Initialize"
    );


    check(
        network_listener_stop() != 0,
        "Stop Before Initialize"
    );


    check(
        network_listener_init() == 0,
        "Initialize"
    );


    check(
        network_listener_get_state() ==
        NETWORK_LISTENER_INITIALIZED,
        "State INITIALIZED"
    );


    test_ipv4();


    check(
        network_listener_start() == 0,
        "Repeated IPv4 Start After Stop"
    );


    check(
        network_listener_stop() == 0,
        "Repeated IPv4 Stop"
    );


    test_ipv6();


    check(
        network_listener_stop() == 0,
        "Repeated Stop"
    );


    check(
        network_listener_shutdown() == 0,
        "Shutdown"
    );


    check(
        network_listener_get_context()->shutdown == 1,
        "Shutdown Flag"
    );


    check(
        network_listener_shutdown() == 0,
        "Repeated Shutdown"
    );


    check(
        network_listener_init() != 0,
        "Initialize After Shutdown"
    );


    check(
        network_listener_start() != 0,
        "Start After Shutdown"
    );


    check(
        network_listener_stop() != 0,
        "Stop After Shutdown"
    );


    check(
        network_listener_get_state() ==
        NETWORK_LISTENER_ERROR,
        "Final Error State"
    );


    statistics = network_listener_get_statistics();


    check(
        statistics->start_count == 3,
        "Start Statistics"
    );


    check(
        statistics->stop_count == 3,
        "Stop Statistics"
    );


    check(
        statistics->error_count == 5,
        "Error Statistics"
    );


    if (failures == 0)
    {
        printf("\nRESULT: ALL NETWORK LISTENER TESTS PASSED\n");
        return 0;
    }


    printf(
        "\nRESULT: FAILED (%d failures)\n",
        failures
    );


    return 1;
}
