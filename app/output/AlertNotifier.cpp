/**
 * @file AlertNotifier.cpp
 * @brief Cài đặt các phương thức định dạng và in cảnh báo của AlertNotifier.
 */

#include "AlertNotifier.h"

#include <iostream>

std::string AlertNotifier::formatNotify(const Log& log, int count) {
    return "\033[1;31m[ALERT]\033[0m " + log.serviceID + " reached " + std::to_string(count) + " errors. Last: " + log.toString();
}

std::string AlertNotifier::formatCritical(const Log& log) {
    return "\033[1;5;31m[CRITICAL ALERT]\033[0m " + log.toString();
}

std::string AlertNotifier::formatIPAlert(const std::string& ip, int count, const Log& lastLog) {
    return "\033[1;35m[IP ALERT]\033[0m Source IP " + ip + " detected " + std::to_string(count) +
           " anomalous activities in window. Last: " + lastLog.toString();
}

void AlertNotifier::notify(const Log& log, int count) {
    std::cout << formatNotify(log, count) << "\n";
}

void AlertNotifier::notifyCritical(const Log& log) {
    std::cout << formatCritical(log) << "\n";
}
