/**
 * @file LogMonitor.cpp
 * @brief Cài đặt bộ máy phân tích log có trạng thái.
 */

#include "LogMonitor.h"

#include "../output/AlertNotifier.h"

LogMonitor::LogMonitor(int alertThreshold) : threshold(alertThreshold), fatalCount(0), criticalCount(0) {
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
