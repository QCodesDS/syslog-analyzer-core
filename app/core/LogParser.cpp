/**
 * @file LogParser.cpp
 * @brief Cài đặt của lớp LogParser để phân tích cú pháp log.
 */

#include "LogParser.h"

bool LogParser::parse(const std::string& line, Log& out) {
    // Chiều dài dòng tối thiểu hợp lệ: [x] [x] [x] x (7 ký tự)
    if (line.size() < 7 || line[0] != '[') {
        return false;
    }

    size_t pos = 0;

    // --- Trích xuất nhãn thời gian: nằm trong cặp ngoặc [...] đầu tiên ---
    size_t open1 = line.find('[', pos);
    size_t close1 = line.find(']', open1);
    if (open1 == std::string::npos || close1 == std::string::npos) {
        return false;
    }
    out.timestamp = line.substr(open1 + 1, close1 - open1 - 1);
    pos = close1 + 1;

    // --- Trích xuất tên dịch vụ: nằm trong cặp ngoặc [...] thứ hai ---
    size_t open2 = line.find('[', pos);
    size_t close2 = line.find(']', open2);
    if (open2 == std::string::npos || close2 == std::string::npos) {
        return false;
    }
    out.serviceID = line.substr(open2 + 1, close2 - open2 - 1);
    pos = close2 + 1;

    // --- Trích xuất mức độ nghiêm trọng: nằm trong cặp ngoặc [...] thứ ba ---
    size_t open3 = line.find('[', pos);
    size_t close3 = line.find(']', open3);
    if (open3 == std::string::npos || close3 == std::string::npos) {
        return false;
    }
    out.severity = line.substr(open3 + 1, close3 - open3 - 1);
    pos = close3 + 1;

    // --- Trích xuất thông điệp: phần còn lại sau dấu ] thứ ba (bỏ qua khoảng trắng đầu) ---
    if (pos < line.size() && line[pos] == ' ') {
        pos++;
    }
    out.message = line.substr(pos);

    return true;
}
