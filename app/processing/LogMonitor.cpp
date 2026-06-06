/**
 * @file LogMonitor.cpp
 * @brief Cài đặt bộ máy phân tích log có trạng thái.
 */

#include "LogMonitor.h"

#include <ctime>

#include "../output/AlertNotifier.h"

/**
 * @brief Chuyển đổi chuỗi timestamp dạng YYYY-MM-DD HH:MM:SS sang Epoch time.
 * @param timestampStr Chuỗi timestamp cần chuyển đổi.
 * @return long long Epoch time tính bằng giây.
 */
static long long parseTimestampToUnix(const std::string& timestampStr) {
    if (timestampStr.size() < 19)
        return 0;
    std::tm t = {};
    try {
        int year = std::stoi(timestampStr.substr(0, 4));
        int month = std::stoi(timestampStr.substr(5, 2));
        int day = std::stoi(timestampStr.substr(8, 2));
        int hour = std::stoi(timestampStr.substr(11, 2));
        int minute = std::stoi(timestampStr.substr(14, 2));
        int second = std::stoi(timestampStr.substr(17, 2));

        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        t.tm_hour = hour;
        t.tm_min = minute;
        t.tm_sec = second;
        t.tm_isdst = -1;
    } catch (...) {
        return 0;
    }

    std::time_t epoch = std::mktime(&t);
    if (epoch == -1)
        return 0;
    return static_cast<long long>(epoch);
}

LogMonitor::LogMonitor(int alertThreshold, int windowSeconds)
    : threshold(alertThreshold), fatalCount(0), criticalCount(0), timeWindowSeconds(windowSeconds) {
    // Thêm các từ khóa cảnh báo quan trọng vào bộ lọc Trie
    keywordFilter.insert("ERROR");
    keywordFilter.insert("FATAL");
    keywordFilter.insert("CRITICAL");
    keywordFilter.insert("TIMEOUT");
    keywordFilter.insert("EXCEPTION");
    keywordFilter.insert("WARN");
}

int LogMonitor::incrementErrorCount(const std::string& serviceID) {
    int* existingCount = errorCount.find(serviceID);
    int previousCount = existingCount ? *existingCount : 0;
    int updatedCount = previousCount + 1;
    // Cập nhật giá trị vào bảng băm (sẽ thay thế giá trị cũ nếu đã tồn tại)
    errorCount.insert(Pair<std::string, int>(serviceID, updatedCount));
    return updatedCount;
}

bool LogMonitor::shouldTriggerAlert(const std::string& serviceID, int count) {
    (void)serviceID;  // Biến không sử dụng, ép kiểu void để tránh cảnh báo
    return count >= threshold;
}

void LogMonitor::enqueueCriticalLog(const Log& log) {
    if (log.severity == "FATAL") {
        fatalCount++;
    } else if (log.severity == "CRITICAL") {
        criticalCount++;
    }
    alertQueue.insert(log);  // Chèn vào hàng đợi ưu tiên theo mức độ nghiêm trọng
    pendingAlerts.pushBack(AlertNotifier::formatCritical(log));
}

void LogMonitor::analyzeLog(const Log& log) {
    // Chỉ xử lý và theo dõi nếu mức độ nghiêm trọng trùng với từ khóa cần thiết
    if (keywordFilter.startsWith(log.severity)) {
        int updatedCount = incrementErrorCount(log.serviceID);

        // Xử lý các lỗi nghiêm trọng riêng biệt
        if (log.severity == "FATAL" || log.severity == "CRITICAL") {
            enqueueCriticalLog(log);
        }

        // Tạo cảnh báo nếu vượt ngưỡng quy định
        if (shouldTriggerAlert(log.serviceID, updatedCount)) {
            pendingAlerts.pushBack(AlertNotifier::formatNotify(log, updatedCount));
        }

        // Theo dõi và phân tích cửa sổ thời gian trượt đối với địa chỉ IP nguồn
        if (!log.sourceIP.empty()) {
            long long currentTime = parseTimestampToUnix(log.timestamp);
            Queue<long long>* q = ipTimeWindow.find(log.sourceIP);
            if (!q) {
                Queue<long long> newQ;
                ipTimeWindow.insert(Pair<std::string, Queue<long long>>(log.sourceIP, newQ));
                q = ipTimeWindow.find(log.sourceIP);
            }
            if (q) {
                q->enqueue(currentTime);
                pruneOldTimestamps(*q, currentTime);

                // Tăng tổng số lỗi của IP nguồn
                int* existingIPCount = ipErrorCount.find(log.sourceIP);
                int previousIPCount = existingIPCount ? *existingIPCount : 0;
                int updatedIPCount = previousIPCount + 1;
                ipErrorCount.insert(Pair<std::string, int>(log.sourceIP, updatedIPCount));

                // Kích hoạt cảnh báo nếu số lượng lỗi trong cửa sổ trượt vượt ngưỡng
                if (q->size() >= threshold) {
                    pendingAlerts.pushBack(AlertNotifier::formatIPAlert(log.sourceIP, q->size(), log));
                }
            }
        }
    }
}

void LogMonitor::pruneOldTimestamps(Queue<long long>& q, long long currentTime) {
    while (!q.empty() && (currentTime - q.front() > timeWindowSeconds)) {
        q.dequeue();
    }
}

Log LogMonitor::getTopAlert() {
    if (alertQueue.empty()) {
        throw std::out_of_range("Không có cảnh báo nào trong hàng đợi ưu tiên.");
    }
    return alertQueue.peek();
}

int LogMonitor::getErrorCount(std::string serviceID) {
    int* existingCount = errorCount.find(serviceID);
    return existingCount ? *existingCount : 0;
}

std::string LogMonitor::formatStatLine(const std::string& serviceID, int count) {
    return serviceID + ": " + std::to_string(count) + " errors";
}

Vector<std::string> LogMonitor::getStats() {
    Vector<std::string> stats;
    // Lấy toàn bộ các phần tử trong bảng băm theo thứ tự
    Vector<Pair<std::string, int>> items = errorCount.lnr();

    for (int i = 0; i < items.getSize(); i++) {
        stats.pushBack(formatStatLine(items[i].key, items[i].value));
    }
    return stats;
}

Vector<std::string> LogMonitor::flushAlerts() {
    Vector<std::string> out = pendingAlerts;
    pendingAlerts.clear();
    return out;
}

int LogMonitor::getFatalCount() const {
    return fatalCount;
}

int LogMonitor::getCriticalCount() const {
    return criticalCount;
}

void LogMonitor::findTopThreat(std::string& topService, int& topCount) {
    topService = "None";
    topCount = 0;
    Vector<Pair<std::string, int>> items = errorCount.lnr();

    // Tìm kiếm tuyến tính dịch vụ có nhiều lỗi nhất
    for (int i = 0; i < items.getSize(); i++) {
        if (items[i].value > topCount) {
            topCount = items[i].value;
            topService = items[i].key;
        }
    }
}

void LogMonitor::findTopMaliciousIP(std::string& topIP, int& topCount) {
    topIP = "None";
    topCount = 0;
    Vector<Pair<std::string, int>> items = ipErrorCount.lnr();

    // Tìm kiếm tuyến tính địa chỉ IP gây ra nhiều lỗi nhất
    for (int i = 0; i < items.getSize(); i++) {
        if (items[i].value > topCount) {
            topCount = items[i].value;
            topIP = items[i].key;
        }
    }
}
