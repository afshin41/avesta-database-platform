#include "authorization_manager.h"
#include "authorization_manager_statistics.h"

#include <stdlib.h>

#define AUTHORIZATION_MANAGER_DEFAULT_MAX_USERS 64U
#define AUTHORIZATION_MANAGER_FIRST_USER_ID 1U

static authorization_manager_context_t context;

static authorization_manager_user_t* users = NULL;

static int authorization_manager_error(void)
{
    context.state = AUTHORIZATION_MANAGER_ERROR;

    authorization_manager_get_statistics()->error_count++;

    return -1;
}

static void authorization_manager_release_users(void)
{
    free(users);
    users = NULL;
}

int authorization_manager_init(void)
{
    if (context.shutdown)
    {
        return authorization_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    users = calloc(
        AUTHORIZATION_MANAGER_DEFAULT_MAX_USERS,
        sizeof(authorization_manager_user_t)
    );

    if (users == NULL)
    {
        return authorization_manager_error();
    }

    context.state = AUTHORIZATION_MANAGER_INITIALIZED;

    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    context.max_users =
        AUTHORIZATION_MANAGER_DEFAULT_MAX_USERS;

    context.active_users = 0;

    context.next_user_id =
        AUTHORIZATION_MANAGER_FIRST_USER_ID;

    authorization_manager_statistics_init();

    return 0;
}

int authorization_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return authorization_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;

    context.state =
        AUTHORIZATION_MANAGER_RUNNING;

    authorization_manager_get_statistics()->
        start_count++;

    return 0;
}

int authorization_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return authorization_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;

    context.state =
        AUTHORIZATION_MANAGER_STOPPED;

    authorization_manager_get_statistics()->
        stop_count++;

    return 0;
}

int authorization_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;

        authorization_manager_get_statistics()->
            stop_count++;
    }

    authorization_manager_release_users();

    context.active_users = 0;
    context.initialized = 0;
    context.shutdown = 1;

    context.state =
        AUTHORIZATION_MANAGER_STOPPED;

    return 0;
}

int authorization_manager_set_max_users(
    uint32_t max_users
)
{
    if (!context.initialized ||
        context.shutdown)
    {
        return authorization_manager_error();
    }

    if (context.running)
    {
        return authorization_manager_error();
    }

    if (max_users == 0 ||
        max_users > AUTHORIZATION_MANAGER_DEFAULT_MAX_USERS)
    {
        return authorization_manager_error();
    }

    context.max_users = max_users;

    return 0;
}

int authorization_manager_add_user(
    uint32_t permissions,
    uint32_t* user_id
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == NULL)
    {
        return authorization_manager_error();
    }

    if (context.active_users >= context.max_users)
    {
        return authorization_manager_error();
    }

    for (index = 0;
         index < context.max_users;
         ++index)
    {
        if (!users[index].active)
        {
            users[index].active = 1;

            users[index].user_id =
                context.next_user_id++;

            users[index].permissions =
                permissions;

            context.active_users++;

            *user_id =
                users[index].user_id;

            authorization_manager_get_statistics()->
                user_add_count++;

            return 0;
        }
    }

    return authorization_manager_error();
}

int authorization_manager_remove_user(
    uint32_t user_id
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0)
    {
        return authorization_manager_error();
    }

    for (index = 0;
         index < context.max_users;
         ++index)
    {
        if (users[index].active &&
            users[index].user_id == user_id)
        {
            users[index].active = 0;
            users[index].permissions = 0;

            context.active_users--;

            authorization_manager_get_statistics()->
                user_remove_count++;

            return 0;
        }
    }

    return authorization_manager_error();
}

int authorization_manager_check_permission(
    uint32_t user_id,
    uint32_t permission
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0 ||
        permission == 0)
    {
        return authorization_manager_error();
    }

    for (index = 0;
         index < context.max_users;
         ++index)
    {
        if (users[index].active &&
            users[index].user_id == user_id)
        {
            if ((users[index].permissions & permission)
                == permission)
            {
                authorization_manager_get_statistics()->
                    permission_grant_count++;

                return 0;
            }

            authorization_manager_get_statistics()->
                permission_denied_count++;

            return -1;
        }
    }

    authorization_manager_get_statistics()->
        permission_denied_count++;

    return -1;
}

authorization_manager_state_t
authorization_manager_get_state(void)
{
    return context.state;
}

authorization_manager_context_t*
authorization_manager_get_context(void)
{
    return &context;
}
