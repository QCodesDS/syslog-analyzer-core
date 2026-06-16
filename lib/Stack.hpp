/**
 * @file Stack.hpp
 * @brief Cài đặt cấu trúc dữ liệu Ngăn xếp (Stack) dựa trên Danh sách liên kết.
 */

#ifndef STACK_HPP
#define STACK_HPP

#include "LinkedList.hpp"

/**
 * @class Stack
 * @brief Lớp triển khai Ngăn xếp (LIFO).
 * @tparam T Kiểu dữ liệu của phần tử.
 */
template<typename T>
class Stack {
private:
    /// @brief Danh sách liên kết lưu trữ các phần tử của ngăn xếp.
    LinkedList<T> list;

public:
    /**
     * @brief Khởi tạo một ngăn xếp rỗng.
     */
    Stack() = default;

    /**
     * @brief Hủy đối tượng ngăn xếp (tự động gọi bộ hủy của LinkedList).
     */
    ~Stack() = default;

    /**
     * @brief Constructor sao chép (tự động sử dụng LinkedList copy constructor).
     * @param other Ngăn xếp cần sao chép.
     */
    Stack(const Stack& other) : list(other.list) {}

    /**
     * @brief Toán tử gán (tự động sử dụng LinkedList operator=).
     * @param other Ngăn xếp cần gán.
     * @return Stack& Tham chiếu tới ngăn xếp này.
     */
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    /**
     * @brief Đẩy một phần tử lên đỉnh ngăn xếp.
     * @param value Giá trị cần đẩy.
     */
    void push(const T& value) { list.insertFront(value); }

    /**
     * @brief Lấy và xóa phần tử ở đỉnh ngăn xếp.
     * @return true Nếu xóa thành công.
     * @return false Nếu ngăn xếp rỗng.
     */
    bool pop() {
        if (empty())
            return false;
        list.removeAt(0);
        return true;
    }

    /**
     * @brief Trả về tham chiếu tới phần tử ở đỉnh ngăn xếp.
     * @return T& Tham chiếu tới phần tử đỉnh.
     * @throw std::out_of_range Nếu ngăn xếp rỗng.
     */
    T& top() {
        if (empty()) {
            throw std::out_of_range("Ngăn xếp đang rỗng");
        }
        return list.getHead()->value;
    }

    /**
     * @brief Kiểm tra xem ngăn xếp có rỗng không.
     * @return true Nếu rỗng.
     * @return false Nếu có phần tử.
     */
    bool empty() const { return list.size() == 0; }

    /**
     * @brief Trả về số lượng phần tử trong ngăn xếp.
     * @return int Số phần tử.
     */
    int size() const { return list.size(); }
};

#endif  // STACK_HPP
