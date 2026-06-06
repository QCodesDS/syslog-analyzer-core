/**
 * @file PriorityQueue.hpp
 * @brief Cài đặt Hàng đợi Ưu tiên (Priority Queue) dựa trên cấu trúc Binary Heap.
 */

#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include <functional>

#include "Vector.hpp"

/**
 * @class PriorityQueue
 * @brief Lớp tổng quát cho hàng đợi ưu tiên sử dụng cấu trúc Binary Heap.
 * 
 * Mặc định sử dụng std::less (tạo thành Max-Heap dựa trên phép toán nhỏ hơn).
 * 
 * @tparam T Kiểu dữ liệu của phần tử.
 * @tparam Comp Hàm đối chiếu để quyết định mức độ ưu tiên.
 */
template<typename T, typename Comp = std::less<T>>
class PriorityQueue {
private:
    Vector<T> elementsHeap; /**< @brief Mảng động dùng làm vùng chứa dữ liệu cho Heap. */
    Comp comparer;          /**< @brief Bộ so sánh để đánh giá độ ưu tiên. */

public:
    /**
     * @brief Khởi tạo một hàng đợi ưu tiên rỗng.
     */
    PriorityQueue() : elementsHeap(), comparer() {}

    /**
     * @brief Hủy đối tượng hàng đợi ưu tiên.
     */
    ~PriorityQueue() = default;

    /**
     * @brief Constructor sao chép (Copy constructor).
     * @param other Đối tượng cần sao chép.
     */
    PriorityQueue(const PriorityQueue& other) : elementsHeap(other.elementsHeap), comparer(other.comparer) {}

    /**
     * @brief Toán tử gán.
     * @param other Đối tượng cần gán.
     * @return PriorityQueue& Tham chiếu tới chính đối tượng này.
     */
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            elementsHeap = other.elementsHeap;
            comparer = other.comparer;
        }
        return *this;
    }

    /**
     * @brief Chèn một giá trị mới vào hàng đợi.
     * @param value Giá trị cần chèn.
     */
    void insert(const T& value) {
        elementsHeap.pushBack(value);
        heapifyUp(elementsHeap.getSize() - 1);
    }

    /**
     * @brief Lấy và xóa phần tử có độ ưu tiên cao nhất khỏi hàng đợi.
     * @throw std::out_of_range Nếu hàng đợi rỗng.
     */
    void extract() {
        if (empty()) {
            throw std::out_of_range("Hàng đợi ưu tiên đang rỗng");
        }
        elementsHeap[0] = elementsHeap[elementsHeap.getSize() - 1];
        elementsHeap.popBack();
        if (!empty()) {
            heapifyDown(0);
        }
    }

    /**
     * @brief Trả về tham chiếu tới phần tử có độ ưu tiên cao nhất mà không xóa nó.
     * @return T& Tham chiếu tới phần tử.
     * @throw std::out_of_range Nếu hàng đợi rỗng.
     */
    T& peek() {
        if (empty()) {
            throw std::out_of_range("Hàng đợi ưu tiên đang rỗng");
        }
        return elementsHeap[0];
    }

    /**
     * @brief Kiểm tra xem hàng đợi có rỗng hay không.
     * @return true Nếu rỗng.
     */
    bool empty() const { return elementsHeap.getSize() == 0; }

    /**
     * @brief Lấy số lượng phần tử hiện tại trong hàng đợi.
     * @return int Số phần tử.
     */
    int size() const { return elementsHeap.getSize(); }

private:
    /**
     * @brief Đẩy một phần tử lên phía trên cây để bảo đảm tính chất của Heap.
     * @param elementIndex Vị trí của phần tử cần đẩy.
     */
    void heapifyUp(int elementIndex) {
        while (elementIndex > 0) {
            int parentIndex = (elementIndex - 1) / 2;
            if (comparer(elementsHeap[parentIndex], elementsHeap[elementIndex])) {
                T temp = elementsHeap[parentIndex];
                elementsHeap[parentIndex] = elementsHeap[elementIndex];
                elementsHeap[elementIndex] = temp;
                elementIndex = parentIndex;
            } else {
                break;
            }
        }
    }

    /**
     * @brief Đẩy một phần tử xuống phía dưới cây để bảo đảm tính chất của Heap.
     * @param elementIndex Vị trí của phần tử cần đẩy xuống.
     */
    void heapifyDown(int elementIndex) {
        int totalElements = elementsHeap.getSize();
        while (true) {
            int leftIndex = 2 * elementIndex + 1;
            int rightIndex = 2 * elementIndex + 2;
            int largestIndex = elementIndex;

            if (leftIndex < totalElements && comparer(elementsHeap[largestIndex], elementsHeap[leftIndex])) {
                largestIndex = leftIndex;
            }
            if (rightIndex < totalElements && comparer(elementsHeap[largestIndex], elementsHeap[rightIndex])) {
                largestIndex = rightIndex;
            }
            if (largestIndex != elementIndex) {
                T temp = elementsHeap[elementIndex];
                elementsHeap[elementIndex] = elementsHeap[largestIndex];
                elementsHeap[largestIndex] = temp;
                elementIndex = largestIndex;
            } else {
                break;
            }
        }
    }
};

#endif  // PRIORITYQUEUE_HPP
