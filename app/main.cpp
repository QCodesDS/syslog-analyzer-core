/**
 * @file main.cpp
 * @brief Giao diện dòng lệnh và vòng lặp xử lý sự kiện chính của ứng dụng.
 */
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "config/Constants.h"
#include "core/LogParser.h"
#include "output/Renderer.h"
#include "processing/LogMonitor.h"
#include "source/LogReader.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
int _kbhit() {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
int _getch() {
    return getchar();
}
#endif

/**
 * @enum AppState
 * @brief Định nghĩa các trạng thái giao diện của ứng dụng.
 */
enum AppState { LIVE_MONITOR, STATISTICS, EXIT };

/**
 * @brief Thêm một cảnh báo mới vào lịch sử hiển thị, ưu tiên giữ lại các cảnh báo nghiêm trọng.
 * 
 * Nếu lịch sử đã đầy, thuật toán sẽ cố gắng loại bỏ các cảnh báo ít nghiêm trọng hơn
 * (không phải CRITICAL) để nhường chỗ cho cảnh báo mới.
 * 
 * @param alertHistory Danh sách lịch sử các cảnh báo.
 * @param newAlert Chuỗi cảnh báo mới cần thêm vào.
 */
void addAlertToHistory(Vector<std::string>& alertHistory, const std::string& newAlert) {
    bool isCritical = (newAlert.find("[CRITICAL ALERT]") != std::string::npos);

    if (alertHistory.getSize() < MAX_ALERT_HISTORY) {
        alertHistory.pushBack(newAlert);
        return;
    }

    if (isCritical) {
        int targetIdx = -1;
        for (int i = 0; i < alertHistory.getSize(); i++) {
            if (alertHistory[i].find("[CRITICAL ALERT]") == std::string::npos) {
                targetIdx = i;
                break;
            }
        }
        if (targetIdx != -1) {
            Vector<std::string> temp;
            for (int i = 0; i < alertHistory.getSize(); i++) {
                if (i != targetIdx) {
                    temp.pushBack(alertHistory[i]);
                }
            }
            temp.pushBack(newAlert);
            alertHistory = temp;
            return;
        }
    }

    Vector<std::string> temp;
    for (int i = 1; i < alertHistory.getSize(); i++) {
        temp.pushBack(alertHistory[i]);
    }
    temp.pushBack(newAlert);
    alertHistory = temp;
}

/**
 * @brief Điểm khởi đầu của chương trình phân tích syslog.
 * 
 * Quản lý vòng lặp chính: Đọc lô dữ liệu, phân tích, định dạng dữ liệu đầu ra,
 * cập nhật giao diện (live monitor hoặc thống kê) và xử lý phím bấm điều hướng.
 * 
 * @param argc Số lượng tham số dòng lệnh.
 * @param argv Danh sách các tham số dòng lệnh.
 * @return int Mã thoát chương trình (0 là thành công).
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_file> [--fast | --slow]\n";
        return 1;
    }

    std::string filename = "";
    int batchSize = DEFAULT_BATCH_SIZE;
    double sleepMultiplier = 1.0;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--fast") {
            sleepMultiplier = FAST_SLEEP_MULTIPLIER;
        } else if (arg == "--slow") {
            sleepMultiplier = SLOW_SLEEP_MULTIPLIER;
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "Usage: " << argv[0] << " <log_file> [--fast | --slow]\n";
        return 1;
    }

    LogReader reader(filename);
    LogMonitor monitor(DEFAULT_ALERT_THRESHOLD);

    AppState state = LIVE_MONITOR;
    bool statsViewDirty = false;
    int batchNum = 0;
    bool isCatchingUp = true;

    Vector<std::string> rollingBuffer;
    int totalLogsProcessed = 0;
    int totalThreatsCount = 0;
    int totalWarningsCount = 0;
    Vector<std::string> alertHistory;

    std::cout << "\033[1;36m=== Distributed Log Analyzer ===\033[0m\n";
    std::cout << "Press 1: Live Monitor | 2: Statistics | Q: Quit\n";

    while (state != EXIT) {
        Vector<std::string> batch = reader.readBatch(batchSize);
        bool batchHasContent = batch.getSize() > 0;
        if (batchHasContent) {
            batchNum++;
        }
        int alertCount = 0;

        Vector<std::string> printBuffer;

        for (int i = 0; i < batch.getSize(); i++) {
            Log log;
            if (LogParser::parse(batch[i], log)) {
                totalLogsProcessed++;
                monitor.analyzeLog(log);

                if (log.severity == "ERROR" || log.severity == "FATAL" || log.severity == "CRITICAL") {
                    alertCount++;
                    totalThreatsCount++;
                } else if (log.severity == "WARN" || log.severity == "WARNING") {
                    totalWarningsCount++;
                }

                std::string formattedLine = Renderer::formatLogLine(log.timestamp, log.serviceID, log.severity, log.message);
                printBuffer.pushBack(formattedLine);
            }
        }

        int printStart = 0;
        if (printBuffer.getSize() > MAX_ROLLING_LINES) {
            printStart = printBuffer.getSize() - MAX_ROLLING_LINES;
        }
        for (int i = printStart; i < printBuffer.getSize(); i++) {
            rollingBuffer.pushBack(printBuffer[i]);
        }
        if (rollingBuffer.getSize() > MAX_ROLLING_LINES) {
            Vector<std::string> trimmed;
            int start = rollingBuffer.getSize() - MAX_ROLLING_LINES;
            for (int i = start; i < rollingBuffer.getSize(); i++)
                trimmed.pushBack(rollingBuffer[i]);
            rollingBuffer = trimmed;
        }

        Vector<std::string> alerts = monitor.flushAlerts();
        for (int i = 0; i < alerts.getSize(); i++) {
            addAlertToHistory(alertHistory, alerts[i]);
        }

        if (state == LIVE_MONITOR) {
            std::string topService;
            int topCount = 0;
            monitor.findTopThreat(topService, topCount);

            std::string topIP;
            int topIPCount = 0;
            monitor.findTopMaliciousIP(topIP, topIPCount);

            Renderer::renderLiveMonitor(batchNum, totalLogsProcessed, totalThreatsCount, filename, topService, topCount, topIP, topIPCount,
                                        totalWarningsCount, monitor.getFatalCount(), monitor.getCriticalCount(), rollingBuffer,
                                        alertHistory);
        }

        if (state == STATISTICS && (statsViewDirty || batchHasContent)) {
            std::string topService;
            int topCount = 0;
            monitor.findTopThreat(topService, topCount);

            std::string topIP;
            int topIPCount = 0;
            monitor.findTopMaliciousIP(topIP, topIPCount);

            Renderer::renderStatsDashboard(totalLogsProcessed, totalThreatsCount, filename, monitor.getStats(), topService, topCount, topIP,
                                           topIPCount, monitor.getFatalCount(), monitor.getCriticalCount(), totalWarningsCount);

            statsViewDirty = false;
        }

        int dynamicSleep = SLEEP_ON_DATA_MS;
        if (isCatchingUp) {
            dynamicSleep = 1;
            if (!batchHasContent) {
                isCatchingUp = false;
            }
        } else {
            if (alertCount > 0) {
                dynamicSleep = SLEEP_ON_ALERT_MS;
            } else if (!batchHasContent) {
                dynamicSleep = SLEEP_ON_EOF_MS;
            }
        }

        int sleepDurationMs = static_cast<int>(dynamicSleep * sleepMultiplier);
        if (sleepDurationMs < KEYBOARD_POLL_INTERVAL_MS)
            sleepDurationMs = KEYBOARD_POLL_INTERVAL_MS;

        int sleepElapsedMs = 0;
        while (sleepElapsedMs < sleepDurationMs && state != EXIT) {
            std::this_thread::sleep_for(std::chrono::milliseconds(KEYBOARD_POLL_INTERVAL_MS));
            sleepElapsedMs += KEYBOARD_POLL_INTERVAL_MS;
            if (_kbhit()) {
                int keyPressed = _getch();
                if (keyPressed == '1' && state != LIVE_MONITOR) {
                    state = LIVE_MONITOR;
                    statsViewDirty = false;
                } else if (keyPressed == '2' && state != STATISTICS) {
                    state = STATISTICS;
                    statsViewDirty = true;
                } else if (keyPressed == 'q' || keyPressed == 'Q') {
                    state = EXIT;
                }
                break;
            }
        }
    }

    Renderer::clearScreen();
    std::cout << "\033[1;36m================================================================================\033[0m\n";
    std::cout << "  \033[1;37mDistributed Log Analyzer v2.0.0\033[0m | \033[1;33mSystem Shutdown\033[0m\n";
    std::cout << "  Status: \033[1;31m[EXITED]\033[0m | Thank you for using the system!\n";
    std::cout << "\033[1;36m================================================================================\033[0m\n";

    return 0;
}
