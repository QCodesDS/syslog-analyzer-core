/**
 * @file Log.h
 * @brief Định nghĩa cấu trúc Log lưu trữ dữ liệu của một mục log đã được phân tích.
 */

#ifndef LOG_H
#define LOG_H

#include <string>

/**
 * @struct Log
 * @brief Đại diện cho một mục log hệ thống đã được phân tích.
 */
struct Log {
    std::string timestamp; /**< @brief Nhãn thời gian của log (vd: "2023-10-12 15:30:45"). */
    std::string serviceID; /**< @brief Tên dịch vụ sinh ra log (vd: "DBService"). */
    std::string severity;  /**< @brief Mức độ nghiêm trọng của log (vd: "INFO", "ERROR", "CRITICAL"). */
    std::string message;   /**< @brief Nội dung thông điệp log chi tiết. */
    std::string sourceIP;  /**< @brief Địa chỉ IP nguồn trích xuất từ thông điệp log (nếu có). */
    std::string username;  /**< @brief Tên người dùng trích xuất từ thông điệp log (nếu có). */

    /**
     * @brief Trả về chuỗi định dạng của log để hiển thị trực tiếp.
     * @return std::string Chuỗi log đã được định dạng.
     */
    std::string toString() const {
        std::string result = "[" + timestamp + "] " + serviceID + " [" + severity + "] ";
        if (!sourceIP.empty()) {
            result += "[IP: " + sourceIP + "] ";
        }
        if (!username.empty()) {
            result += "[User: " + username + "] ";
        }
        result += message;
        return result;
    }
};

#endif  // LOG_H