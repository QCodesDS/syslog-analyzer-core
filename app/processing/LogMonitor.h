#ifndef LOGMONITOR_H
#define LOGMONITOR_H

#include <stdexcept>
#include <string>

#include "../../lib/HashTable.hpp"
#include "../../lib/PriorityQueue.hpp"
#include "../../lib/Trie.hpp"
#include "../../lib/Vector.hpp"
#include "../core/Log.h"

struct LogSeverityComparator {
    int severityRank(const std::string& s) const {
        if (s == "FATAL")
            return 4;
        if (s == "CRITICAL")
            return 3;
        if (s == "ERROR")
            return 2;
        if (s == "WARN")
            return 1;
        return 0;
    }

    // Returns true if a has lower priority than b
    bool operator()(const Log& a, const Log& b) const { return severityRank(a.severity) < severityRank(b.severity); }
};

class LogMonitor {
private:
    HashTable<std::string, int> errorCount;
    Trie keywordFilter;
    PriorityQueue<Log, LogSeverityComparator> alertQueue;
    int threshold;

public:
    LogMonitor(int alertThreshold = 10);

    void analyzeLog(const Log& log);
    Log getTopAlert();
    int getErrorCount(std::string serviceID);
    Vector<std::string> getStats();
};

#endif  // LOGMONITOR_H