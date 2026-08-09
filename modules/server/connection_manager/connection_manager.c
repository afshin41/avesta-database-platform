#include "connection_manager.h"
#include "connection_manager_statistics.h"

#include <stdlib.h>

#define CONNECTION_MANAGER_DEFAULT_MAX_CONNECTIONS 64U
#define CONNECTION_MANAGER_FIRST_CONNECTION_ID 1U

static connection_manager_context_t context;

static connection_manager_connection_t* connections = NULL;

static int connection_manager_error(void)
{
    context.state = CONNECTION_MANAGER_ERROR;
    connection_manager_get_statistics()->error_count++;
    return -1;
}

static void connection_manager_release_connections(void)
{
    free(connections);
    connections = NULL;
}

int connection_manager_init(void)
{
    if (context.shutdown)
    {
        return connection_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    connections = calloc(
        CONNECTION_MANAGER_DEFAULT_MAX_CONNECTIONS,
        sizeof(connection_manager_connection_t)
    );

    if (connections == NULL)
    {
        return connection_manager_error();
    }

    context.state = CONNECTION_MANAGER_INITIALIZED;
    context.max_connections =
        CONNECTION_MANAGER_DEFAULT_MAX_CONNECTIONS;
    context.active_connections = 0;
    context.next_connection_id =
        CONNECTION_MANAGER_FIRST_CONNECTION_ID;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;


    return 0;
}

int connection_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return connection_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;
    context.state = CONNECTION_MANAGER_RUNNING;

    connection_manager_get_statistics()->start_count++;

    return 0;
}

int connection_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return connection_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;
    context.state = CONNECTION_MANAGER_STOPPED;

    connection_manager_get_statistics()->stop_count++;

    return 0;
}

int connection_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;
        connection_manager_get_statistics()->stop_count++;
    }

    connection_manager_release_connections();

    context.active_connections = 0;
    context.initialized = 0;
    context.shutdown = 1;
    context.state = CONNECTION_MANAGER_STOPPED;

    return 0;
}

int connection_manager_set_max_connections(uint32_t max_connections)
{
    if (!context.initialized || context.shutdown)
    {
        return connection_manager_error();
    }

    if (context.running)
    {
        return connection_manager_error();
    }

    if (max_connections == 0 ||
        max_connections > CONNECTION_MANAGER_DEFAULT_MAX_CONNECTIONS)
    {
        return connection_manager_error();
    }

    context.max_connections = max_connections;

    return 0;
}

int connection_manager_open(uint32_t* connection_id)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        connection_id == NULL)
    {
        return connection_manager_error();
    }

    if (context.active_connections >= context.max_connections)
    {
        return connection_manager_error();
    }

    for (index = 0; index < context.max_connections; ++index)
    {
        if (!connections[index].active)
        {
            connections[index].active = 1;
            connections[index].connection_id =
                context.next_connection_id++;

            context.active_connections++;

            *connection_id = connections[index].connection_id;

            connection_manager_get_statistics()->
                connection_open_count++;

            return 0;
        }
    }

    return connection_manager_error();
}

int connection_manager_close(uint32_t connection_id)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        connection_id == 0)
    {
        return connection_manager_error();
    }

    for (index = 0; index < context.max_connections; ++index)
    {
        if (connections[index].active &&
            connections[index].connection_id == connection_id)
        {
            connections[index].active = 0;
            context.active_connections--;

            connection_manager_get_statistics()->
                connection_close_count++;

            return 0;
        }
    }

    return connection_manager_error();
}

connection_manager_state_t
connection_manager_get_state(void)
{
    return context.state;
}

connection_manager_context_t*
connection_manager_get_context(void)
{
    return &context;
}
