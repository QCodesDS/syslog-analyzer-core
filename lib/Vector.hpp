/**
 * @file Vector.hpp
 * @brief Cài đặt cấu trúc dữ liệu mảng động (Dynamic Array) mô phỏng lại std::vector.
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <algorithm>
#include <stdexcept>

/**
 * @class Vector
 * @brief Mảng động tự co giãn kích thước, lưu trữ các phần tử có cùng kiểu dữ liệu.
 * @tparam T Kiểu dữ liệu của các phần tử trong Vector.
 */
template<typename T>
class Vector {
private:
    int capacity; /**< @brief Sức chứa hiện tại của mảng (số lượng phần tử tối đa trước khi phải cấp phát lại). */
    int size;     /**< @brief Số lượng phần tử thực tế đang lưu trữ. */
    T* arr;       /**< @brief Con trỏ quản lý vùng nhớ cấp phát động. */

public:
    static constexpr int DEFAULT_CAPACITY = 100; /**< @brief Sức chứa mặc định khi khởi tạo Vector rỗng. */
    static constexpr int EXPAND_COEFFICIENT = 2; /**< @brief Hệ số mở rộng sức chứa khi mảng đầy. */

    /**
     * @brief Lấy số lượng phần tử hiện tại.
     * @return int Số phần tử.
     */
    int getSize() const { return size; }

    /**
     * @brief Khởi tạo Vector với một số lượng phần tử và giá trị mặc định.
     * @param len Số lượng phần tử ban đầu.
     * @param defaultValue Giá trị dùng để khởi tạo tất cả các phần tử.
     */
    Vector(int len, const T& defaultValue) {
        this->capacity = std::max(len, DEFAULT_CAPACITY);
        this->arr = new T[this->capacity];
        this->size = len;
        for (int i = 0; i < len; i++) {
            this->arr[i] = defaultValue;
        }
    }

    /**
     * @brief Khởi tạo Vector rỗng với sức chứa mặc định.
     */
    Vector() {
        this->arr = new T[DEFAULT_CAPACITY];
        this->capacity = DEFAULT_CAPACITY;
        this->size = 0;
    }

    /**
     * @brief Constructor sao chép (Copy constructor).
     * @param other Đối tượng Vector cần sao chép.
     */
    Vector(const Vector& other) {
        this->capacity = other.capacity;
        this->size = other.size;
        this->arr = new T[this->capacity];
        for (int i = 0; i < this->size; i++) {
            this->arr[i] = other.arr[i];
        }
    }

    /**
     * @brief Toán tử gán (Assignment operator).
     * @param other Đối tượng Vector cần gán.
     * @return Vector& Tham chiếu tới chính đối tượng này sau khi gán.
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            freeMemory();
            this->capacity = other.capacity;
            this->size = other.size;
            if (this->capacity > 0) {
                this->arr = new T[this->capacity];
                for (int i = 0; i < this->size; i++) {
                    this->arr[i] = other.arr[i];
                }
            } else {
                this->arr = nullptr;
            }
        }
        return *this;
    }

    /**
     * @brief Thêm một phần tử vào cuối Vector. Nếu đầy, sức chứa sẽ tự động nhân đôi.
     * @param value Giá trị cần thêm.
     */
    void pushBack(const T& value) {
        if ((this->size + 1) > this->capacity) {
            int oldSize = this->size;
            this->capacity *= EXPAND_COEFFICIENT;
            T* newArr = new T[this->capacity];
            for (int i = 0; i < oldSize; i++) {
                newArr[i] = this->arr[i];
            }
            delete[] this->arr;
            this->arr = newArr;
        }
        this->arr[this->size] = value;
        this->size++;
    }

    /**
     * @brief Xóa phần tử cuối cùng ra khỏi Vector.
     */
    void popBack() { this->size = std::max(this->size - 1, 0); }

    /**
     * @brief Truy cập phần tử theo chỉ số (cho phép sửa đổi).
     * @param idx Chỉ số cần truy cập.
     * @return T& Tham chiếu đến phần tử.
     * @throw std::runtime_error Nếu chỉ số vượt ra ngoài phạm vi.
     */
    T& operator[](int idx) {
        if (idx >= 0 && idx < this->size) {
            return this->arr[idx];
        }
        throw std::runtime_error("Chỉ số vượt ra ngoài phạm vi");
    }

    /**
     * @brief Truy cập phần tử theo chỉ số (chỉ đọc).
     * @param idx Chỉ số cần truy cập.
     * @return const T& Tham chiếu hằng đến phần tử.
     * @throw std::runtime_error Nếu chỉ số vượt ra ngoài phạm vi.
     */
    const T& operator[](int idx) const {
        if (idx >= 0 && idx < this->size) {
            return this->arr[idx];
        }
        throw std::runtime_error("Chỉ số vượt ra ngoài phạm vi");
    }

    /**
     * @brief Hủy đối tượng Vector và giải phóng bộ nhớ.
     */
    ~Vector() { freeMemory(); }

    /**
     * @brief Làm rỗng Vector (đặt kích thước về 0, giữ nguyên sức chứa).
     */
    void clear() { this->size = 0; }

    /**
     * @brief Giải phóng hoàn toàn bộ nhớ đã cấp phát.
     */
    void freeMemory() {
        if (this->arr) {
            delete[] arr;
            this->arr = nullptr;
            this->capacity = 0;
            this->size = 0;
        }
    }
};

#endif  // VECTOR_HPP
