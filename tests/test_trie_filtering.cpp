/**
 * @file test_trie_filtering.cpp
 * @brief Tests the Trie data structure for exact matches and prefix searches.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../lib/Trie.hpp"
#include "../app/processing/LogMonitor.h" // To test default keywords

/**
 * @brief Test Suite for Trie
 * Verifies insertion, exact search, prefix search, and clearing, including
 * case sensitivity and empty string handling.
 */
TEST_SUITE("Trie") {
    TEST_CASE("1. insert \"ERROR\", \"FATAL\", \"TIMEOUT\" - search() true for all") {
        Trie t;
        t.insert("ERROR");
        t.insert("FATAL");
        t.insert("TIMEOUT");
        CHECK(t.search("ERROR") == true);
        CHECK(t.search("FATAL") == true);
        CHECK(t.search("TIMEOUT") == true);
    }

    TEST_CASE("2. search \"WARN\" (not inserted) - false") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.search("WARN") == false);
    }

    TEST_CASE("3. startsWith(\"ERR\") - true (prefix of \"ERROR\")") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.startsWith("ERR") == true);
    }

    TEST_CASE("4. startsWith(\"XYZ\") - false") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.startsWith("XYZ") == false);
    }

    TEST_CASE("5. startsWith(\"ERROR\") - true (exact match is also a prefix)") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.startsWith("ERROR") == true);
    }

    TEST_CASE("6. insert \"ERR\", search \"ERROR\" - false, but startsWith(\"ERR\") - true") {
        Trie t;
        t.insert("ERR");
        CHECK(t.search("ERROR") == false);
        CHECK(t.startsWith("ERR") == true);
    }

    TEST_CASE("7. clear() - search returns false for all previously inserted") {
        Trie t;
        t.insert("ERROR");
        t.insert("FATAL");
        t.clear();
        CHECK(t.search("ERROR") == false);
        CHECK(t.search("FATAL") == false);
    }

    TEST_CASE("8. Empty string insert - no crash") {
        Trie t;
        t.insert("");
        CHECK(t.search("") == true);
    }

    TEST_CASE("9. Case sensitivity: \"error\" != \"ERROR\" - search(\"error\") false") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.search("error") == false);
    }

    TEST_CASE("10. All 6 default keywords pre-loaded in LogMonitor's Trie") {
        // verify via LogMonitor.analyzeLog() behavior
        LogMonitor monitor(1);
        Log l; l.timestamp="t"; l.message="m";
        
        // Ensure "WARN", "ERROR", "FATAL", "CRITICAL", "TIMEOUT", "EXCEPTION" are tracked.
        const char* keywords[] = {"WARN", "ERROR", "FATAL", "CRITICAL", "TIMEOUT", "EXCEPTION"};
        for(const char* kw : keywords) {
            l.serviceID = std::string(kw) + "_svc";
            l.severity = kw;
            monitor.analyzeLog(l);
            CHECK(monitor.getErrorCount(l.serviceID) == 1);
        }
    }
}
