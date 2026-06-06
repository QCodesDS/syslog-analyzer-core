/**
 * @file Log.h
 * @brief Defines the Log struct representing a single parsed log entry.
 */

#ifndef LOG_H
#define LOG_H

#include <string>

struct Log {
    std::string timestamp;
    std::string serviceID;
    std::string severity;
    std::string message;

    // Returns a formatted string for display
    std::string toString() const { return "[" + timestamp + "] " + serviceID + " [" + severity + "] " + message; }
};

#endif  // LOG_H