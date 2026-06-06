/**
 * @file LogParser.h
 * @brief Trình phân tích cú pháp chuỗi log thô thành đối tượng Log có cấu trúc.
 * 
 * Định dạng log đầu vào được kỳ vọng: [timestamp] [serviceID] [severity] message
 */

#ifndef LOGPARSER_H
#define LOGPARSER_H

#include "Log.h"

/**
 * @class LogParser
 * @brief Cung cấp các phương thức tĩnh để phân tích cú pháp dòng log.
 */
class LogParser {
public:
    /**
     * @brief Phân tích một dòng log thô thành cấu trúc Log.
     * 
     * @param line Chuỗi log thô đọc từ file.
     * @param out Biến tham chiếu lưu đối tượng Log sau khi phân tích thành công.
     * @return true Nếu phân tích thành công.
     * @return false Nếu chuỗi log không đúng định dạng.
     */
    static bool parse(const std::string& line, Log& out);
};

#endif  // LOGPARSER_H