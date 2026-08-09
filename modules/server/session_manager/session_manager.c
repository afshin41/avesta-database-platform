#include "session_manager.h"
#include "session_manager_statistics.h"

#include <stdlib.h>

#define SESSION_MANAGER_DEFAULT_MAX_SESSIONS 64U
#define SESSION_MANAGER_FIRST_SESSION_ID 1U

static session_manager_context_t context;
static session_manager_session_t* sessions = NULL;

static int session_manager_error(void)
{
    context.state = SESSION_MANAGER_ERROR;
    session_manager_get_statistics()->error_count++;
    return -1;
}

static void session_manager_release_sessions(void)
{
    free(sessions);
    sessions = NULL;
}

int session_manager_init(void)
{
    if (context.shutdown)
    {
        return session_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    sessions = calloc(
        SESSION_MANAGER_DEFAULT_MAX_SESSIONS,
        sizeof(session_manager_session_t)
    );

    if (sessions == NULL)
    {
        return session_manager_error();
    }

    context.state = SESSION_MANAGER_INITIALIZED;
    context.max_sessions = SESSION_MANAGER_DEFAULT_MAX_SESSIONS;
    context.active_sessions = 0;
    context.next_session_id = SESSION_MANAGER_FIRST_SESSION_ID;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    session_manager_statistics_init();

    return 0;
}

int session_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return session_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;
    context.state = SESSION_MANAGER_RUNNING;

    session_manager_get_statistics()->start_count++;

    return 0;
}

int session_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return session_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;
    context.state = SESSION_MANAGER_STOPPED;

    session_manager_get_statistics()->stop_count++;

    return 0;
}

int session_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;
        session_manager_get_statistics()->stop_count++;
    }

    session_manager_release_sessions();

    context.active_sessions = 0;
    context.initialized = 0;
    context.shutdown = 1;
    context.state = SESSION_MANAGER_STOPPED;

    return 0;
}

int session_manager_set_max_sessions(uint32_t max_sessions)
{
    if (!context.initialized || context.shutdown)
    {
        return session_manager_error();
    }

    if (context.running)
    {
        return session_manager_error();
    }

    if (max_sessions == 0 ||
        max_sessions > SESSION_MANAGER_DEFAULT_MAX_SESSIONS)
    {
        return session_manager_error();
    }

    context.max_sessions = max_sessions;

    return 0;
}

int session_manager_open(uint32_t* session_id)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        session_id == NULL)
    {
        return session_manager_error();
    }

    if (context.active_sessions >= context.max_sessions)
    {
        return session_manager_error();
    }

    for (index = 0; index < context.max_sessions; ++index)
    {
        if (!sessions[index].active)
        {
            sessions[index].active = 1;
            sessions[index].session_id =
                context.next_session_id++;

            context.active_sessions++;

            *session_id = sessions[index].session_id;

            session_manager_get_statistics()->
                session_open_count++;

            return 0;
        }
    }

    return session_manager_error();
}

int session_manager_close(uint32_t session_id)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        session_id == 0)
    {
        return session_manager_error();
    }

    for (index = 0; index < context.max_sessions; ++index)
    {
        if (sessions[index].active &&
            sessions[index].session_id == session_id)
        {
            sessions[index].active = 0;
            context.active_sessions--;

            session_manager_get_statistics()->
                session_close_count++;

            return 0;
        }
    }

    return session_manager_error();
}

session_manager_state_t
session_manager_get_state(void)
{
    return context.state;
}

session_manager_context_t*
session_manager_get_context(void)
{
    return &context;
}
