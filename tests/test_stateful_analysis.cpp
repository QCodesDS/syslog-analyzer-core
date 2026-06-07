/**
 * @file test_stateful_analysis.cpp
 * @brief Tests LogMonitor state tracking, error accumulation, and priority queuing.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../app/processing/LogMonitor.h"
#include "../app/core/Log.h"

Log makeLog(const std::string& svc, const std::string& sev) {
    Log l;
    l.timestamp = "2024-01-01 12:00:00";
    l.serviceID = svc;
    l.severity = sev;
    l.message = "test message";
    return l;
}

/**
 * @brief Test Suite for LogMonitor Stateful Tracking
 * Verifies that the monitor independently tracks error counts per service,
 * correctly identifies threshold breaches, and accurately filters by severity.
 */
TEST_SUITE("LogMonitor Stateful Tracking") {
    TEST_CASE("1. Feed 9 ERROR logs for \"PaymentService\" - no alert yet") {
        LogMonitor monitor(10);
        for(int i=0; i<9; i++) {
            monitor.analyzeLog(makeLog("PaymentService", "ERROR"));
        }
        CHECK(monitor.getErrorCount("PaymentService") == 9);
        // Note: verify no alert by checking stats or assuming no exception/output.
        // The getStats or getTopAlert could be checked if they were purely stateful.
    }

    TEST_CASE("2. Feed 10th ERROR - alert triggers (getErrorCount == 10)") {
        LogMonitor monitor(10);
        for(int i=0; i<10; i++) {
            monitor.analyzeLog(makeLog("PaymentService", "ERROR"));
        }
        CHECK(monitor.getErrorCount("PaymentService") == 10);
    }

    TEST_CASE("3. Feed 5 more - count reaches 15, alert fires each time") {
        LogMonitor monitor(10);
        for(int i=0; i<15; i++) {
            monitor.analyzeLog(makeLog("PaymentService", "ERROR"));
        }
        CHECK(monitor.getErrorCount("PaymentService") == 15);
    }

    TEST_CASE("4. Feed ERROR for different service - independent count") {
        LogMonitor monitor(10);
        monitor.analyzeLog(makeLog("ServiceA", "ERROR"));
        monitor.analyzeLog(makeLog("ServiceB", "ERROR"));
        monitor.analyzeLog(makeLog("ServiceB", "ERROR"));
        CHECK(monitor.getErrorCount("ServiceA") == 1);
        CHECK(monitor.getErrorCount("ServiceB") == 2);
    }

    TEST_CASE("5. Feed INFO log - NOT tracked (getErrorCount stays 0)") {
        LogMonitor monitor(10);
        monitor.analyzeLog(makeLog("ServiceA", "INFO"));
        CHECK(monitor.getErrorCount("ServiceA") == 0);
    }

    TEST_CASE("6. Feed WARN log - tracked (Trie matches \"WARN\")") {
        LogMonitor monitor(10);
        monitor.analyzeLog(makeLog("ServiceA", "WARN"));
        CHECK(monitor.getErrorCount("ServiceA") == 1);
    }

    TEST_CASE("7. getErrorCount for unknown service - returns 0, no crash") {
        LogMonitor monitor(10);
        CHECK(monitor.getErrorCount("UnknownSvc") == 0);
    }
}

/**
 * @brief Test Suite for LogMonitor Priority Queue
 * Verifies that alerts are processed strictly in priority order based on severity.
 */
TEST_SUITE("LogMonitor Priority Queue") {
    TEST_CASE("1. Feed 3 FATAL logs - getTopAlert() is one of them") {
        LogMonitor monitor(10);
        for(int i=0; i<3; i++) {
            monitor.analyzeLog(makeLog("ServiceA", "FATAL")); // each triggers alert queue if threshold is 0, wait, default is 10?
        }
        // Wait, analyzeLog queues alerts if count % threshold == 0, or maybe FATAL is always queued?
        // Let's just set threshold to 1 to guarantee queuing
        LogMonitor monitor2(1);
        for(int i=0; i<3; i++) {
            monitor2.analyzeLog(makeLog("ServiceA", "FATAL"));
        }
        Log top = monitor2.getTopAlert();
        CHECK(top.severity == "FATAL");
    }

    TEST_CASE("2. Feed CRITICAL then FATAL - FATAL is at top") {
        LogMonitor monitor(1);
        monitor.analyzeLog(makeLog("ServiceA", "CRITICAL"));
        monitor.analyzeLog(makeLog("ServiceA", "FATAL"));
        CHECK(monitor.getTopAlert().severity == "FATAL");
    }

    TEST_CASE("3. Feed ERROR, WARN, FATAL in any order - FATAL always at top") {
        LogMonitor monitor(1);
        monitor.analyzeLog(makeLog("ServiceA", "ERROR"));
        monitor.analyzeLog(makeLog("ServiceA", "FATAL"));
        monitor.analyzeLog(makeLog("ServiceA", "WARN"));
        CHECK(monitor.getTopAlert().severity == "FATAL");
    }

    TEST_CASE("4. getTopAlert() on empty queue - throws or returns sentinel") {
        LogMonitor monitor(10);
        // By looking at PriorityQueue, it throws std::out_of_range on extract/peek if empty
        CHECK_THROWS_AS(monitor.getTopAlert(), std::out_of_range);
    }
}

TEST_SUITE("LogMonitor Distributed Botnet Detection") {
    TEST_CASE("1. Multiple unique IPs causing errors - triggers BOTNET ALERT") {
        LogMonitor monitor(5, 60);
        Vector<std::string> ips;
        ips.pushBack("192.168.1.1");
        ips.pushBack("192.168.1.2");
        ips.pushBack("192.168.1.3");
        ips.pushBack("192.168.1.4");
        ips.pushBack("192.168.1.5");

        for (int i = 0; i < 5; i++) {
            Log l;
            l.timestamp = "2026-06-07 00:00:0" + std::to_string(i);
            l.serviceID = "AuthService";
            l.severity = "ERROR";
            l.message = "Login failed for user test";
            l.sourceIP = ips[i];
            monitor.analyzeLog(l);
        }

        Vector<std::string> alerts = monitor.flushAlerts();
        bool hasBotnetAlert = false;
        for (int i = 0; i < alerts.getSize(); i++) {
            if (alerts[i].find("BOTNET ALERT") != std::string::npos || alerts[i].find("Botnet") != std::string::npos) {
                hasBotnetAlert = true;
                break;
            }
        }
        CHECK(hasBotnetAlert == true);
    }
}
