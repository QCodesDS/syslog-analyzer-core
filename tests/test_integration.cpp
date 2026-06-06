/**
 * @file test_integration.cpp
 * @brief Tests the end-to-end flow from string parsing to stateful alerting.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../app/core/LogParser.h"
#include "../app/processing/LogMonitor.h"

void feedLine(LogMonitor& monitor, const std::string& line) {
    Log parsedLog;
    if (LogParser::parse(line, parsedLog)) {
        monitor.analyzeLog(parsedLog);
    }
}

/**
 * @brief Test Suite for Full Pipeline Integration
 * Verifies that raw logs are correctly parsed and then accurately tracked
 * and queued by the LogMonitor, handling malformed lines gracefully.
 */
TEST_SUITE("Full Pipeline Integration") {
    TEST_CASE("1. Feed 10 valid ERROR lines for same service - parse succeeds, alert triggers, count == 10") {
        LogMonitor monitor(10);
        for(int i=0; i<10; i++) {
            feedLine(monitor, "[2024-01-01 10:00:00] [AuthService] [ERROR] connection lost");
        }
        CHECK(monitor.getErrorCount("AuthService") == 10);
    }

    TEST_CASE("2. Feed 5 malformed lines - parse returns false, LogMonitor not called, no crash") {
        LogMonitor monitor(10);
        for(int i=0; i<5; i++) {
            feedLine(monitor, "This is not a valid log line format");
        }
        // Assuming getting an unknown service returns 0
        CHECK(monitor.getErrorCount("AuthService") == 0);
    }

    TEST_CASE("3. Feed mix: 5 ERROR + 3 INFO + 2 FATAL - only ERROR and FATAL counted, FATAL in queue") {
        LogMonitor monitor(1); // Set threshold to 1 so they queue immediately
        for(int i=0; i<5; i++) feedLine(monitor, "[time] [Svc] [ERROR] msg");
        for(int i=0; i<3; i++) feedLine(monitor, "[time] [Svc] [INFO] msg");
        for(int i=0; i<2; i++) feedLine(monitor, "[time] [Svc] [FATAL] msg");
        
        CHECK(monitor.getErrorCount("Svc") == 7); // 5 ERROR + 2 FATAL
        // FATAL should be at the top of the priority queue
        Log top = monitor.getTopAlert();
        CHECK(top.severity == "FATAL");
    }

    TEST_CASE("4. Feed 50 lines for 5 different services (10 each) - each hits threshold independently") {
        LogMonitor monitor(10);
        for(int i=0; i<10; i++) {
            feedLine(monitor, "[time] [Svc1] [ERROR] msg");
            feedLine(monitor, "[time] [Svc2] [ERROR] msg");
            feedLine(monitor, "[time] [Svc3] [ERROR] msg");
            feedLine(monitor, "[time] [Svc4] [ERROR] msg");
            feedLine(monitor, "[time] [Svc5] [ERROR] msg");
        }
        CHECK(monitor.getErrorCount("Svc1") == 10);
        CHECK(monitor.getErrorCount("Svc2") == 10);
        CHECK(monitor.getErrorCount("Svc3") == 10);
        CHECK(monitor.getErrorCount("Svc4") == 10);
        CHECK(monitor.getErrorCount("Svc5") == 10);
        
        Vector<std::string> stats = monitor.getStats();
        // Stats format contains service names and their counts
        CHECK(stats.getSize() == 5);
    }

    TEST_CASE("5. Feed FATAL, CRITICAL, ERROR in reverse priority order - getTopAlert() returns FATAL") {
        LogMonitor monitor(1);
        feedLine(monitor, "[time] [Svc] [ERROR] msg");
        feedLine(monitor, "[time] [Svc] [CRITICAL] msg");
        feedLine(monitor, "[time] [Svc] [FATAL] msg");
        
        Log top = monitor.getTopAlert();
        CHECK(top.severity == "FATAL");
    }
}
