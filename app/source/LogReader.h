/**
 * @file LogReader.h
 * @brief Quản lý luồng đọc file log theo từng lô (batch).
 * 
 * Hỗ trợ đọc liên tục (tailing) từ một file text, lưu lại vị trí con trỏ cuối cùng.
 */

#ifndef LOGREADER_H
#define LOGREADER_H

#include <fstream>
#include <string>

#include "../../lib/Vector.hpp"

/**
 * @class LogReader
 * @brief Lớp cung cấp chức năng đọc file log theo cơ chế streaming/batching.
 */
class LogReader {
private:
    std::ifstream file;     /**< @brief Đối tượng luồng đầu vào đọc file. */
    std::streampos lastPos; /**< @brief Vị trí con trỏ tập tin sau lần đọc cuối để tiếp tục đọc (tailing). */

public:
    /**
     * @brief Khởi tạo LogReader và mở file cần phân tích.
     * @param filename Đường dẫn hoặc tên file log.
     */
    LogReader(const std::string& filename);

    /**
     * @brief Hủy đối tượng LogReader và đóng luồng file an toàn.
     */
    ~LogReader();

    /**
     * @brief Đọc một số lượng dòng log nhất định từ file.
     * 
     * @param batchSize Số lượng dòng tối đa cần đọc trong một lô (mặc định: 200).
     * @return Vector<std::string> Danh sách các dòng log vừa đọc được.
     */
    Vector<std::string> readBatch(int batchSize = 200);

    /**
     * @brief Kiểm tra xem luồng file còn khả dụng không.
     * @return true Nếu file đang mở và không bị lỗi phần cứng.
     */
    bool hasNext() const;

    /**
     * @brief Đóng luồng file hiện tại một cách thủ công.
     */
    void close();
};

#endif  // LOGREADER_H