/**
 * @file LogParser.h
 * @brief Parses raw log strings into structured Log objects.
 * Expected format: [timestamp] [serviceID] [severity] message
 */

#ifndef LOGPARSER_H
#define LOGPARSER_H

#include <string>

#include "Log.h"

class LogParser {
public:
    // Parses a raw log line into a Log struct. Returns true on success.
    static bool parse(const std::string& line, Log& out);
};

#endif  // LOGPARSER_H