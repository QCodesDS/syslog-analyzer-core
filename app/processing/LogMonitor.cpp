#include "LogMonitor.h"

#include "../output/AlertNotifier.h"

LogMonitor::LogMonitor(int alertThreshold) : threshold(alertThreshold) {
    keywordFilter.insert("ERROR");
    keywordFilter.insert("FATAL");
    keywordFilter.insert("CRITICAL");
    keywordFilter.insert("TIMEOUT");
    keywordFilter.insert("EXCEPTION");
    keywordFilter.insert("WARN");
}

void LogMonitor::analyzeLog(const Log& log) {
    // 1. Use Trie.startsWith(log.severity) to check if severity is tracked
    if (keywordFilter.startsWith(log.severity)) {
        // 2. Increment count in HashTable using find()
        int* ptr = errorCount.find(log.serviceID);
        int current = ptr ? *ptr : 0;
        int newCount = current + 1;
        errorCount.insert(Pair<std::string, int>(log.serviceID, newCount));

        // 3. If severity == "FATAL" or "CRITICAL", push log into PriorityQueue
        if (log.severity == "FATAL" || log.severity == "CRITICAL") {
            alertQueue.insert(log);
            // Optionally, also notify immediately for critical alerts
            AlertNotifier::notifyCritical(log);
        }

        // 4. If count >= THRESHOLD, call AlertNotifier
        if (newCount >= threshold) {
            AlertNotifier::notify(log, newCount);
        }
    }
}

Log LogMonitor::getTopAlert() {
    if (alertQueue.empty()) {
        throw std::out_of_range("No alerts in the priority queue.");
    }
    return alertQueue.peek();
}

int LogMonitor::getErrorCount(std::string serviceID) {
    int* ptr = errorCount.find(serviceID);
    return ptr ? *ptr : 0;
}

Vector<std::string> LogMonitor::getStats() {
    Vector<std::string> stats;
    Vector<Pair<std::string, int>> items = errorCount.lnr();

    for (int i = 0; i < items.getSize(); i++) {
        stats.pushBack(items[i].key + ": " + std::to_string(items[i].value) + " errors");
    }
    return stats;
}
