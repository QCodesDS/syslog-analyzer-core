#ifndef LOGREADER_H
#define LOGREADER_H

#include <fstream>
#include <string>

#include "../../lib/Vector.hpp"

class LogReader {
private:
    std::ifstream file;
    std::streampos lastPos;

public:
    // Open file once in constructor, keep ifstream open
    LogReader(const std::string& filename);
    ~LogReader();

    // readBatch(int batchSize = 200) returns Vector<std::string> of new lines
    Vector<std::string> readBatch(int batchSize = 200);

    // returns true if file is open and healthy
    bool hasNext() const;

    // closes the file cleanly
    void close();
};

#endif  // LOGREADER_H