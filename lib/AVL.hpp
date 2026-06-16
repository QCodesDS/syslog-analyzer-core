/**
 * @file AVL.hpp
 * @brief Cài đặt cấu trúc dữ liệu Cây Tìm kiếm Nhị phân Tự cân bằng (AVL Tree).
 */

#ifndef AVL_HPP
#define AVL_HPP

#include "Vector.hpp"

/**
 * @struct AVLNode
 * @brief Nút (Node) của cây AVL.
 * @tparam T Kiểu dữ liệu của giá trị lưu trong nút.
 */
template<typename T>
struct AVLNode {
    /// @brief Giá trị của nút.
    T value;
    /// @brief Con trỏ tới cây con trái.
    AVLNode* left;
    /// @brief Con trỏ tới cây con phải.
    AVLNode* right;
    /// @brief Chiều cao của nút.
    int height;

    AVLNode(const T& val) : value(val), left(nullptr), right(nullptr), height(0) {}
};

/**
 * @class AVL
 * @brief Cây AVL tự cân bằng.
 * @tparam T Kiểu dữ liệu của các phần tử.
 */
template<typename T>
class AVL {
private:
    /// @brief Con trỏ tới gốc của cây.
    AVLNode<T>* root;

public:
    /**
     * @brief Lấy con trỏ tới nút gốc.
     * @return AVLNode<T>* Nút gốc của cây.
     */
    AVLNode<T>* getRoot() const { return root; }

    /**
     * @brief Lấy chiều cao của cây.
     * @return int Chiều cao của cây.
     */
    int height() const { return getHeight(root); }

    /**
     * @brief Khởi tạo cây AVL rỗng.
     */
    AVL() : root(nullptr) {}

    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    /**
     * @brief Hủy cây và giải phóng bộ nhớ.
     */
    ~AVL() { clearTree(); }

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
     * @brief Kiểm tra xem một giá trị có tồn tại trong cây hay không.
     * @param value Giá trị cần tìm.
     * @return true Nếu tìm thấy.
     */
    bool search(const T& value) const { return searchNode(root, value); }

    /**
     * @brief Tìm và trả về con trỏ tới giá trị trong cây.
     * @param value Giá trị cần tìm.
     * @return T* Con trỏ tới giá trị, hoặc nullptr nếu không tìm thấy.
     */
    T* find(const T& value) { return findNode(root, value); }

    /**
     * @brief Duyệt cây theo thứ tự LNR (Left-Node-Right) để lấy danh sách các phần tử tăng dần.
     * @return Vector<T> Danh sách phần tử.
     */
    Vector<T> lnr() const {
        Vector<T> result;
        inOrder(root, result);
        return result;
    }

    /**
     * @brief Duyệt cây theo thứ tự NLR (Node-Left-Right).
     * @return Vector<T> Danh sách phần tử.
     */
    Vector<T> nlr() const {
        Vector<T> result;
        preOrder(root, result);
        return result;
    }

    /**
     * @brief Duyệt cây theo thứ tự LRN (Left-Right-Node).
     * @return Vector<T> Danh sách phần tử.
     */
    Vector<T> lrn() const {
        Vector<T> result;
        postOrder(root, result);
        return result;
    }

private:
    /**
     * @brief Lấy chiều cao của một nút.
     * @param node Nút cần lấy chiều cao.
     * @return int Chiều cao của nút.
     */
    int getHeight(AVLNode<T>* node) const { return node ? node->height : -1; }

    /**
     * @brief Lấy hệ số cân bằng của một nút.
     * @param node Nút cần lấy hệ số cân bằng.
     * @return int Hệ số cân bằng.
     */
    int getBalanceFactor(AVLNode<T>* node) const {
        if (!node)
            return 0;
        return getHeight(node->right) - getHeight(node->left);
    }

    /**
     * @brief Tính toán lại chiều cao của một nút dựa trên các nút con.
     * @param node Nút cần tính lại chiều cao.
     */
    void recalculateHeight(AVLNode<T>* node) {
        if (node) {
            int leftH = getHeight(node->left);
            int rightH = getHeight(node->right);
            node->height = 1 + (leftH > rightH ? leftH : rightH);
        }
    }

    /**
     * @brief Xoay phải cây tại một nút chốt.
     * @param pivotNode Nút chốt.
     * @return AVLNode<T>* Nút gốc mới sau khi xoay.
     */
    AVLNode<T>* rotateRight(AVLNode<T>* pivotNode) {
        AVLNode<T>* leftChild = pivotNode->left;
        AVLNode<T>* rightOfLeftChild = leftChild->right;

        leftChild->right = pivotNode;
        pivotNode->left = rightOfLeftChild;

        recalculateHeight(pivotNode);
        recalculateHeight(leftChild);

        return leftChild;
    }

    /**
     * @brief Xoay trái cây tại một nút chốt.
     * @param pivotNode Nút chốt.
     * @return AVLNode<T>* Nút gốc mới sau khi xoay.
     */
    AVLNode<T>* rotateLeft(AVLNode<T>* pivotNode) {
        AVLNode<T>* rightChild = pivotNode->right;
        AVLNode<T>* leftOfRightChild = rightChild->left;

        rightChild->left = pivotNode;
        pivotNode->right = leftOfRightChild;

        recalculateHeight(pivotNode);
        recalculateHeight(rightChild);

        return rightChild;
    }

    /**
     * @brief Cân bằng lại cây tại một nút.
     * @param node Nút cần cân bằng.
     * @return AVLNode<T>* Nút gốc mới của cây con sau khi cân bằng.
     */
    AVLNode<T>* rebalance(AVLNode<T>* node) {
        recalculateHeight(node);
        int balance = getBalanceFactor(node);

        if (balance > 1) {
            if (getBalanceFactor(node->right) < 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        if (balance < -1) {
            if (getBalanceFactor(node->left) > 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        return node;
    }

    /**
     * @brief Chèn một nút mới vào cây con.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần chèn.
     * @return AVLNode<T>* Nút gốc mới của cây con.
     */
    AVLNode<T>* insertNode(AVLNode<T>* node, const T& value) {
        if (!node)
            return new AVLNode<T>(value);
        if (value < node->value) {
            node->left = insertNode(node->left, value);
        } else if (value > node->value) {
            node->right = insertNode(node->right, value);
        } else {
            return node;
        }
        return rebalance(node);
    }

    /**
     * @brief Tìm nút có giá trị nhỏ nhất trong cây con.
     * @param node Nút gốc của cây con.
     * @return AVLNode<T>* Nút chứa giá trị nhỏ nhất.
     */
    AVLNode<T>* findMin(AVLNode<T>* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    /**
     * @brief Xóa một nút khỏi cây con.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần xóa.
     * @return AVLNode<T>* Nút gốc mới của cây con sau khi xóa.
     */
    AVLNode<T>* removeNode(AVLNode<T>* node, const T& value) {
        if (!node)
            return nullptr;

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        } else if (value > node->value) {
            node->right = removeNode(node->right, value);
        } else {
            if (!node->left) {
                AVLNode<T>* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (!node->right) {
                AVLNode<T>* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                AVLNode<T>* minNode = findMin(node->right);
                node->value = minNode->value;
                node->right = removeNode(node->right, minNode->value);
            }
        }
        return rebalance(node);
    }

    /**
     * @brief Xóa sạch cây con giải phóng bộ nhớ.
     * @param node Nút gốc của cây con.
     */
    void clear(AVLNode<T>* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    /**
     * @brief Tìm kiếm một giá trị trong cây con.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần tìm.
     * @return true Nếu tìm thấy.
     * @return false Nếu không tìm thấy.
     */
    bool searchNode(AVLNode<T>* node, const T& value) const {
        if (!node)
            return false;
        if (value == node->value)
            return true;
        if (value < node->value)
            return searchNode(node->left, value);
        return searchNode(node->right, value);
    }

    /**
     * @brief Tìm và trả về con trỏ tới giá trị trong cây con.
     * @param node Nút gốc của cây con.
     * @param value Giá trị cần tìm.
     * @return T* Con trỏ tới giá trị, hoặc nullptr.
     */
    T* findNode(AVLNode<T>* node, const T& value) {
        if (!node)
            return nullptr;
        if (value == node->value)
            return &(node->value);
        if (value < node->value)
            return findNode(node->left, value);
        return findNode(node->right, value);
    }

    /**
     * @brief Duyệt cây theo thứ tự LNR (In-order).
     * @param node Nút gốc của cây con.
     * @param vec Vector lưu trữ kết quả duyệt.
     */
    void inOrder(AVLNode<T>* node, Vector<T>& vec) const {
        if (node) {
            inOrder(node->left, vec);
            vec.pushBack(node->value);
            inOrder(node->right, vec);
        }
    }

    /**
     * @brief Duyệt cây theo thứ tự NLR (Pre-order).
     * @param node Nút gốc của cây con.
     * @param vec Vector lưu trữ kết quả duyệt.
     */
    void preOrder(AVLNode<T>* node, Vector<T>& vec) const {
        if (node) {
            vec.pushBack(node->value);
            preOrder(node->left, vec);
            preOrder(node->right, vec);
        }
    }

    /**
     * @brief Duyệt cây theo thứ tự LRN (Post-order).
     * @param node Nút gốc của cây con.
     * @param vec Vector lưu trữ kết quả duyệt.
     */
    void postOrder(AVLNode<T>* node, Vector<T>& vec) const {
        if (node) {
            postOrder(node->left, vec);
            postOrder(node->right, vec);
            vec.pushBack(node->value);
        }
    }
};

#endif  // AVL_HPP
