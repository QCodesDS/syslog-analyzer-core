/**
 * @file AlertNotifier.h
 * @brief Chịu trách nhiệm định dạng và xuất các cảnh báo an ninh hoặc lỗi nghiêm trọng.
 */
#ifndef ALERTNOTIFIER_H
#define ALERTNOTIFIER_H

#include "../core/Log.h"

/**
 * @class AlertNotifier
 * @brief Lớp cung cấp các phương thức tĩnh để định dạng và in thông báo cảnh báo.
 */

class AlertNotifier {
public:
    /**
     * @brief Định dạng chuỗi thông báo khi một dịch vụ vượt ngưỡng lỗi.
     * @param log Đối tượng log gây ra cảnh báo.
     * @param count Số lượng lỗi hiện tại của dịch vụ.
     * @return std::string Chuỗi cảnh báo đã được định dạng.
     */
    static std::string formatNotify(const Log& log, int count);

    /**
     * @brief Định dạng chuỗi thông báo cho các log cực kỳ nghiêm trọng (FATAL/CRITICAL).
     * @param log Đối tượng log nghiêm trọng.
     * @return std::string Chuỗi cảnh báo nghiêm trọng đã được định dạng.
     */
    static std::string formatCritical(const Log& log);

    /**
     * @brief In trực tiếp thông báo vượt ngưỡng lỗi ra console.
     * @param log Đối tượng log gây ra cảnh báo.
     * @param count Số lượng lỗi hiện tại của dịch vụ.
     */
    static void notify(const Log& log, int count);

    /**
     * @brief In trực tiếp thông báo lỗi nghiêm trọng ra console.
     * @param log Đối tượng log nghiêm trọng.
     */
    static void notifyCritical(const Log& log);
};

#endif  // ALERTNOTIFIER_H