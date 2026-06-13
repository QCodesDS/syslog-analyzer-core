/**
 * @file Constants.h
 * @brief Cấu hình và các hằng số toàn cục cho hệ thống phân tích syslog.
 * 
 * Tệp này chứa tất cả các tham số có thể điều chỉnh của chương trình, 
 * bao gồm giới hạn giao diện, tham số xử lý batch và hiển thị.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// --- Cấu hình LogMonitor ---
/** @brief Ngưỡng lỗi mặc định để kích hoạt cảnh báo cho một dịch vụ. */
static const int DEFAULT_ALERT_THRESHOLD = 10;
/** @brief Số lượng cảnh báo tối đa được lưu giữ trong lịch sử hiển thị. */
static const int MAX_ALERT_HISTORY = 3;

// --- Cấu hình hiển thị (main.cpp) ---
/** @brief Số dòng log tối đa hiển thị trong phần cuộn log trực tiếp. */
static const int MAX_ROLLING_LINES = 15;
/** @brief Số lượng cảnh báo cuộn tối đa hiển thị. */
static const int MAX_ROLLING_ALERTS = 5;
/** @brief Khoảng thời gian (ms) kiểm tra phím bấm trên bàn phím. */
static const int KEYBOARD_POLL_INTERVAL_MS = 50;

// --- Cấu hình xử lý Batch ---
/** @brief Kích thước lô (batch) mặc định khi đọc log từ file. */
static const int DEFAULT_BATCH_SIZE = 5000;
/** @brief Hệ số nhân thời gian chờ khi ứng dụng ở trạng thái đọc nhanh (fast mode). */
static const double FAST_SLEEP_MULTIPLIER = 0.2;
/** @brief Hệ số nhân thời gian chờ khi ứng dụng ở trạng thái đọc chậm (slow mode). */
static const double SLOW_SLEEP_MULTIPLIER = 3.0;
/** @brief Thời gian nghỉ (ms) khi phát hiện cảnh báo. */
static const int SLEEP_ON_ALERT_MS = 100;
/** @brief Thời gian nghỉ cơ bản (ms) giữa các lần xử lý lô dữ liệu. */
static const int SLEEP_ON_DATA_MS = 1000;
/** @brief Thời gian nghỉ (ms) khi đạt đến cuối file log (EOF). */
static const int SLEEP_ON_EOF_MS = 2000;

// --- Cấu hình thứ tự mức độ nghiêm trọng (Severity Ranks) ---
/** @brief Xếp hạng cho log mức độ FATAL (nghiêm trọng nhất). */
static const int RANK_FATAL = 4;
/** @brief Xếp hạng cho log mức độ CRITICAL. */
static const int RANK_CRITICAL = 3;
/** @brief Xếp hạng cho log mức độ ERROR. */
static const int RANK_ERROR = 2;
/** @brief Xếp hạng cho log mức độ WARN (Cảnh báo). */
static const int RANK_WARN = 1;
/** @brief Xếp hạng cho log mức độ thông thường (INFO, DEBUG, v.v.). */
static const int RANK_DEFAULT = 0;

// --- Cấu hình cấu trúc dữ liệu ---
/** @brief Kích thước bảng băm (HashTable) sử dụng trong hệ thống. */
static const int HASHTABLE_SIZE = 10000;

// --- Cấu hình giao diện (Display) ---
/** @brief Độ rộng tối đa khi in tên dịch vụ. */
static const int SERVICE_NAME_WIDTH = 20;
/** @brief Độ rộng tối đa khi in mức độ nghiêm trọng (severity). */
static const int SEVERITY_WIDTH = 10;
/** @brief Độ rộng của biểu đồ thanh (bar chart) trên giao diện thống kê. */
static const int BAR_CHART_WIDTH = 20;

#endif  // CONSTANTS_H
