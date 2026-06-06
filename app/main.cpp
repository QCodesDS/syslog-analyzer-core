#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "core/LogParser.h"
#include "output/AlertNotifier.h"
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

enum AppState { LIVE_MONITOR, STATISTICS, EXIT };

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_file> [--fast | --slow]\n";
        return 1;
    }

    std::string filename = "";
    int batchSize = 200;
    int windowSleepMs = 1000;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--fast") {
            batchSize = 50;
            windowSleepMs = 200;
        } else if (arg == "--slow") {
            batchSize = 20;
            windowSleepMs = 2000;
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "Usage: " << argv[0] << " <log_file> [--fast | --slow]\n";
        return 1;
    }

    LogReader reader(filename);
    LogMonitor monitor(10);  // alert threshold = 10

    AppState state = LIVE_MONITOR;
    bool needsStatsRedraw = false;

    std::cout << "\033[1;36m=== Distributed Log Analyzer ===\033[0m\n";
    std::cout << "Press 1: Live Monitor | 2: Statistics | Q: Quit\n";

    while (state != EXIT) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == '1' && state != LIVE_MONITOR) {
                state = LIVE_MONITOR;
                std::cout << "\n[Switched to Live Monitor]\n";
            } else if (ch == '2' && state != STATISTICS) {
                state = STATISTICS;
                needsStatsRedraw = true;
            } else if (ch == 'q' || ch == 'Q') {
                state = EXIT;
                std::cout << "\n[Exiting...]\n";
                break;
            }
        }

        Vector<std::string> batch = reader.readBatch(batchSize);
        bool hasNewData = batch.getSize() > 0;
        int alertCount = 0;

        for (int i = 0; i < batch.getSize(); i++) {
            Log log;
            if (LogParser::parse(batch[i], log)) {
                monitor.analyzeLog(log);

                if (log.severity == "FATAL" || log.severity == "CRITICAL") {
                    alertCount++;
                }

                if (state == LIVE_MONITOR) {
                    if (log.severity == "FATAL" || log.severity == "CRITICAL") {
                        std::cout << "\033[1;31m" << batch[i] << "\033[0m\n";
                    } else if (log.severity == "ERROR") {
                        std::cout << "\033[0;31m" << batch[i] << "\033[0m\n";
                    } else if (log.severity == "WARN") {
                        std::cout << "\033[0;33m" << batch[i] << "\033[0m\n";
                    } else if (log.severity == "INFO") {
                        std::cout << "\033[0;32m" << batch[i] << "\033[0m\n";
                    } else {
                        std::cout << "\033[0m" << batch[i] << "\033[0m\n";
                    }
                }
            }
        }

        if (hasNewData) {
            // Print batch summary
            if (state == LIVE_MONITOR) {
                std::cout << "\033[0;90m--- Batch: " << batch.getSize() << " lines processed";
                if (alertCount > 0)
                    std::cout << " | \033[1;31mAlerts: " << alertCount << "\033[0;90m";
                std::cout << " ---\033[0m\n";
            }

            // 1-second analysis window (configurable)
            std::this_thread::sleep_for(std::chrono::milliseconds(windowSleepMs));
        }

        if (state == STATISTICS && (needsStatsRedraw || hasNewData)) {
#ifdef _WIN32
            std::system("cls");
#else
            std::system("clear");
#endif
            std::cout << "\033[1;36m=== Current System Statistics ===\033[0m\n";
            std::cout << "Press 1: Live Monitor | 2: Statistics | Q: Quit\n\n";

            Vector<std::string> stats = monitor.getStats();
            for (int i = 0; i < stats.getSize(); i++) {
                std::cout << stats[i] << "\n";
            }
            needsStatsRedraw = false;
        }

        if (!hasNewData) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    return 0;
}
