#include "database_server_manager.h"
#include "database_server_manager_statistics.h"

static database_server_manager_context_t context =
{
    DATABASE_SERVER_MANAGER_CREATED,
    0,
    0,
    0
};


int database_server_manager_init(void)
{
    if (context.shutdown)
    {
        context.state = DATABASE_SERVER_MANAGER_ERROR;
        database_server_manager_get_statistics()->error_count++;
        return -1;
    }

    if (context.initialized)
    {
        return 0;
    }

    context.state = DATABASE_SERVER_MANAGER_INITIALIZED;
    context.initialized = 1;
    context.running = 0;

    database_server_manager_statistics_init();

    return 0;
}


int database_server_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        context.state = DATABASE_SERVER_MANAGER_ERROR;
        database_server_manager_get_statistics()->error_count++;
        return -1;
    }

    if (context.running)
    {
        return 0;
    }

    context.state = DATABASE_SERVER_MANAGER_RUNNING;
    context.running = 1;

    database_server_manager_get_statistics()->start_count++;

    return 0;
}


int database_server_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        context.state = DATABASE_SERVER_MANAGER_ERROR;
        database_server_manager_get_statistics()->error_count++;
        return -1;
    }

    if (!context.running)
    {
        context.state = DATABASE_SERVER_MANAGER_STOPPED;
        return 0;
    }

    context.running = 0;
    context.state = DATABASE_SERVER_MANAGER_STOPPED;

    database_server_manager_get_statistics()->stop_count++;

    return 0;
}


int database_server_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        if (database_server_manager_stop() != 0)
        {
            context.state = DATABASE_SERVER_MANAGER_ERROR;
            return -1;
        }
    }

    context.initialized = 0;
    context.running = 0;
    context.shutdown = 1;
    context.state = DATABASE_SERVER_MANAGER_STOPPED;

    return 0;
}


database_server_manager_state_t
database_server_manager_get_state(void)
{
    return context.state;
}


const database_server_manager_context_t*
database_server_manager_get_context(void)
{
    return &context;
}
