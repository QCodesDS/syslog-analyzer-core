/**
 * @file LogReader.cpp
 * @brief Cài đặt các phương thức của lớp LogReader.
 */

#include "LogReader.h"

LogReader::LogReader(const std::string& filename) {
    file.open(filename);
    if (file.is_open()) {
        lastPos = 0;  // Đặt vị trí bắt đầu tại đầu file
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

    file.clear();         // Xóa cờ lỗi/EOF để có thể đọc tiếp nếu có dữ liệu mới được thêm vào file
    file.seekg(lastPos);  // Di chuyển con trỏ tới vị trí đọc cuối cùng

    std::string line;
    int count = 0;

    // Đọc từng dòng cho đến khi đạt kích thước lô hoặc hết dữ liệu hiện tại
    while (count < batchSize && std::getline(file, line)) {
        batch.pushBack(line);
        count++;
    }

    // Nếu có dữ liệu mới được đọc, cập nhật lại vị trí con trỏ cuối
    if (batch.getSize() > 0) {
        if (file.eof()) {
            file.clear();
        }
        std::streampos pos = file.tellg();
        if (pos != std::streampos(-1)) {
            lastPos = pos;
        }
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
