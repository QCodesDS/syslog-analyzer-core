/**
 * @file Renderer.cpp
 * @brief Cài đặt các phương thức giao diện console của lớp Renderer.
 */

#include "Renderer.h"

#include <iostream>

#include "../config/Constants.h"

void Renderer::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

std::string Renderer::padRight(const std::string& s, int width) {
    if ((int)s.size() >= width)
        return s.substr(0, width);
    return s + std::string(width - s.size(), ' ');
}

std::string Renderer::extractTime(const std::string& timestamp) {
    if (timestamp.size() >= 8)
        return timestamp.substr(timestamp.size() - 8);
    return timestamp;
}

std::string Renderer::renderBar(int value, int maxValue, int barWidth) {
    int barLength = 0;
    if (maxValue > 0) {
        barLength = (value * barWidth) / maxValue;
    }
    std::string barStr = std::string(barLength, '=') + std::string(barWidth - barLength, ' ');
    int percentage = (value * 100) / (maxValue > 0 ? maxValue : 1);
    return "[" + barStr + "] " + std::to_string(percentage) + "%";
}

std::string Renderer::colorSeverity(const std::string& severity) {
    std::string sevTag = "[" + severity + "]";
    std::string paddedSev = padRight(sevTag, SEVERITY_WIDTH);
    if (severity == "FATAL" || severity == "CRITICAL") {
        return "\033[1;31m" + paddedSev + "\033[0m";
    } else if (severity == "ERROR") {
        return "\033[0;31m" + paddedSev + "\033[0m";
    } else if (severity == "WARN" || severity == "WARNING") {
        return "\033[0;33m" + paddedSev + "\033[0m";
    } else {
        return "\033[0;90m" + paddedSev + "\033[0m";
    }
}

std::string Renderer::formatLogLine(const std::string& timestamp, const std::string& serviceID, const std::string& severity,
                                    const std::string& message) {
    return "  " + extractTime(timestamp) + " | " + padRight(serviceID, 16) + " | " + colorSeverity(severity) + " | " + message;
}

void Renderer::renderLiveMonitor(int batchNum, int totalLogsProcessed, int totalThreats, const std::string& filename,
                                 const std::string& topService, int topCount, const std::string& topIP, int topIPCount, int totalWarnings,
                                 int fatalCount, int criticalCount, const Vector<std::string>& rollingBuffer,
                                 const Vector<std::string>& alertHistory) {
    (void)batchNum;
    clearScreen();
    std::cout << "\033[1;36m================================================================================\033[0m\n";
    std::cout << "  \033[1;37mDistributed Log Analyzer \033[0m | Target File: \033[1;33m" << filename << "\033[0m\n";
    std::cout << "  Status: \033[1;32m[MONITORING]\033[0m | Total Logs Analyzed: \033[1;35m" << totalLogsProcessed
              << "\033[0m | Total Threats: \033[1;31m" << totalThreats << "\033[0m\n";
    std::cout << "\033[1;36m================================================================================\033[0m\n\n";

    std::cout << "\033[1;37m[SYSTEM METRICS]\033[0m\n";
    if (topService.empty() || topService == "None") {
        std::cout << "  Most Critical Service: \033[0;90mNone\033[0m\n";
    } else {
        std::cout << "  Most Critical Service: \033[1;31m" << topService << "\033[0m (\033[1;33m" << topCount
                  << "\033[0m incidents)\033[0m\n";
    }
    if (topIP.empty() || topIP == "None") {
        std::cout << "  Top Malicious IP     : \033[0;90mNone\033[0m\n";
    } else {
        std::cout << "  Top Malicious IP     : \033[1;35m" << topIP << "\033[0m (\033[1;33m" << topIPCount << "\033[0m incidents)\033[0m\n";
    }
    std::cout << "  Total System Warnings: \033[1;33m" << totalWarnings << "\033[0m | Fatal/Critical Errors: \033[1;31m"
              << (fatalCount + criticalCount) << "\033[0m\n\n";

    std::cout << "\033[1;37m[RECENT LOG EVENTS (Last 15)]\033[0m\n";
    if (rollingBuffer.getSize() == 0) {
        std::cout << "  \033[0;90mWaiting for log events...\033[0m\n";
    } else {
        for (int i = 0; i < rollingBuffer.getSize(); i++) {
            std::cout << rollingBuffer[i] << "\n";
        }
    }

    std::cout << "\n\033[1;37m[CRITICAL ALERTS HISTORY (Last 3)]\033[0m\n";
    if (alertHistory.getSize() == 0) {
        std::cout << "  \033[0;90mNo critical alerts triggered yet.\033[0m\n";
    } else {
        for (int i = 0; i < alertHistory.getSize(); i++) {
            std::cout << "  " << alertHistory[i] << "\n";
        }
    }

    std::cout << "\n\033[1;36m------------------------------------------------------------------------------------------\033[0m\n";
    std::cout << "  Navigation: \033[1;32m[1] Live Monitor (Active)\033[0m  |  \033[1;37m[2] Stats Dashboard\033[0m  |  \033[1;31m[Q] Quit "
                 "System\033[0m\n";
    std::cout << "\033[1;36m==========================================================================================\033[0m\n";
}

void Renderer::renderStatsDashboard(const Vector<std::string>& stats, const std::string& topService, int topCount, int fatalCount,
                                    int criticalCount) {
    // Phương thức rút gọn, định tuyến về phương thức đầy đủ
    renderStatsDashboard(0, 0, "Unknown", stats, topService, topCount, "None", 0, fatalCount, criticalCount, 0);
}

void Renderer::renderStatsDashboard(int totalLogsProcessed, int totalThreats, const std::string& filename, const Vector<std::string>& stats,
                                    const std::string& topService, int topCount, const std::string& topIP, int topIPCount, int fatalCount,
                                    int criticalCount, int totalWarnings) {
    clearScreen();
    std::cout << "\033[1;36m================================================================================\033[0m\n";
    std::cout << "  \033[1;37mSystem Health Dashboard\033[0m | Target File: \033[1;33m" << filename << "\033[0m\n";
    std::cout << "  Total Logs Scanned: \033[1;35m" << totalLogsProcessed << "\033[0m | Total Threats: \033[1;31m" << totalThreats
              << "\033[0m\n";
    std::cout << "\033[1;36m================================================================================\033[0m\n\n";

    std::cout << "\033[1;37m[SERVICE ERROR BREAKDOWN]\033[0m\n";
    std::cout << "  " << padRight("Service Name", SERVICE_NAME_WIDTH) << " " << padRight("Error Count", 12) << " Distribution Chart\n";
    std::cout << "  \033[0;90m------------------------------------------------------------------\033[0m\n";

    for (int i = 0; i < stats.getSize(); i++) {
        std::string s = stats[i];
        size_t colonPos = s.find(':');
        if (colonPos != std::string::npos) {
            std::string sName = s.substr(0, colonPos);
            std::string remainder = s.substr(colonPos + 2);
            size_t spacePos = remainder.find(' ');
            std::string countStr = remainder.substr(0, spacePos);
            int count = std::stoi(countStr);

            std::string distributionBar = renderBar(count, topCount, BAR_CHART_WIDTH);
            std::cout << "  " << padRight(sName, SERVICE_NAME_WIDTH) << " " << padRight(countStr, 12) << " " << distributionBar << "\n";
        }
    }
    std::cout << "  \033[0;90m------------------------------------------------------------------\033[0m\n\n";

    std::cout << "\033[1;37m[THREAT OVERVIEW]\033[0m\n";
    if (topService.empty() || topService == "None") {
        std::cout << "  Top System Threat    : \033[0;90mNone\033[0m\n";
    } else {
        std::cout << "  Top System Threat    : \033[1;31m" << topService << "\033[0m (\033[1;33m" << topCount << "\033[0m errors)\033[0m\n";
    }
    if (topIP.empty() || topIP == "None") {
        std::cout << "  Top Malicious IP     : \033[0;90mNone\033[0m\n";
    } else {
        std::cout << "  Top Malicious IP     : \033[1;35m" << topIP << "\033[0m (\033[1;33m" << topIPCount << "\033[0m alerts)\033[0m\n";
    }
    std::cout << "  Fatal/Critical Errors: \033[1;31m" << (fatalCount + criticalCount) << "\033[0m events detected\n";
    std::cout << "  Total Warnings       : \033[1;33m" << totalWarnings << "\033[0m events detected\n";

    std::cout << "\n\033[1;36m------------------------------------------------------------------------------------------\033[0m\n";
    std::cout << "  Navigation: \033[1;37m[1] Live Monitor\033[0m  |  \033[1;32m[2] Stats Dashboard (Active)\033[0m  |  \033[1;31m[Q] Quit "
                 "System\033[0m\n";
    std::cout << "\033[1;36m==========================================================================================\033[0m\n";
}
