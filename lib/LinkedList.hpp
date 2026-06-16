/**
 * @file LinkedList.hpp
 * @brief Cài đặt cấu trúc dữ liệu Danh sách Liên kết Đôi (Doubly Linked List).
 */

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <stdexcept>

/**
 * @struct LinkedListNode
 * @brief Nút (Node) của danh sách liên kết đôi.
 * @tparam T Kiểu dữ liệu của giá trị lưu trong nút.
 */
template<typename T>
struct LinkedListNode {
    /// @brief Giá trị của nút.
    T value;
    /// @brief Con trỏ tới nút tiếp theo.
    LinkedListNode* next;
    /// @brief Con trỏ tới nút trước đó.
    LinkedListNode* prev;

    /**
     * @brief Constructor khởi tạo nút với một giá trị.
     * @param val Giá trị để khởi tạo nút.
     */
    LinkedListNode(const T& val) : value(val), next(nullptr), prev(nullptr) {}
};

/**
 * @class LinkedList
 * @brief Lớp triển khai Danh sách Liên kết Đôi.
 * @tparam T Kiểu dữ liệu của phần tử.
 */
template<typename T>
class LinkedList {
private:
    /// @brief Con trỏ tới nút đầu tiên.
    LinkedListNode<T>* head;
    /// @brief Con trỏ tới nút cuối cùng.
    LinkedListNode<T>* tail;
    /// @brief Số lượng phần tử trong danh sách.
    int listSize;

public:
    /**
     * @brief Lấy con trỏ tới nút đầu tiên.
     * @return LinkedListNode<T>* Nút đầu tiên.
     */
    LinkedListNode<T>* getHead() const { return head; }

    /**
     * @brief Lấy con trỏ tới nút cuối cùng.
     * @return LinkedListNode<T>* Nút cuối cùng.
     */
    LinkedListNode<T>* getTail() const { return tail; }

    /**
     * @brief Khởi tạo một danh sách liên kết rỗng.
     */
    LinkedList() : head(nullptr), tail(nullptr), listSize(0) {}

    /**
     * @brief Hủy danh sách và giải phóng bộ nhớ.
     */
    ~LinkedList() { clear(); }

    /**
     * @brief Constructor sao chép (Deep copy).
     * @param other Danh sách cần sao chép.
     */
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), listSize(0) {
        LinkedListNode<T>* current = other.head;
        while (current) {
            insertBack(current->value);
            current = current->next;
        }
    }

    /**
     * @brief Toán tử gán (Deep copy).
     * @param other Danh sách cần gán.
     * @return LinkedList& Tham chiếu tới danh sách này.
     */
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            LinkedListNode<T>* current = other.head;
            while (current) {
                insertBack(current->value);
                current = current->next;
            }
        }
        return *this;
    }

    /**
     * @brief Move constructor.
     * @param other Danh sách nguồn để di chuyển.
     */
    LinkedList(LinkedList&& other) noexcept : head(other.head), tail(other.tail), listSize(other.listSize) {
        other.head = nullptr;
        other.tail = nullptr;
        other.listSize = 0;
    }

    /**
     * @brief Move assignment operator.
     * @param other Danh sách nguồn để di chuyển.
     * @return LinkedList& Tham chiếu tới danh sách này.
     */
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            listSize = other.listSize;
            other.head = nullptr;
            other.tail = nullptr;
            other.listSize = 0;
        }
        return *this;
    }

    /**
     * @brief Chèn một giá trị vào đầu danh sách.
     * @param value Giá trị cần chèn.
     */
    void insertFront(const T& value) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        listSize++;
    }

    /**
     * @brief Chèn một giá trị vào cuối danh sách.
     * @param value Giá trị cần chèn.
     */
    void insertBack(const T& value) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        listSize++;
    }

    /**
     * @brief Chèn một giá trị vào vị trí cụ thể.
     * @param index Vị trí chèn.
     * @param value Giá trị cần chèn.
     * @throw std::out_of_range Nếu vị trí vượt quá kích thước.
     */
    void insertAt(int index, const T& value) {
        if (index < 0 || index > listSize) {
            throw std::out_of_range("Chỉ số vượt quá giới hạn");
        }
        if (index == 0) {
            insertFront(value);
        } else if (index == listSize) {
            insertBack(value);
        } else {
            LinkedListNode<T>* current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
            newNode->prev = current->prev;
            newNode->next = current;
            current->prev->next = newNode;
            current->prev = newNode;
            listSize++;
        }
    }

    /**
     * @brief Xóa phần tử đầu tiên mang giá trị cụ thể.
     * @param value Giá trị cần xóa.
     * @return true Nếu tìm thấy và xóa thành công.
     * @return false Nếu không tìm thấy.
     */
    bool remove(const T& value) {
        LinkedListNode<T>* current = head;
        while (current) {
            if (current->value == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
                delete current;
                listSize--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    /**
     * @brief Xóa phần tử tại một vị trí cụ thể.
     * @param index Vị trí của phần tử cần xóa.
     * @throw std::out_of_range Nếu vị trí không hợp lệ.
     */
    void removeAt(int index) {
        if (index < 0 || index >= listSize) {
            throw std::out_of_range("Chỉ số vượt quá giới hạn");
        }
        LinkedListNode<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }
        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }
        delete current;
        listSize--;
    }

    /**
     * @brief Kiểm tra một giá trị có tồn tại trong danh sách hay không.
     * @param value Giá trị cần tìm kiếm.
     * @return true Nếu tìm thấy.
     * @return false Nếu không tìm thấy.
     */
    bool find(const T& value) const {
        LinkedListNode<T>* current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    /**
     * @brief Trả về số lượng phần tử hiện có trong danh sách.
     * @return int Kích thước danh sách.
     */
    int size() const { return listSize; }

    /**
     * @brief Xóa sạch toàn bộ phần tử trong danh sách.
     */
    void clear() {
        LinkedListNode<T>* current = head;
        while (current) {
            LinkedListNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        listSize = 0;
    }
};

#endif  // LINKEDLIST_HPP
