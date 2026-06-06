/**
 * @file Renderer.h
 * @brief Giao diện kết xuất (rendering) bảng điều khiển terminal.
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include "../../lib/Vector.hpp"

/**
 * @class Renderer
 * @brief Chịu trách nhiệm hiển thị giao diện Live Monitor và Stats Dashboard lên console.
 */
class Renderer {
public:
    /**
     * @brief Xóa toàn bộ màn hình terminal sử dụng ANSI escape codes.
     */
    static void clearScreen();

    /**
     * @brief Hiển thị bảng điều khiển theo dõi trực tiếp (Live Monitor).
     * 
     * @param batchNum Số thứ tự của lô dữ liệu (không còn sử dụng, giữ cho tương thích).
     * @param totalLogsProcessed Tổng số log đã xử lý.
     * @param totalThreats Tổng số mối đe dọa phát hiện được.
     * @param filename Tên file đang theo dõi.
     * @param topService Tên dịch vụ có nhiều lỗi nhất.
     * @param topCount Số lượng lỗi của dịch vụ đứng đầu.
     * @param totalWarnings Tổng số cảnh báo hệ thống.
     * @param fatalCount Tổng số lỗi FATAL.
     * @param criticalCount Tổng số lỗi CRITICAL.
     * @param rollingBuffer Danh sách các log gần đây nhất để cuộn hiển thị.
     * @param alertHistory Lịch sử các cảnh báo nghiêm trọng gần đây.
     */
    static void renderLiveMonitor(int batchNum, int totalLogsProcessed, int totalThreats, const std::string& filename,
                                  const std::string& topService, int topCount, int totalWarnings, int fatalCount, int criticalCount,
                                  const Vector<std::string>& rollingBuffer, const Vector<std::string>& alertHistory);

    /**
     * @brief Cổng hiển thị bảng thống kê rút gọn (deprecated/fallback).
     * 
     * @param stats Danh sách các chuỗi thống kê theo dịch vụ.
     * @param topService Tên dịch vụ có nhiều lỗi nhất.
     * @param topCount Số lượng lỗi của dịch vụ đứng đầu.
     * @param fatalCount Tổng số lỗi FATAL.
     * @param criticalCount Tổng số lỗi CRITICAL.
     */
    static void renderStatsDashboard(const Vector<std::string>& stats, const std::string& topService, int topCount, int fatalCount,
                                     int criticalCount);

    /**
     * @brief Hiển thị bảng điều khiển Thống kê hệ thống đầy đủ chi tiết.
     * 
     * @param totalLogsProcessed Tổng số log đã xử lý.
     * @param totalThreats Tổng số mối đe dọa (cảnh báo + nghiêm trọng).
     * @param filename Tên file đang theo dõi.
     * @param stats Danh sách các chuỗi thống kê theo dịch vụ.
     * @param topService Tên dịch vụ có nhiều lỗi nhất.
     * @param topCount Số lượng lỗi của dịch vụ đứng đầu.
     * @param fatalCount Tổng số lỗi FATAL.
     * @param criticalCount Tổng số lỗi CRITICAL.
     * @param totalWarnings Tổng số cảnh báo thông thường.
     */
    static void renderStatsDashboard(int totalLogsProcessed, int totalThreats, const std::string& filename,
                                     const Vector<std::string>& stats, const std::string& topService, int topCount, int fatalCount,
                                     int criticalCount, int totalWarnings);

    /**
     * @brief Định dạng một dòng log với nhãn màu mức độ nghiêm trọng.
     * 
     * @param timestamp Nhãn thời gian.
     * @param serviceID Tên dịch vụ.
     * @param severity Mức độ nghiêm trọng.
     * @param message Nội dung thông điệp.
     * @return std::string Chuỗi log đã được định dạng và thêm mã màu ANSI.
     */
    static std::string formatLogLine(const std::string& timestamp, const std::string& serviceID, const std::string& severity,
                                     const std::string& message);

    /**
     * @brief Trả về nhãn mức độ nghiêm trọng có màu ANSI.
     * 
     * @param severity Chuỗi mức độ nghiêm trọng.
     * @return std::string Chuỗi đã bọc mã màu.
     */
    static std::string colorSeverity(const std::string& severity);

private:
    /**
     * @brief Thêm khoảng trắng vào bên phải chuỗi để đạt độ rộng cố định.
     * 
     * @param s Chuỗi ban đầu.
     * @param width Độ rộng mong muốn.
     * @return std::string Chuỗi sau khi được đệm thêm khoảng trắng.
     */
    static std::string padRight(const std::string& s, int width);

    /**
     * @brief Trích xuất thời gian (HH:MM:SS) từ một chuỗi nhãn thời gian đầy đủ.
     * 
     * @param timestamp Chuỗi thời gian gốc.
     * @return std::string Chuỗi thời gian rút gọn.
     */
    static std::string extractTime(const std::string& timestamp);

    /**
     * @brief Vẽ biểu đồ thanh ASCII tỷ lệ phần trăm.
     * 
     * Ví dụ: renderBar(45, 100, 20) -> "[=========           ] 45%"
     * 
     * @param value Giá trị hiện tại.
     * @param maxValue Giá trị lớn nhất (để làm mốc 100%).
     * @param barWidth Chiều rộng tối đa của thanh biểu đồ.
     * @return std::string Biểu đồ dạng chuỗi văn bản.
     */
    static std::string renderBar(int value, int maxValue, int barWidth);
};

#endif  // RENDERER_H
