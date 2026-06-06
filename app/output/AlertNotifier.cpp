#include "AlertNotifier.h"

#include <iostream>

void AlertNotifier::notify(const Log& log, int count) {
    std::cout << "\033[1;31m[ALERT]\033[0m " << log.serviceID << " reached " << count << " errors. Last: " << log.toString() << "\n";
}

void AlertNotifier::notifyCritical(const Log& log) {
    std::cout << "\033[1;5;31m[CRITICAL ALERT]\033[0m " << log.toString() << "\n";
}
