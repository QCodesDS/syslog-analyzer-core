/**
 * @file LogMonitor.h
 * @brief Bộ máy phân tích log có lưu trạng thái (stateful) và hệ thống cảnh báo theo ngưỡng.
 */

#ifndef LOGMONITOR_H
#define LOGMONITOR_H

#include "../../lib/HashTable.hpp"
#include "../../lib/PriorityQueue.hpp"
#include "../../lib/Queue.hpp"
#include "../../lib/Trie.hpp"
#include "../core/Log.h"

/**
 * @struct LogSeverityComparator
 * @brief Bộ so sánh tùy chỉnh dùng cho PriorityQueue để sắp xếp log theo độ nghiêm trọng.
 */
struct LogSeverityComparator {
    /**
     * @brief Trả về điểm xếp hạng mức độ nghiêm trọng.
     * @param s Chuỗi mức độ (FATAL, CRITICAL, ERROR, WARN, ...).
     * @return int Điểm xếp hạng.
     */
    int severityRank(const std::string& s) const {
        if (s == "FATAL")
            return RANK_FATAL;
        if (s == "CRITICAL")
            return RANK_CRITICAL;
        if (s == "ERROR")
            return RANK_ERROR;
        if (s == "WARN")
            return RANK_WARN;
        return RANK_DEFAULT;
    }

    /**
     * @brief So sánh hai đối tượng Log dựa trên mức độ nghiêm trọng.
     * @param a Đối tượng Log thứ nhất.
     * @param b Đối tượng Log thứ hai.
     * @return true Nếu mức độ nghiêm trọng của a nhỏ hơn b.
     */
    bool operator()(const Log& a, const Log& b) const { return severityRank(a.severity) < severityRank(b.severity); }
};

/**
 * @class LogMonitor
 * @brief Theo dõi, phân tích tần suất lỗi, và quản lý các cảnh báo.
 */
class LogMonitor {
private:
    HashTable<std::string, int> errorCount;               /**< @brief Bảng băm đếm số lỗi cho từng dịch vụ. */
    Trie keywordFilter;                                   /**< @brief Bộ lọc Trie chứa các từ khóa quan trọng cần theo dõi. */
    PriorityQueue<Log, LogSeverityComparator> alertQueue; /**< @brief Hàng đợi ưu tiên lưu các cảnh báo quan trọng nhất. */
    Vector<std::string> pendingAlerts;                    /**< @brief Danh sách các cảnh báo chuỗi đang chờ để hiển thị. */
    int threshold;                                        /**< @brief Ngưỡng lỗi kích hoạt cảnh báo dịch vụ. */
    int fatalCount;                                       /**< @brief Tổng số lỗi FATAL. */
    int criticalCount;                                    /**< @brief Tổng số lỗi CRITICAL. */
    HashTable<std::string, Queue<long long>> ipTimeWindow; /**< @brief Bảng băm lưu trữ hàng đợi timestamp của các hoạt động lỗi cho mỗi IP nguồn. */
    HashTable<std::string, int> ipErrorCount;              /**< @brief Bảng băm lưu trữ tổng số lỗi của mỗi IP nguồn. */
    int timeWindowSeconds;                                 /**< @brief Độ rộng cửa sổ thời gian (giây) để theo dõi IP. */
    Queue<Pair<long long, std::string>> globalErrorQueue;  /**< @brief Hàng đợi lỗi toàn cục để phân tích tương quan Botnet. */
    HashTable<std::string, int> activeIPsInWindow;         /**< @brief Số lỗi được tạo ra bởi mỗi IP trong cửa sổ toàn cục. */
    int globalTimeWindowSeconds;                           /**< @brief Độ rộng cửa sổ thời gian toàn cục (giây, mặc định 300). */
    int uniqueIpThreshold;                                 /**< @brief Ngưỡng số lượng IP độc hại duy nhất để phát hiện Botnet. */

    /**
     * @brief Tăng bộ đếm lỗi cho một dịch vụ.
     * @param serviceID Tên dịch vụ.
     * @return int Số lượng lỗi mới được cập nhật.
     */
    int incrementErrorCount(const std::string& serviceID);

    /**
     * @brief Kiểm tra xem số lượng lỗi đã vượt ngưỡng cảnh báo hay chưa.
     * @param serviceID Tên dịch vụ.
     * @param count Số lượng lỗi hiện tại.
     * @return true Nếu cần kích hoạt cảnh báo.
     */
    bool shouldTriggerAlert(const std::string& serviceID, int count);

    /**
     * @brief Đưa một log mang tính chất nghiêm trọng (FATAL/CRITICAL) vào hệ thống cảnh báo.
     * @param log Đối tượng Log nghiêm trọng.
     */
    void enqueueCriticalLog(const Log& log);

    /**
     * @brief Định dạng một dòng thống kê số lượng lỗi của dịch vụ.
     * @param serviceID Tên dịch vụ.
     * @param count Số lượng lỗi.
     * @return std::string Chuỗi thống kê đã định dạng.
     */
    std::string formatStatLine(const std::string& serviceID, int count);

    /**
     * @brief Loại bỏ các nhãn thời gian đã cũ nằm ngoài cửa sổ thời gian khỏi hàng đợi.
     * @param q Hàng đợi các nhãn thời gian của một IP.
     * @param currentTime Thời gian hiện tại cần so sánh.
     */
    void pruneOldTimestamps(Queue<long long>& q, long long currentTime);

public:
    /**
     * @brief Khởi tạo hệ thống giám sát log.
     * @param alertThreshold Ngưỡng cảnh báo (mặc định được lấy từ Constants.h).
     * @param windowSeconds Ngưỡng thời gian của cửa sổ trượt (mặc định 60 giây).
     */
    LogMonitor(int alertThreshold = DEFAULT_ALERT_THRESHOLD, int windowSeconds = 60);

    /**
     * @brief Phân tích một dòng log và cập nhật trạng thái bên trong.
     * @param log Đối tượng Log cần phân tích.
     */
    void analyzeLog(const Log& log);

    /**
     * @brief Lấy cảnh báo nghiêm trọng nhất từ hàng đợi.
     * @return Log Đối tượng Log nghiêm trọng nhất.
     * @throw std::out_of_range Nếu hàng đợi rỗng.
     */
    Log getTopAlert();

    /**
     * @brief Lấy số lượng lỗi hiện tại của một dịch vụ.
     * @param serviceID Tên dịch vụ.
     * @return int Số lượng lỗi.
     */
    int getErrorCount(std::string serviceID);

    /**
     * @brief Trả về danh sách chuỗi thống kê lỗi cho tất cả các dịch vụ.
     * @return Vector<std::string> Danh sách thống kê.
     */
    Vector<std::string> getStats();

    /**
     * @brief Lấy và xóa sạch danh sách cảnh báo đang chờ xử lý.
     * @return Vector<std::string> Danh sách các cảnh báo dạng chuỗi.
     */
    Vector<std::string> flushAlerts();

    /**
     * @brief Lấy tổng số lỗi FATAL đã nhận được.
     * @return int Tổng số lỗi FATAL.
     */
    int getFatalCount() const;

    /**
     * @brief Lấy tổng số lỗi CRITICAL đã nhận được.
     * @return int Tổng số lỗi CRITICAL.
     */
    int getCriticalCount() const;

    /**
     * @brief Tìm ra dịch vụ tạo ra nhiều lỗi nhất hiện tại.
     * @param topService Biến tham chiếu lưu tên dịch vụ lỗi nhiều nhất.
     * @param topCount Biến tham chiếu lưu số lượng lỗi của dịch vụ đó.
     */
    void findTopThreat(std::string& topService, int& topCount);

    /**
     * @brief Tìm ra địa chỉ IP gây ra nhiều lỗi nhất hiện tại.
     * @param topIP Biến tham chiếu lưu địa chỉ IP lỗi nhiều nhất.
     * @param topCount Biến tham chiếu lưu tổng số lỗi của địa chỉ IP đó.
     */
    void findTopMaliciousIP(std::string& topIP, int& topCount);
};

#endif  // LOGMONITOR_H