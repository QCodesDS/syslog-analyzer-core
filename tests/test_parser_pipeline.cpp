/**
 * @file test_parser_pipeline.cpp
 * @brief Tests the LogParser to ensure it extracts fields correctly and handles edge cases.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../app/core/LogParser.h"
#include "../app/core/Log.h"

/**
 * @brief Test Suite for LogParser
 * Verifies successful parsing of valid format, correct bracket consumption,
 * and graceful rejection of malformed lines.
 */
TEST_SUITE("LogParser") {
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

    TEST_CASE("4. Malformed: no opening bracket - returns false") {
        Log out;
        bool result = LogParser::parse("2024-01-01 00:00:00] [svc] [WARN] hello", out);
        CHECK(result == false);
    }

    TEST_CASE("5. Malformed: unclosed bracket - returns false") {
        Log out;
        bool result = LogParser::parse("[time] [svc [WARN] hello", out);
        CHECK(result == false);
    }

    TEST_CASE("6. Empty string - returns false") {
        Log out;
        bool result = LogParser::parse("", out);
        CHECK(result == false);
    }

    TEST_CASE("7. Extra whitespace between fields - still parses correctly") {
        Log out;
        // Single extra space between fields: parser finds next '[' so gaps are fine
        bool result = LogParser::parse("[time] [svc] [INFO] message here", out);
        REQUIRE(result == true);
        CHECK(out.timestamp == "time");
        CHECK(out.serviceID == "svc");
        CHECK(out.severity == "INFO");
        CHECK(out.message == "message here");
    }

    TEST_CASE("8. Message with special characters (!@#$%) - captured as-is") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [ERROR] User password!@#$% failed", out);
        REQUIRE(result == true);
        CHECK(out.message == "User password!@#$% failed");
    }

    TEST_CASE("9. Log Injection Prevention - replaces newlines with [NL]") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [ERROR] User admin\n[time] [svc] [INFO] Login success", out);
        REQUIRE(result == true);
        CHECK(out.message.find("[NL]") != std::string::npos);
        CHECK(out.message == "User admin [NL] [time] [svc] [INFO] Login success");
    }

    TEST_CASE("10. OOM Prevention - truncates log line to 8KB") {
        Log out;
        std::string hugeMessage(10000, 'A');
        std::string input = "[time] [svc] [ERROR] " + hugeMessage;
        bool result = LogParser::parse(input, out);
        REQUIRE(result == true);
        CHECK(out.message.size() <= 8192);
    }

    TEST_CASE("11. De-obfuscation - decodes Hex/URL and Base64 encoded values in message/severity") {
        Log out;
        // Hex \x45\x52\x52\x4f\x52 -> ERROR
        // Base64 QWNjaWRlbnQ= -> Accident
        bool result = LogParser::parse("[time] [svc] [\\x45\\x52\\x52\\x4f\\x52] Login failed for user QWNjaWRlbnQ=", out);
        REQUIRE(result == true);
        CHECK(out.severity == "ERROR");
        CHECK(out.message.find("Accident") != std::string::npos);
        CHECK(out.username == "Accident"); // The username should extract the decoded username
    }

    TEST_CASE("12. De-obfuscation - decodes URL-encoding %45%52%52%4f%52 -> ERROR") {
        Log out;
        bool result = LogParser::parse("[time] [svc] [%45%52%52%4f%52] message", out);
        REQUIRE(result == true);
        CHECK(out.severity == "ERROR");
    }
}
