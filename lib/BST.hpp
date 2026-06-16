/**
 * @file BST.hpp
 * @brief Cài đặt cấu trúc dữ liệu Cây Tìm kiếm Nhị phân (Binary Search Tree).
 */

#ifndef BST_HPP
#define BST_HPP

#include "Vector.hpp"

/**
 * @struct BSTNode
 * @brief Nút (Node) của cây tìm kiếm nhị phân.
 * @tparam T Kiểu dữ liệu của giá trị lưu trong nút.
 */
template<typename T>
struct BSTNode {
    /// @brief Giá trị của nút.
    T value;
    /// @brief Con trỏ tới cây con trái.
    BSTNode* left;
    /// @brief Con trỏ tới cây con phải.
    BSTNode* right;
    /// @brief Chiều cao (dùng lại cho AVL).
    int height;

    /**
     * @brief Constructor khởi tạo nút với một giá trị.
     * @param val Giá trị để khởi tạo nút.
     */
    BSTNode(const T& val) : value(val), left(nullptr), right(nullptr), height(0) {}
};

/**
 * @class BST
 * @brief Lớp triển khai Cây Tìm kiếm Nhị phân.
 * @tparam T Kiểu dữ liệu của phần tử.
 */
template<typename T>
class BST {
private:
    /// @brief Con trỏ tới gốc của cây.
    BSTNode<T>* root;

public:
    /**
     * @brief Lấy con trỏ tới nút gốc.
     * @return BSTNode<T>* Con trỏ gốc của cây.
     */
    BSTNode<T>* getRoot() const { return root; }

    /**
     * @brief Khởi tạo cây rỗng.
     */
    BST() : root(nullptr) {}

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    /**
     * @brief Hủy cây và giải phóng bộ nhớ.
     */
    ~BST() { clearTree(); }

    /**
     * @brief Xóa sạch toàn bộ cây.
     */
    void clearTree() {
        clear(root);
        root = nullptr;
    }

    /**
     * @brief Chèn một giá trị vào cây.
     * @param value Giá trị cần chèn.
     */
    void insert(const T& value) { root = insertNode(root, value); }

    /**
     * @brief Xóa một giá trị khỏi cây.
     * @param value Giá trị cần xóa.
     */
    void remove(const T& value) { root = removeNode(root, value); }

    /**
     * @brief Tìm kiếm một giá trị trong cây.
     * @param value Giá trị cần tìm.
     * @return true Nếu tìm thấy.
     * @return false Nếu không tìm thấy.
     */
    bool search(const T& value) const { return searchNode(root, value); }

    /**
     * @brief Duyệt cây theo thứ tự LNR (In-order).
     * @return Vector<T> Danh sách phần tử đã duyệt.
     */
    Vector<T> lnr() const {
        Vector<T> result;
        inOrder(root, result);
        return result;
    }

    /**
     * @brief Duyệt cây theo thứ tự NLR (Pre-order).
     * @return Vector<T> Danh sách phần tử đã duyệt.
     */
    Vector<T> nlr() const {
        Vector<T> result;
        preOrder(root, result);
        return result;
    }

    /**
     * @brief Duyệt cây theo thứ tự LRN (Post-order).
     * @return Vector<T> Danh sách phần tử đã duyệt.
     */
    Vector<T> lrn() const {
        Vector<T> result;
        postOrder(root, result);
        return result;
    }

private:
    /**
     * @brief Hàm hỗ trợ để xóa sạch cây con.
     * @param node Nút gốc của cây con.
     */
    void clear(BSTNode<T>* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    /**
     * @brief Hàm hỗ trợ chèn một nút mới.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần chèn.
     * @return BSTNode<T>* Nút gốc mới của cây con.
     */
    BSTNode<T>* insertNode(BSTNode<T>* node, const T& value) {
        if (!node)
            return new BSTNode<T>(value);
        if (value < node->value) {
            node->left = insertNode(node->left, value);
        } else if (value > node->value) {
            node->right = insertNode(node->right, value);
        }
        return node;
    }

    /**
     * @brief Hàm hỗ trợ tìm nút nhỏ nhất trong cây con.
     * @param node Nút gốc của cây con.
     * @return BSTNode<T>* Nút chứa giá trị nhỏ nhất.
     */
    BSTNode<T>* findMin(BSTNode<T>* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    /**
     * @brief Hàm hỗ trợ xóa một nút.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần xóa.
     * @return BSTNode<T>* Nút gốc mới của cây con.
     */
    BSTNode<T>* removeNode(BSTNode<T>* node, const T& value) {
        if (!node)
            return nullptr;

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        } else if (value > node->value) {
            node->right = removeNode(node->right, value);
        } else {
            if (!node->left) {
                BSTNode<T>* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (!node->right) {
                BSTNode<T>* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                BSTNode<T>* minNode = findMin(node->right);
                node->value = minNode->value;
                node->right = removeNode(node->right, minNode->value);
            }
        }
        return node;
    }

    /**
     * @brief Hàm hỗ trợ tìm kiếm giá trị.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần tìm.
     * @return true Nếu tìm thấy.
     * @return false Nếu không tìm thấy.
     */
    bool searchNode(BSTNode<T>* node, const T& value) const {
        if (!node)
            return false;
        if (value == node->value)
            return true;
        if (value < node->value)
            return searchNode(node->left, value);
        return searchNode(node->right, value);
    }

    /**
     * @brief Hàm hỗ trợ duyệt cây LNR.
     * @param node Nút gốc của cây con.
     * @param vec Vector kết quả.
     */
    void inOrder(BSTNode<T>* node, Vector<T>& vec) const {
        if (node) {
            inOrder(node->left, vec);
            vec.pushBack(node->value);
            inOrder(node->right, vec);
        }
    }

    /**
     * @brief Hàm hỗ trợ duyệt cây NLR.
     * @param node Nút gốc của cây con.
     * @param vec Vector kết quả.
     */
    void preOrder(BSTNode<T>* node, Vector<T>& vec) const {
        if (node) {
            vec.pushBack(node->value);
            preOrder(node->left, vec);
            preOrder(node->right, vec);
        }
    }

    /**
     * @brief Hàm hỗ trợ duyệt cây LRN.
     * @param node Nút gốc của cây con.
     * @param vec Vector kết quả.
     */
    void postOrder(BSTNode<T>* node, Vector<T>& vec) const {
        if (node) {
            postOrder(node->left, vec);
            postOrder(node->right, vec);
            vec.pushBack(node->value);
        }
    }
};

#endif  // BST_HPP
