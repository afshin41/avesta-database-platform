#include "session_security_manager.h"
#include "session_security_manager_statistics.h"

#include <stdlib.h>

#define SESSION_SECURITY_MANAGER_DEFAULT_MAX_SESSIONS 64U
#define SESSION_SECURITY_MANAGER_FIRST_SESSION_ID 1U

static session_security_manager_context_t context;

static session_security_manager_session_t* sessions = NULL;

static int session_security_manager_error(void)
{
    context.state = SESSION_SECURITY_MANAGER_ERROR;

    session_security_manager_get_statistics()->
        error_count++;

    return -1;
}

static void session_security_manager_release_sessions(void)
{
    free(sessions);
    sessions = NULL;
}

static uint64_t
session_security_manager_generate_token(
    uint32_t session_id,
    uint32_t user_id
)
{
    uint64_t token;

    token = ((uint64_t)session_id << 32);
    token ^= (uint64_t)user_id * 0x9E3779B1ULL;
    token ^= 0xA5A5A5A55A5A5A5AULL;

    return token;
}

int session_security_manager_init(void)
{
    if (context.shutdown)
    {
        return session_security_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    sessions = calloc(
        SESSION_SECURITY_MANAGER_DEFAULT_MAX_SESSIONS,
        sizeof(session_security_manager_session_t)
    );

    if (sessions == NULL)
    {
        return session_security_manager_error();
    }

    context.state =
        SESSION_SECURITY_MANAGER_INITIALIZED;

    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    context.max_sessions =
        SESSION_SECURITY_MANAGER_DEFAULT_MAX_SESSIONS;

    context.active_sessions = 0;

    context.next_session_id =
        SESSION_SECURITY_MANAGER_FIRST_SESSION_ID;

    session_security_manager_statistics_init();

    return 0;
}

int session_security_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return session_security_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;

    context.state =
        SESSION_SECURITY_MANAGER_RUNNING;

    session_security_manager_get_statistics()->
        start_count++;

    return 0;
}

int session_security_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return session_security_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;

    context.state =
        SESSION_SECURITY_MANAGER_STOPPED;

    session_security_manager_get_statistics()->
        stop_count++;

    return 0;
}

int session_security_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;

        session_security_manager_get_statistics()->
            stop_count++;
    }

    session_security_manager_release_sessions();

    context.active_sessions = 0;
    context.initialized = 0;
    context.shutdown = 1;

    context.state =
        SESSION_SECURITY_MANAGER_STOPPED;

    return 0;
}

int session_security_manager_set_max_sessions(
    uint32_t max_sessions
)
{
    if (!context.initialized ||
        context.shutdown)
    {
        return session_security_manager_error();
    }

    if (context.running)
    {
        return session_security_manager_error();
    }

    if (max_sessions == 0 ||
        max_sessions >
        SESSION_SECURITY_MANAGER_DEFAULT_MAX_SESSIONS)
    {
        return session_security_manager_error();
    }

    context.max_sessions = max_sessions;

    return 0;
}

int session_security_manager_create(
    uint32_t user_id,
    uint32_t lifetime,
    uint32_t* session_id,
    uint64_t* token
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0 ||
        lifetime == 0 ||
        session_id == NULL ||
        token == NULL)
    {
        return session_security_manager_error();
    }

    if (context.active_sessions >= context.max_sessions)
    {
        return session_security_manager_error();
    }

    for (index = 0;
         index < context.max_sessions;
         ++index)
    {
        if (!sessions[index].active)
        {
            sessions[index].active = 1;

            sessions[index].session_id =
                context.next_session_id++;

            sessions[index].user_id =
                user_id;

            sessions[index].lifetime =
                lifetime;

            sessions[index].age = 0;

            sessions[index].token =
                session_security_manager_generate_token(
                    sessions[index].session_id,
                    user_id
                );

            context.active_sessions++;

            *session_id =
                sessions[index].session_id;

            *token =
                sessions[index].token;

            session_security_manager_get_statistics()->
                session_create_count++;

            return 0;
        }
    }

    return session_security_manager_error();
}

int session_security_manager_validate(
    uint32_t session_id,
    uint64_t token
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        session_id == 0 ||
        token == 0)
    {
        return session_security_manager_error();
    }

    for (index = 0;
         index < context.max_sessions;
         ++index)
    {
        if (sessions[index].active &&
            sessions[index].session_id == session_id)
        {
            if (sessions[index].token == token)
            {
                session_security_manager_get_statistics()->
                    validation_success_count++;

                return 0;
            }

            session_security_manager_get_statistics()->
                validation_failure_count++;

            return -1;
        }
    }

    session_security_manager_get_statistics()->
        validation_failure_count++;

    return -1;
}

int session_security_manager_destroy(
    uint32_t session_id
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        session_id == 0)
    {
        return session_security_manager_error();
    }

    for (index = 0;
         index < context.max_sessions;
         ++index)
    {
        if (sessions[index].active &&
            sessions[index].session_id == session_id)
        {
            sessions[index].active = 0;
            sessions[index].token = 0;
            sessions[index].user_id = 0;
            sessions[index].lifetime = 0;
            sessions[index].age = 0;

            context.active_sessions--;

            session_security_manager_get_statistics()->
                session_destroy_count++;

            return 0;
        }
    }

    return session_security_manager_error();
}

int session_security_manager_expire(
    uint32_t session_id
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        session_id == 0)
    {
        return session_security_manager_error();
    }

    for (index = 0;
         index < context.max_sessions;
         ++index)
    {
        if (sessions[index].active &&
            sessions[index].session_id == session_id)
        {
            sessions[index].age =
                sessions[index].lifetime;

            sessions[index].active = 0;
            sessions[index].token = 0;

            context.active_sessions--;

            session_security_manager_get_statistics()->
                expiration_count++;

            return 0;
        }
    }

    return session_security_manager_error();
}

session_security_manager_state_t
session_security_manager_get_state(void)
{
    return context.state;
}

session_security_manager_context_t*
session_security_manager_get_context(void)
{
    return &context;
}
