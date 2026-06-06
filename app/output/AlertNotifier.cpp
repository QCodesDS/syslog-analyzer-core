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

void AlertNotifier::notify(const Log& log, int count) {
    std::cout << formatNotify(log, count) << "\n";
}

void AlertNotifier::notifyCritical(const Log& log) {
    std::cout << formatCritical(log) << "\n";
}
