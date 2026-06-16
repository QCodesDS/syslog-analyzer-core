/**
 * @file LogParser.cpp
 * @brief Cài đặt của lớp LogParser để phân tích cú pháp log.
 */

#include "LogParser.h"

#include <cctype>
#include <regex>
#include <vector>

/**
 * @brief Giải mã một chuỗi Base64.
 * 
 * @param in Chuỗi Base64 đầu vào.
 * @return std::string Chuỗi sau khi được giải mã.
 */
static std::string decodeBase64(const std::string& in) {
    std::string padded = in;
    while (padded.length() % 4 != 0) {
        padded += '=';
    }
    static const char* const b64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[b64chars[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : padded) {
        if (T[c] == -1) {
            if (c == '=')
                break;
            continue;
        }
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

/**
 * @brief Kiểm tra xem một chuỗi có phải là định dạng Base64 hợp lệ hay không.
 * 
 * @param s Chuỗi cần kiểm tra.
 * @return true Nếu chuỗi là Base64 hợp lệ.
 * @return false Nếu chuỗi không phải là Base64 hợp lệ.
 */
static bool isBase64(const std::string& s) {
    if (s.length() < 3)
        return false;
    int paddingCount = 0;
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (std::isalnum(c) || c == '+' || c == '/') {
            if (paddingCount > 0)
                return false;
        } else if (c == '=') {
            paddingCount++;
            if (paddingCount > 2)
                return false;
        } else {
            return false;
        }
    }
    if (s.length() % 4 == 1)
        return false;
    return true;
}

/**
 * @brief Giải mã mã hóa Hex và URL-encoding, đồng thời dọn dẹp kết quả.
 * 
 * @param input Chuỗi đầu vào có thể bị mã hóa.
 * @return std::string Chuỗi đã được giải mã và làm sạch.
 */
static std::string decodeObfuscationClean(const std::string& input) {
    // 1. Giải mã hex (\xXX) và URL-encoding (%XX)
    std::string decodedHex;
    size_t i = 0;
    while (i < input.length()) {
        if (input[i] == '\\' && i + 3 < input.length() && (input[i + 1] == 'x' || input[i + 1] == 'X')) {
            std::string hexPart = input.substr(i + 2, 2);
            if (std::isxdigit(static_cast<unsigned char>(hexPart[0])) && std::isxdigit(static_cast<unsigned char>(hexPart[1]))) {
                try {
                    char chr = static_cast<char>(std::stoi(hexPart, nullptr, 16));
                    decodedHex.push_back(chr);
                } catch (...) {
                    decodedHex.push_back(input[i]);
                    decodedHex.push_back(input[i + 1]);
                    decodedHex.push_back(input[i + 2]);
                    decodedHex.push_back(input[i + 3]);
                }
            } else {
                decodedHex.push_back(input[i]);
                decodedHex.push_back(input[i + 1]);
                decodedHex.push_back(input[i + 2]);
                decodedHex.push_back(input[i + 3]);
            }
            i += 4;
        } else if (input[i] == '%' && i + 2 < input.length() && std::isxdigit(input[i + 1]) && std::isxdigit(input[i + 2])) {
            std::string hexPart = input.substr(i + 1, 2);
            try {
                char chr = static_cast<char>(std::stoi(hexPart, nullptr, 16));
                decodedHex.push_back(chr);
            } catch (...) {
                decodedHex.push_back(input[i]);
                decodedHex.push_back(input[i + 1]);
                decodedHex.push_back(input[i + 2]);
            }
            i += 3;
        } else {
            decodedHex.push_back(input[i]);
            i++;
        }
    }

    // 2. Nếu toàn bộ chuỗi đã được giải mã hex là Base64 hợp lệ, trả về kết quả giải mã Base64 sạch
    if (isBase64(decodedHex)) {
        std::string decoded = decodeBase64(decodedHex);
        int printable = 0;
        for (char dc : decoded) {
            if (std::isprint(static_cast<unsigned char>(dc)) || std::isspace(static_cast<unsigned char>(dc))) {
                printable++;
            }
        }
        if (!decoded.empty() && printable == (int)decoded.length()) {
            return decoded;
        }
    }
    return decodedHex;
}

/**
 * @brief Giải mã các dạng làm rối (obfuscation) như Hex, URL-encoding và Base64.
 * 
 * @param input Chuỗi đầu vào có thể bị làm rối.
 * @return std::string Chuỗi đã được giải mã đầy đủ.
 */
static std::string decodeObfuscation(const std::string& input) {
    // 1. Giải mã hex (\xXX) và URL-encoding (%XX)
    std::string decodedHex;
    size_t i = 0;
    while (i < input.length()) {
        if (input[i] == '\\' && i + 3 < input.length() && (input[i + 1] == 'x' || input[i + 1] == 'X')) {
            std::string hexPart = input.substr(i + 2, 2);
            if (std::isxdigit(static_cast<unsigned char>(hexPart[0])) && std::isxdigit(static_cast<unsigned char>(hexPart[1]))) {
                try {
                    char chr = static_cast<char>(std::stoi(hexPart, nullptr, 16));
                    decodedHex.push_back(chr);
                } catch (...) {
                    decodedHex.push_back(input[i]);
                    decodedHex.push_back(input[i + 1]);
                    decodedHex.push_back(input[i + 2]);
                    decodedHex.push_back(input[i + 3]);
                }
            } else {
                decodedHex.push_back(input[i]);
                decodedHex.push_back(input[i + 1]);
                decodedHex.push_back(input[i + 2]);
                decodedHex.push_back(input[i + 3]);
            }
            i += 4;
        } else if (input[i] == '%' && i + 2 < input.length() && std::isxdigit(input[i + 1]) && std::isxdigit(input[i + 2])) {
            std::string hexPart = input.substr(i + 1, 2);
            try {
                char chr = static_cast<char>(std::stoi(hexPart, nullptr, 16));
                decodedHex.push_back(chr);
            } catch (...) {
                decodedHex.push_back(input[i]);
                decodedHex.push_back(input[i + 1]);
                decodedHex.push_back(input[i + 2]);
            }
            i += 3;
        } else {
            decodedHex.push_back(input[i]);
            i++;
        }
    }

    // 2. Tìm và giải mã từ Base64
    std::string result;
    std::string currentWord;
    for (char c : decodedHex) {
        if (std::isalnum(c) || c == '+' || c == '/' || c == '=') {
            currentWord += c;
        } else {
            if (!currentWord.empty()) {
                if (isBase64(currentWord)) {
                    std::string decoded = decodeBase64(currentWord);
                    int printable = 0;
                    for (char dc : decoded) {
                        if (std::isprint(static_cast<unsigned char>(dc)) || std::isspace(static_cast<unsigned char>(dc))) {
                            printable++;
                        }
                    }
                    if (!decoded.empty() && printable == (int)decoded.length()) {
                        result += currentWord + " (Decoded: \"" + decoded + "\")";
                    } else {
                        result += currentWord;
                    }
                } else {
                    result += currentWord;
                }
                currentWord.clear();
            }
            result += c;
        }
    }
    if (!currentWord.empty()) {
        if (isBase64(currentWord)) {
            std::string decoded = decodeBase64(currentWord);
            int printable = 0;
            for (char dc : decoded) {
                if (std::isprint(static_cast<unsigned char>(dc)) || std::isspace(static_cast<unsigned char>(dc))) {
                    printable++;
                }
            }
            if (!decoded.empty() && printable == (int)decoded.length()) {
                result += currentWord + " (Decoded: \"" + decoded + "\")";
            } else {
                result += currentWord;
            }
        } else {
            result += currentWord;
        }
    }
    return result;
}

/**
 * @brief Phân tích một dòng log thô thành cấu trúc Log.
 * 
 * @param rawLine Chuỗi log thô đọc từ file.
 * @param out Biến tham chiếu lưu đối tượng Log sau khi phân tích thành công.
 * @return true Nếu phân tích thành công.
 * @return false Nếu chuỗi log không đúng định dạng.
 */
bool LogParser::parse(const std::string& rawLine, Log& out) {
    // 1. Giới hạn độ dài dòng log tối đa 8KB (8192 ký tự) để chống tấn công OOM/Denial of Service
    std::string line = rawLine;
    if (line.size() > 8192) {
        line = line.substr(0, 8192);
    }

    // 2. Thay thế ký tự xuống dòng ẩn để ngăn chặn Log Injection/Forging
    std::string cleanLine;
    cleanLine.reserve(line.size());
    for (char c : line) {
        if (c == '\n' || c == '\r') {
            cleanLine += " [NL] ";
        } else {
            cleanLine += c;
        }
    }
    line = cleanLine;

    // Chiều dài dòng tối thiểu hợp lệ: [x] [x] [x] x (7 ký tự)
    if (line.size() < 7 || line[0] != '[') {
        return false;
    }

    size_t pos = 0;

    // --- Trích xuất nhãn thời gian: nằm trong cặp ngoặc [...] đầu tiên ---
    size_t open1 = line.find('[', pos);
    size_t close1 = line.find(']', open1);
    if (open1 == std::string::npos || close1 == std::string::npos) {
        return false;
    }
    out.timestamp = line.substr(open1 + 1, close1 - open1 - 1);
    pos = close1 + 1;

    // --- Trích xuất tên dịch vụ: nằm trong cặp ngoặc [...] thứ hai ---
    size_t open2 = line.find('[', pos);
    size_t close2 = line.find(']', open2);
    if (open2 == std::string::npos || close2 == std::string::npos) {
        return false;
    }
    out.serviceID = line.substr(open2 + 1, close2 - open2 - 1);
    pos = close2 + 1;

    // --- Trích xuất mức độ nghiêm trọng: nằm trong cặp ngoặc [...] thứ ba ---
    size_t open3 = line.find('[', pos);
    size_t close3 = line.find(']', open3);
    if (open3 == std::string::npos || close3 == std::string::npos) {
        return false;
    }
    out.severity = decodeObfuscation(line.substr(open3 + 1, close3 - open3 - 1));
    pos = close3 + 1;

    // --- Trích xuất thông điệp: phần còn lại sau dấu ] thứ ba (bỏ qua khoảng trắng đầu) ---
    if (pos < line.size() && line[pos] == ' ') {
        pos++;
    }
    out.message = decodeObfuscation(line.substr(pos));

    // --- Trích xuất IP và Username bằng biểu thức chính quy (Regex) ---
    static const std::regex ipRegex(R"(\b(?:\d{1,3}\.){3}\d{1,3}\b)");
    static const std::regex userRegex(R"((?:user\s*=\s*|user:|user\s+)([a-zA-Z0-9_\-\.]+))", std::regex_constants::icase);

    // Tiền lọc (Pre-filtering) để tránh ReDoS và tối ưu hiệu năng
    bool hasMaybeIP = (out.message.find('.') != std::string::npos);
    bool hasMaybeUser = false;

    std::string lowerMessage = out.message;
    for (char& c : lowerMessage)
        c = std::tolower(c);
    if (lowerMessage.find("user") != std::string::npos) {
        hasMaybeUser = true;
    }

    std::smatch ipMatch;
    if (hasMaybeIP && std::regex_search(out.message, ipMatch, ipRegex)) {
        out.sourceIP = ipMatch.str(0);
    } else {
        out.sourceIP = "";
    }

    std::smatch userMatch;
    if (hasMaybeUser && std::regex_search(out.message, userMatch, userRegex)) {
        if (userMatch.size() > 1) {
            out.username = decodeObfuscationClean(userMatch.str(1));
        }
    } else {
        out.username = "";
    }

    return true;
}
