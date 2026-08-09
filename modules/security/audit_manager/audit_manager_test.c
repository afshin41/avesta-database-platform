#include <stdio.h>
#include <stdint.h>

#include "audit_manager.h"
#include "audit_manager_statistics.h"

static int failures = 0;

static void check(int condition, const char *name)
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

int main(void)
{
    uint64_t event1;
    uint64_t event2;
    uint64_t event3;
    audit_event_t event;
    audit_manager_statistics_t *statistics;

    printf("===============================\n");
    printf(" Audit Manager Test\n");
    printf(" Edge Case Verification\n");
    printf("===============================\n");

    check(
        audit_manager_start() != 0,
        "Start Before Initialize"
    );

    check(
        audit_manager_record_event(
            AUDIT_EVENT_INFO, 1, 1, 100, 1000, &event1
        ) != 0,
        "Record Before Initialize"
    );

    check(
        audit_manager_get_event(1, &event) != 0,
        "Read Before Initialize"
    );

    check(
        audit_manager_init() == 0,
        "Initialize"
    );

    check(
        audit_manager_get_state() ==
        AUDIT_MANAGER_INITIALIZED,
        "State INITIALIZED"
    );

    check(
        audit_manager_set_max_events(2) == 0,
        "Configure Maximum Events"
    );

    check(
        audit_manager_start() == 0,
        "Start"
    );

    check(
        audit_manager_get_state() ==
        AUDIT_MANAGER_RUNNING,
        "State RUNNING"
    );

    check(
        audit_manager_start() == 0,
        "Repeated Start"
    );

    check(
        audit_manager_record_event(
            AUDIT_EVENT_INFO,
            10,
            100,
            1001,
            10000,
            &event1
        ) == 0,
        "Record Event 1"
    );

    check(
        audit_manager_record_event(
            AUDIT_EVENT_SECURITY,
            20,
            200,
            2002,
            20000,
            &event2
        ) == 0,
        "Record Event 2"
    );

    check(
        audit_manager_get_context()->active_events == 2,
        "Active Event Count"
    );

    check(
        audit_manager_record_event(
            AUDIT_EVENT_ERROR,
            30,
            300,
            3003,
            30000,
            &event3
        ) != 0,
        "Reject Event Above Limit"
    );

    check(
        audit_manager_get_event(event1, &event) == 0,
        "Read Event 1"
    );

    check(
        event.event_id == event1 &&
        event.type == AUDIT_EVENT_INFO &&
        event.user_id == 10 &&
        event.session_id == 100 &&
        event.code == 1001 &&
        event.timestamp == 10000,
        "Validate Event 1"
    );

    check(
        audit_manager_get_event(999999, &event) != 0,
        "Reject Invalid Event"
    );

    check(
        audit_manager_clear_event(event1) == 0,
        "Clear Event 1"
    );

    check(
        audit_manager_get_context()->active_events == 1,
        "Active Event Count After Clear"
    );

    check(
        audit_manager_clear_event(event1) != 0,
        "Reject Repeated Clear"
    );

    check(
        audit_manager_record_event(
            AUDIT_EVENT_WARNING,
            40,
            400,
            4004,
            40000,
            &event3
        ) == 0,
        "Reuse Event Slot"
    );

    check(
        audit_manager_stop() == 0,
        "Stop"
    );

    check(
        audit_manager_get_state() ==
        AUDIT_MANAGER_STOPPED,
        "State STOPPED"
    );

    check(
        audit_manager_record_event(
            AUDIT_EVENT_INFO,
            50,
            500,
            5005,
            50000,
            &event3
        ) != 0,
        "Reject Record After Stop"
    );

    check(
        audit_manager_get_event(event2, &event) != 0,
        "Reject Read After Stop"
    );

    check(
        audit_manager_stop() == 0,
        "Repeated Stop"
    );

    check(
        audit_manager_shutdown() == 0,
        "Shutdown"
    );

    check(
        audit_manager_get_context()->shutdown == 1,
        "Shutdown Flag"
    );

    check(
        audit_manager_shutdown() == 0,
        "Repeated Shutdown"
    );

    check(
        audit_manager_init() != 0,
        "Initialize After Shutdown"
    );

    check(
        audit_manager_start() != 0,
        "Start After Shutdown"
    );

    check(
        audit_manager_clear_event(event2) != 0,
        "Clear After Shutdown"
    );

    check(
        audit_manager_get_state() ==
        AUDIT_MANAGER_ERROR,
        "Final Error State"
    );

    statistics = audit_manager_get_statistics();

    printf(
        "Actual Error Count: %llu\n",
        (unsigned long long)statistics->error_count
    );

    check(
        statistics->start_count == 1,
        "Start Statistics"
    );

    check(
        statistics->stop_count == 1,
        "Stop Statistics"
    );

    check(
        statistics->event_record_count == 3,
        "Event Record Statistics"
    );

    check(
        statistics->event_read_count == 1,
        "Event Read Statistics"
    );

    check(
        statistics->event_clear_count == 1,
        "Event Clear Statistics"
    );

    check(
        statistics->error_count == 8,
        "Error Statistics"
    );

    if (failures == 0)
    {
        printf("\nRESULT: ALL EDGE CASES PASSED\n");
        return 0;
    }

    printf(
        "\nRESULT: FAILED (%d failures)\n",
        failures
    );

    return 1;
}
