#include "audit_manager.h"
#include <stdlib.h>

#define AUDIT_MANAGER_DEFAULT_MAX_EVENTS 64U
#define AUDIT_MANAGER_FIRST_EVENT_ID 1ULL

static audit_manager_context_t context;
static audit_event_t *events = NULL;

static int audit_manager_error(void)
{
    context.state = AUDIT_MANAGER_ERROR;
    audit_manager_get_statistics()->error_count++;
    return -1;
}

static void audit_manager_release_events(void)
{
    free(events);
    events = NULL;
}

static audit_event_t *audit_manager_find_event(uint64_t event_id)
{
    uint32_t index;

    if (events == NULL || event_id == 0)
    {
        return NULL;
    }

    for (index = 0; index < context.max_events; ++index)
    {
        if (events[index].event_id == event_id)
        {
            return &events[index];
        }
    }

    return NULL;
}

int audit_manager_init(void)
{
    if (context.shutdown)
    {
        return audit_manager_error();
    }

    if (context.initialized)
    {
        return 0;
    }

    events = calloc(
        AUDIT_MANAGER_DEFAULT_MAX_EVENTS,
        sizeof(audit_event_t)
    );

    if (events == NULL)
    {
        return audit_manager_error();
    }

    context.state = AUDIT_MANAGER_INITIALIZED;
    context.max_events = AUDIT_MANAGER_DEFAULT_MAX_EVENTS;
    context.active_events = 0;
    context.next_event_id = AUDIT_MANAGER_FIRST_EVENT_ID;
    context.initialized = 1;
    context.running = 0;
    context.shutdown = 0;

    audit_manager_statistics_init();

    return 0;
}

int audit_manager_start(void)
{
    if (!context.initialized || context.shutdown)
    {
        return audit_manager_error();
    }

    if (context.running)
    {
        return 0;
    }

    context.running = 1;
    context.state = AUDIT_MANAGER_RUNNING;

    audit_manager_get_statistics()->start_count++;

    return 0;
}

int audit_manager_stop(void)
{
    if (!context.initialized || context.shutdown)
    {
        return audit_manager_error();
    }

    if (!context.running)
    {
        return 0;
    }

    context.running = 0;
    context.state = AUDIT_MANAGER_STOPPED;

    audit_manager_get_statistics()->stop_count++;

    return 0;
}

int audit_manager_shutdown(void)
{
    if (context.shutdown)
    {
        return 0;
    }

    if (context.running)
    {
        context.running = 0;
        audit_manager_get_statistics()->stop_count++;
    }

    audit_manager_release_events();

    context.active_events = 0;
    context.initialized = 0;
    context.shutdown = 1;
    context.state = AUDIT_MANAGER_STOPPED;

    return 0;
}

int audit_manager_set_max_events(uint32_t max_events)
{
    if (!context.initialized || context.shutdown)
    {
        return audit_manager_error();
    }

    if (context.running)
    {
        return audit_manager_error();
    }

    if (max_events == 0 ||
        max_events > AUDIT_MANAGER_DEFAULT_MAX_EVENTS)
    {
        return audit_manager_error();
    }

    context.max_events = max_events;

    return 0;
}

int audit_manager_record_event(
    audit_event_type_t type,
    uint32_t user_id,
    uint32_t session_id,
    uint32_t code,
    uint64_t timestamp,
    uint64_t *event_id
)
{
    uint32_t index;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        event_id == NULL)
    {
        return audit_manager_error();
    }

    if (type < AUDIT_EVENT_INFO ||
        type > AUDIT_EVENT_ERROR)
    {
        return audit_manager_error();
    }

    if (context.active_events >= context.max_events)
    {
        return audit_manager_error();
    }

    for (index = 0; index < context.max_events; ++index)
    {
        if (events[index].event_id == 0)
        {
            events[index].event_id = context.next_event_id++;
            events[index].type = type;
            events[index].user_id = user_id;
            events[index].session_id = session_id;
            events[index].code = code;
            events[index].timestamp = timestamp;

            context.active_events++;

            *event_id = events[index].event_id;

            audit_manager_get_statistics()->event_record_count++;

            return 0;
        }
    }

    return audit_manager_error();
}

int audit_manager_get_event(
    uint64_t event_id,
    audit_event_t *event
)
{
    audit_event_t *source;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        event == NULL ||
        event_id == 0)
    {
        return audit_manager_error();
    }

    source = audit_manager_find_event(event_id);

    if (source == NULL)
    {
        return audit_manager_error();
    }

    *event = *source;

    audit_manager_get_statistics()->event_read_count++;

    return 0;
}

int audit_manager_clear_event(uint64_t event_id)
{
    audit_event_t *event;

    if (!context.initialized ||
        context.shutdown ||
        !context.running ||
        event_id == 0)
    {
        return audit_manager_error();
    }

    event = audit_manager_find_event(event_id);

    if (event == NULL)
    {
        return audit_manager_error();
    }

    event->event_id = 0;
    event->type = AUDIT_EVENT_INFO;
    event->timestamp = 0;
    event->user_id = 0;
    event->session_id = 0;
    event->code = 0;

    context.active_events--;

    audit_manager_get_statistics()->event_clear_count++;

    return 0;
}

audit_manager_state_t audit_manager_get_state(void)
{
    return context.state;
}

audit_manager_context_t *audit_manager_get_context(void)
{
    return &context;
}
