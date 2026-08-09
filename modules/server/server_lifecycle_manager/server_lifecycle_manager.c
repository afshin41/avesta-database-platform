#include "server_lifecycle_manager.h"
#include "server_lifecycle_manager_statistics.h"


static server_lifecycle_manager_context_t context;


static int lifecycle_error(void)
{
    context.state = SERVER_LIFECYCLE_MANAGER_ERROR;

    server_lifecycle_manager_get_statistics()->error_count++;

    return -1;
}


int server_lifecycle_manager_init(void)
{
    if (context.shutdown)
    {
        return lifecycle_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    context.state = SERVER_LIFECYCLE_MANAGER_INITIALIZED;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    server_lifecycle_manager_statistics_init();

    return 0;
}


int server_lifecycle_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return lifecycle_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.state = SERVER_LIFECYCLE_MANAGER_RUNNING;
    context.running = 1;

    server_lifecycle_manager_get_statistics()->start_count++;

    return 0;
}


int server_lifecycle_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return lifecycle_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.state = SERVER_LIFECYCLE_MANAGER_STOPPED;
    context.running = 0;

    server_lifecycle_manager_get_statistics()->stop_count++;

    return 0;
}


int server_lifecycle_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;
    }

    context.shutdown = 1;
    context.initialized = 0;
    context.state = SERVER_LIFECYCLE_MANAGER_STOPPED;

    return 0;
}


server_lifecycle_manager_state_t
server_lifecycle_manager_get_state(void)
{
    return context.state;
}


server_lifecycle_manager_context_t*
server_lifecycle_manager_get_context(void)
{
    return &context;
}
