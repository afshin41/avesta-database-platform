#include "authentication_manager.h"
#include "authentication_manager_statistics.h"

#include <string.h>

#define AUTHENTICATION_MANAGER_DEFAULT_MAX_FAILED_ATTEMPTS 3U

static authentication_manager_context_t context;

static uint32_t configured_user_id;
static char configured_username[128];
static char configured_password[128];
static uint32_t credentials_configured;

static int authentication_manager_error(void)
{
    context.state = AUTHENTICATION_MANAGER_ERROR;
    authentication_manager_get_statistics()->error_count++;
    return -1;
}

int authentication_manager_init(void)
{
    if (context.shutdown)
        return authentication_manager_error();

    if (context.initialized)
        return 0;

    memset(&context, 0, sizeof(context));

    context.state = AUTHENTICATION_MANAGER_INITIALIZED;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;
    context.authenticated = 0;
    context.failed_attempts = 0;
    context.max_failed_attempts =
        AUTHENTICATION_MANAGER_DEFAULT_MAX_FAILED_ATTEMPTS;
    context.active_session_id = 0;

    credentials_configured = 0;
    configured_user_id = 0;
    configured_username[0] = '\0';
    configured_password[0] = '\0';

    authentication_manager_statistics_init();

    return 0;
}

int authentication_manager_start(void)
{
    if (!context.initialized || context.shutdown)
        return authentication_manager_error();

    if (context.running)
        return 0;

    context.running = 1;
    context.state = AUTHENTICATION_MANAGER_RUNNING;

    authentication_manager_get_statistics()->start_count++;

    return 0;
}

int authentication_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
        return authentication_manager_error();

    if (!context.running)
        return 0;

    context.running = 0;
    context.authenticated = 0;
    context.active_session_id = 0;
    context.state = AUTHENTICATION_MANAGER_STOPPED;

    authentication_manager_get_statistics()->stop_count++;

    return 0;
}

int authentication_manager_shutdown(void)
{
    if (context.shutdown)
        return 0;

    if (context.running)
    {
        context.running = 0;
        context.authenticated = 0;
        context.active_session_id = 0;
        authentication_manager_get_statistics()->stop_count++;
    }

    context.initialized = 0;
    context.shutdown = 1;
    context.state = AUTHENTICATION_MANAGER_STOPPED;

    return 0;
}

int authentication_manager_set_credentials(
    uint32_t user_id,
    const char* username,
    const char* password
)
{
    size_t username_length;
    size_t password_length;

    if (!context.initialized || context.shutdown)
        return authentication_manager_error();

    if (context.running)
        return authentication_manager_error();

    if (username == NULL || password == NULL || user_id == 0)
        return authentication_manager_error();

    username_length = strlen(username);
    password_length = strlen(password);

    if (username_length == 0 || username_length >= sizeof(configured_username))
        return authentication_manager_error();

    if (password_length == 0 || password_length >= sizeof(configured_password))
        return authentication_manager_error();

    configured_user_id = user_id;

    strcpy(configured_username, username);
    strcpy(configured_password, password);

    credentials_configured = 1;

    return 0;
}

int authentication_manager_authenticate(
    uint32_t user_id,
    const char* username,
    const char* password,
    uint32_t session_id
)
{
    if (!context.initialized || context.shutdown)
        return authentication_manager_error();

    if (!context.running)
        return authentication_manager_error();

    if (username == NULL || password == NULL || user_id == 0 || session_id == 0)
        return authentication_manager_error();

    if (context.authenticated)
        return authentication_manager_error();

    if (context.failed_attempts >= context.max_failed_attempts)
        return authentication_manager_error();

    if (!credentials_configured ||
        user_id != configured_user_id ||
        strcmp(username, configured_username) != 0 ||
        strcmp(password, configured_password) != 0)
    {
        context.failed_attempts++;
        authentication_manager_get_statistics()->
            authentication_failure_count++;

        if (context.failed_attempts >= context.max_failed_attempts)
            context.state = AUTHENTICATION_MANAGER_ERROR;

        return AUTHENTICATION_RESULT_INVALID_CREDENTIALS;
    }

    context.authenticated = 1;
    context.active_session_id = session_id;
    context.failed_attempts = 0;

    authentication_manager_get_statistics()->
        authentication_success_count++;

    return AUTHENTICATION_RESULT_SUCCESS;
}

int authentication_manager_logout(void)
{
    if (!context.initialized || context.shutdown)
        return authentication_manager_error();

    if (!context.running)
        return authentication_manager_error();

    if (!context.authenticated)
        return authentication_manager_error();

    context.authenticated = 0;
    context.active_session_id = 0;

    authentication_manager_get_statistics()->logout_count++;

    return 0;
}

authentication_manager_state_t
authentication_manager_get_state(void)
{
    return context.state;
}

authentication_manager_context_t*
authentication_manager_get_context(void)
{
    return &context;
}

int authentication_manager_is_authenticated(void)
{
    return context.authenticated ? 1 : 0;
}
