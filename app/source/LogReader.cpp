#include "LogReader.h"

#include <chrono>
#include <iostream>
#include <thread>

LogReader::LogReader(const std::string& filename) {
    file.open(filename);
    if (file.is_open()) {
        lastPos = 0;
    }
}

LogReader::~LogReader() {
    close();
}

Vector<std::string> LogReader::readBatch(int batchSize) {
    Vector<std::string> batch;
    if (!file.is_open()) {
        return batch;
    }

    file.clear();  // Clear EOF flag
    file.seekg(lastPos);

    std::string line;
    int count = 0;
    while (count < batchSize && std::getline(file, line)) {
        batch.pushBack(line);
        count++;
    }

    if (batch.getSize() > 0) {
        lastPos = file.tellg();
    }

    return batch;
}

bool LogReader::hasNext() const {
    return file.is_open() && file.good();
}

void LogReader::close() {
    if (file.is_open()) {
        file.close();
    }
}
