/**
 * @file Queue.hpp
 * @brief Cài đặt cấu trúc dữ liệu Hàng đợi (Queue) dựa trên Danh sách liên kết.
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "LinkedList.hpp"

/**
 * @class Queue
 * @brief Lớp triển khai Hàng đợi (FIFO).
 * @tparam T Kiểu dữ liệu của phần tử.
 */
template<typename T>
class Queue {
private:
    /// @brief Danh sách liên kết lưu trữ các phần tử của hàng đợi.
    LinkedList<T> list;

public:
    /**
     * @brief Khởi tạo một hàng đợi rỗng.
     */
    Queue() = default;

    /**
     * @brief Hủy đối tượng hàng đợi (tự động gọi bộ hủy của LinkedList).
     */
    ~Queue() = default;

    /**
     * @brief Constructor sao chép.
     * @param other Hàng đợi cần sao chép.
     */
    Queue(const Queue& other) : list(other.list) {}

    /**
     * @brief Toán tử gán.
     * @param other Hàng đợi cần gán.
     * @return Queue& Tham chiếu tới hàng đợi này.
     */
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    /**
     * @brief Thêm một phần tử vào cuối hàng đợi.
     * @param value Giá trị cần thêm.
     */
    void enqueue(const T& value) { list.insertBack(value); }

    /**
     * @brief Lấy và xóa phần tử ở đầu hàng đợi.
     * @return true Nếu xóa thành công.
     * @return false Nếu hàng đợi rỗng.
     */
    bool dequeue() {
        if (empty())
            return false;
        list.removeAt(0);
        return true;
    }

    /**
     * @brief Trả về tham chiếu hằng tới phần tử ở đầu hàng đợi.
     * @return const T& Tham chiếu tới phần tử đầu.
     * @throw std::out_of_range Nếu hàng đợi rỗng.
     */
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Hàng đợi đang rỗng");
        }
        return list.getHead()->value;
    }

    /**
     * @brief Kiểm tra xem hàng đợi có rỗng không.
     * @return true Nếu rỗng.
     * @return false Nếu có phần tử.
     */
    bool empty() const { return list.size() == 0; }

    /**
     * @brief Trả về số lượng phần tử trong hàng đợi.
     * @return int Số phần tử.
     */
    int size() const { return list.size(); }
};

#endif  // QUEUE_HPP
