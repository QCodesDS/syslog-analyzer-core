/**
 * @file test_app.cpp
 * @brief Tests all core application components (app/), including LogParser, LogMonitor, and integration flows.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../app/core/Log.h"
#include "../../app/core/LogParser.h"
#include "../../app/processing/LogMonitor.h"
#include "../doctest.h"

/**
 * @brief Hàm hỗ trợ tạo đối tượng Log nhanh chóng.
 * @param svc Tên dịch vụ (ServiceID).
 * @param sev Mức độ nghiêm trọng (Severity).
 * @return Log Đối tượng Log được khởi tạo.
 */
static Log makeLog(const std::string& svc, const std::string& sev) {
    Log l;
    l.timestamp = "2024-01-01 12:00:00";
    l.serviceID = svc;
    l.severity = sev;
    l.message = "test message";
    return l;
}

/**
 * @brief Hàm hỗ trợ đọc chuỗi log thô và đưa vào LogMonitor.
 * @param monitor Bộ giám sát LogMonitor.
 * @param line Chuỗi log thô.
 */
static void feedLine(LogMonitor& monitor, const std::string& line) {
    Log parsedLog;
    if (LogParser::parse(line, parsedLog)) {
        monitor.analyzeLog(parsedLog);
    }
}

/**
 * @brief Test Suite for LogParser
 */
TEST_SUITE("LogParser Pipeline") {
    TEST_CASE("1. Valid line: \"[2024-01-01 00:00:00] [PaymentService] [ERROR] DB timeout\" - all 4 fields correct") {
        Log out;
        bool result = LogParser::parse("[2024-01-01 00:00:00] [PaymentService] [ERROR] DB timeout", out);
        REQUIRE(result == true);
        CHECK(out.timestamp == "2024-01-01 00:00:00");
        CHECK(out.serviceID == "PaymentService");
        CHECK(out.severity == "ERROR");
        CHECK(out.message == "DB timeout");
    }

    TEST_CASE("2. Valid line where message contains brackets - only first 3 bracket pairs consumed") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [INFO] This [message] has [brackets]", out);
        REQUIRE(result == true);
        CHECK(out.message == "This [message] has [brackets]");
    }

    TEST_CASE("3. Empty message after severity - does not crash, message is empty") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [WARN]", out);
        REQUIRE(result == true);
        CHECK(out.message == "");
    }

    TEST_CASE("4. Malformed lines - returns false gracefully") {
        Log out;
        CHECK(LogParser::parse("2024-01-01 00:00:00] [svc] [WARN] hello", out) == false);
        CHECK(LogParser::parse("[time] [svc [WARN] hello", out) == false);
        CHECK(LogParser::parse("", out) == false);
    }

    TEST_CASE("5. Log Injection Prevention - replaces newlines with [NL]") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [ERROR] User admin\n[time] [svc] [INFO] Login success", out);
        REQUIRE(result == true);
        CHECK(out.message == "User admin [NL] [time] [svc] [INFO] Login success");
    }

    TEST_CASE("6. OOM Prevention - truncates log line to 8KB") {
        Log out;
        std::string hugeMessage(10000, 'A');
        std::string input = "[time] [svc] [ERROR] " + hugeMessage;
        bool result = LogParser::parse(input, out);
        REQUIRE(result == true);
        CHECK(out.message.size() <= 8192);
    }

    TEST_CASE("7. De-obfuscation - decodes Hex/URL and Base64 encoded values in message/severity") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [\\x45\\x52\\x52\\x4f\\x52] Login failed for user QWNjaWRlbnQ=", out);
        REQUIRE(result == true);
        CHECK(out.severity == "ERROR");
        CHECK(out.message.find("Accident") != std::string::npos);
        CHECK(out.username == "Accident");
    }

    TEST_CASE("8. De-obfuscation - decodes URL-encoding %45%52%52%4f%52 -> ERROR") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [%45%52%52%4f%52] message", out);
        REQUIRE(result == true);
        CHECK(out.severity == "ERROR");
    }
}

/**
 * @brief Test Suite for LogMonitor Stateful Tracking
 */
TEST_SUITE("LogMonitor Stateful Tracking") {
    TEST_CASE("1. Feed 9 ERROR logs for \"PaymentService\" - no alert yet") {
        LogMonitor monitor(10);
        for (int i = 0; i < 9; i++) {
            monitor.analyzeLog(makeLog("PaymentService", "ERROR"));
        }
        CHECK(monitor.getErrorCount("PaymentService") == 9);
    }

    TEST_CASE("2. Feed 10th ERROR - alert triggers (getErrorCount == 10)") {
        LogMonitor monitor(10);
        for (int i = 0; i < 10; i++) {
            monitor.analyzeLog(makeLog("PaymentService", "ERROR"));
        }
        CHECK(monitor.getErrorCount("PaymentService") == 10);
    }

    TEST_CASE("3. Feed ERROR for different service - independent count") {
        LogMonitor monitor(10);
        monitor.analyzeLog(makeLog("ServiceA", "ERROR"));
        monitor.analyzeLog(makeLog("ServiceB", "ERROR"));
        monitor.analyzeLog(makeLog("ServiceB", "ERROR"));
        CHECK(monitor.getErrorCount("ServiceA") == 1);
        CHECK(monitor.getErrorCount("ServiceB") == 2);
    }

    TEST_CASE("4. Feed INFO log - NOT tracked") {
        LogMonitor monitor(10);
        monitor.analyzeLog(makeLog("ServiceA", "INFO"));
        CHECK(monitor.getErrorCount("ServiceA") == 0);
    }

    TEST_CASE("5. Default keywords loaded correctly in Trie") {
        LogMonitor monitor(1);
        Log l;
        l.timestamp = "2026-06-07 12:00:00";
        l.message = "m";

        const char* keywords[] = {"WARN", "ERROR", "FATAL", "CRITICAL", "TIMEOUT", "EXCEPTION"};
        for (const char* kw : keywords) {
            l.serviceID = std::string(kw) + "_svc";
            l.severity = kw;
            monitor.analyzeLog(l);
            CHECK(monitor.getErrorCount(l.serviceID) == 1);
        }
    }
}

/**
 * @brief Test Suite for LogMonitor Priority Queue
 */
TEST_SUITE("LogMonitor Priority Queue") {
    TEST_CASE("1. Feed CRITICAL then FATAL - FATAL is at top") {
        LogMonitor monitor(1);
        monitor.analyzeLog(makeLog("ServiceA", "CRITICAL"));
        monitor.analyzeLog(makeLog("ServiceA", "FATAL"));
        CHECK(monitor.getTopAlert().severity == "FATAL");
    }

    TEST_CASE("2. Feed ERROR, WARN, FATAL in any order - FATAL always at top") {
        LogMonitor monitor(1);
        monitor.analyzeLog(makeLog("ServiceA", "ERROR"));
        monitor.analyzeLog(makeLog("ServiceA", "FATAL"));
        monitor.analyzeLog(makeLog("ServiceA", "WARN"));
        CHECK(monitor.getTopAlert().severity == "FATAL");
    }

    TEST_CASE("3. getTopAlert() on empty queue - throws out_of_range") {
        LogMonitor monitor(10);
        CHECK_THROWS_AS(monitor.getTopAlert(), std::out_of_range);
    }
}

/**
 * @brief Test Suite for LogMonitor Sliding Window
 */
TEST_SUITE("LogMonitor Sliding Window & Botnet Detection") {
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

    TEST_CASE("2. Sliding window prunes old incidents for Malicious IP rank") {
        LogMonitor monitor(10, 5);  // 5-second window

        Log l1 = makeLog("Svc", "ERROR");
        l1.timestamp = "2026-06-07 00:00:00";
        l1.sourceIP = "192.168.1.100";

        Log l2 = makeLog("Svc", "ERROR");
        l2.timestamp = "2026-06-07 00:00:06";  // 6 seconds later (out of window)
        l2.sourceIP = "192.168.1.100";

        monitor.analyzeLog(l1);
        monitor.analyzeLog(l2);

        std::string topIP;
        int topIPCount = 0;
        monitor.findTopMaliciousIP(topIP, topIPCount);

        CHECK(topIP == "192.168.1.100");
        CHECK(topIPCount == 1);  // The first incident should be pruned!
    }
}

/**
 * @brief Test Suite for Full Pipeline Integration
 */
TEST_SUITE("Full Pipeline Integration") {
    TEST_CASE("1. Feed 10 valid ERROR lines for same service - parse succeeds, alert triggers, count == 10") {
        LogMonitor monitor(10);
        for (int i = 0; i < 10; i++) {
            feedLine(monitor, "[2024-01-01 10:00:00] [AuthService] [ERROR] connection lost");
        }
        CHECK(monitor.getErrorCount("AuthService") == 10);
    }

    TEST_CASE("2. Feed 5 malformed lines - parse returns false, LogMonitor not called, no crash") {
        LogMonitor monitor(10);
        for (int i = 0; i < 5; i++) {
            feedLine(monitor, "This is not a valid log line format");
        }
        CHECK(monitor.getErrorCount("AuthService") == 0);
    }

    TEST_CASE("3. Feed mix: 5 ERROR + 3 INFO + 2 FATAL - only ERROR and FATAL counted, FATAL in queue") {
        LogMonitor monitor(1);
        for (int i = 0; i < 5; i++)
            feedLine(monitor, "[time] [Svc] [ERROR] msg");
        for (int i = 0; i < 3; i++)
            feedLine(monitor, "[time] [Svc] [INFO] msg");
        for (int i = 0; i < 2; i++)
            feedLine(monitor, "[time] [Svc] [FATAL] msg");

        CHECK(monitor.getErrorCount("Svc") == 7);
        Log top = monitor.getTopAlert();
        CHECK(top.severity == "FATAL");
    }
}
