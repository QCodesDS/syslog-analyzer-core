#include "LogParser.h"

// Parses: [timestamp] [serviceID] [severity] message
bool LogParser::parse(const std::string& line, Log& out) {
    // Minimum valid line: [x] [x] [x] x
    if (line.size() < 7 || line[0] != '[')
        return false;

    size_t pos = 0;

    // --- Extract timestamp: first [...] ---
    size_t open1 = line.find('[', pos);
    size_t close1 = line.find(']', open1);
    if (open1 == std::string::npos || close1 == std::string::npos)
        return false;
    out.timestamp = line.substr(open1 + 1, close1 - open1 - 1);
    pos = close1 + 1;

    // --- Extract serviceID: second [...] ---
    size_t open2 = line.find('[', pos);
    size_t close2 = line.find(']', open2);
    if (open2 == std::string::npos || close2 == std::string::npos)
        return false;
    out.serviceID = line.substr(open2 + 1, close2 - open2 - 1);
    pos = close2 + 1;

    // --- Extract severity: third [...] ---
    size_t open3 = line.find('[', pos);
    size_t close3 = line.find(']', open3);
    if (open3 == std::string::npos || close3 == std::string::npos)
        return false;
    out.severity = line.substr(open3 + 1, close3 - open3 - 1);
    pos = close3 + 1;

    // --- Extract message: everything after the third ] (skip leading space) ---
    if (pos < line.size() && line[pos] == ' ')
        pos++;
    out.message = line.substr(pos);

    return true;
}
