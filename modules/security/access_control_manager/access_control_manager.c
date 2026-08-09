#include "access_control_manager.h"
#include "access_control_manager_statistics.h"

#include <stdlib.h>

#define ACCESS_CONTROL_MANAGER_DEFAULT_MAX_USERS 64U

static access_control_manager_context_t context;
static access_control_entry_t *users = NULL;

static int access_control_manager_error(void)
{
    context.state = ACCESS_CONTROL_MANAGER_ERROR;
    access_control_manager_get_statistics()->error_count++;
    return -1;
}

static access_control_entry_t*
find_user(uint32_t user_id)
{
    uint32_t i;

    if (users == NULL)
    {
        return NULL;
    }

    for (i = 0; i < context.max_users; ++i)
    {
        if (users[i].active && users[i].user_id == user_id)
        {
            return &users[i];
        }
    }

    return NULL;
}

int access_control_manager_init(void)
{
    if (context.shutdown)
    {
        return access_control_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    users = calloc(
        ACCESS_CONTROL_MANAGER_DEFAULT_MAX_USERS,
        sizeof(access_control_entry_t)
    );

    if (users == NULL)
    {
        return access_control_manager_error();
    }

    context.state = ACCESS_CONTROL_MANAGER_INITIALIZED;
    context.max_users = ACCESS_CONTROL_MANAGER_DEFAULT_MAX_USERS;
    context.active_users = 0;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    access_control_manager_statistics_init();

    return 0;
}

int access_control_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return access_control_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;
    context.state = ACCESS_CONTROL_MANAGER_RUNNING;

    access_control_manager_get_statistics()->start_count++;

    return 0;
}

int access_control_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return access_control_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;
    context.state = ACCESS_CONTROL_MANAGER_STOPPED;

    access_control_manager_get_statistics()->stop_count++;

    return 0;
}

int access_control_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;
        access_control_manager_get_statistics()->stop_count++;
    }

    free(users);
    users = NULL;

    context.active_users = 0;
    context.initialized = 0;
    context.shutdown = 1;
    context.state = ACCESS_CONTROL_MANAGER_STOPPED;

    return 0;
}

int access_control_manager_set_max_users(uint32_t max_users)
{
    if (!context.initialized || context.shutdown)
    {
        return access_control_manager_error();
    }

    if (context.running)
    {
        return access_control_manager_error();
    }

    if (max_users == 0 ||
        max_users > ACCESS_CONTROL_MANAGER_DEFAULT_MAX_USERS)
    {
        return access_control_manager_error();
    }

    context.max_users = max_users;

    return 0;
}

int access_control_manager_add_user(uint32_t user_id)
{
    uint32_t i;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0)
    {
        return access_control_manager_error();
    }

    if (find_user(user_id) != NULL)
    {
        return access_control_manager_error();
    }

    if (context.active_users >= context.max_users)
    {
        return access_control_manager_error();
    }

    for (i = 0; i < context.max_users; ++i)
    {
        if (!users[i].active)
        {
            users[i].active = 1;
            users[i].user_id = user_id;
            users[i].permissions = 0;

            context.active_users++;

            access_control_manager_get_statistics()->
                user_add_count++;

            return 0;
        }
    }

    return access_control_manager_error();
}

int access_control_manager_remove_user(uint32_t user_id)
{
    access_control_entry_t *user;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0)
    {
        return access_control_manager_error();
    }

    user = find_user(user_id);

    if (user == NULL)
    {
        return access_control_manager_error();
    }

    user->active = 0;
    user->user_id = 0;
    user->permissions = 0;

    context.active_users--;

    access_control_manager_get_statistics()->
        user_remove_count++;

    return 0;
}

int access_control_manager_grant_permission(
    uint32_t user_id,
    uint32_t permission
)
{
    access_control_entry_t *user;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0 ||
        permission == 0)
    {
        return access_control_manager_error();
    }

    user = find_user(user_id);

    if (user == NULL)
    {
        access_control_manager_get_statistics()->
            permission_denied_count++;

        return access_control_manager_error();
    }

    user->permissions |= permission;

    access_control_manager_get_statistics()->
        permission_grant_count++;

    return 0;
}

access_control_result_t
access_control_manager_check_access(
    uint32_t user_id,
    uint32_t permission
)
{
    access_control_entry_t *user;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0 ||
        permission == 0)
    {
        access_control_manager_get_statistics()->
            permission_denied_count++;

        access_control_manager_error();

        return ACCESS_CONTROL_DENIED;
    }

    user = find_user(user_id);

    if (user == NULL ||
        (user->permissions & permission) != permission)
    {
        access_control_manager_get_statistics()->
            permission_denied_count++;

        access_control_manager_get_statistics()->
            access_deny_count++;

        return ACCESS_CONTROL_DENIED;
    }

    access_control_manager_get_statistics()->
        access_allow_count++;

    return ACCESS_CONTROL_ALLOWED;
}

int access_control_manager_revoke_permission(
    uint32_t user_id,
    uint32_t permission
)
{
    access_control_entry_t *user;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        user_id == 0 ||
        permission == 0)
    {
        return access_control_manager_error();
    }

    user = find_user(user_id);

    if (user == NULL)
    {
        return access_control_manager_error();
    }

    user->permissions &= ~permission;

    access_control_manager_get_statistics()->
        permission_revoke_count++;

    return 0;
}

access_control_manager_state_t
access_control_manager_get_state(void)
{
    return context.state;
}

access_control_manager_context_t*
access_control_manager_get_context(void)
{
    return &context;
}
