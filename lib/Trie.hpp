/**
 * @file Trie.hpp
 * @brief Cài đặt cấu trúc dữ liệu Trie (Prefix Tree) để tìm kiếm chuỗi nhanh chóng.
 */

#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>

/**
 * @struct TrieNode
 * @brief Nút đại diện cho một ký tự trong cây Trie.
 */
struct TrieNode {
    TrieNode* children[128]; /**< @brief Mảng con trỏ tới các nút con (hỗ trợ tập ký tự ASCII 128). */
    bool isEnd;              /**< @brief Đánh dấu nút này có phải là điểm kết thúc của một từ hay không. */

    /**
     * @brief Khởi tạo một nút Trie rỗng.
     */
    TrieNode() : isEnd(false) {
        for (int i = 0; i < 128; i++) {
            children[i] = nullptr;
        }
    }
};

/**
 * @class Trie
 * @brief Cấu trúc dữ liệu Trie giúp tìm kiếm chuỗi và tiền tố cực kỳ hiệu quả.
 */
class Trie {
private:
    TrieNode* root; /**< @brief Nút gốc của cây Trie. */

    /**
     * @brief Đệ quy xóa toàn bộ các nút con để giải phóng bộ nhớ.
     * @param node Nút đang xét.
     */
    void clearNode(TrieNode* node) {
        if (!node) {
            return;
        }
        for (int i = 0; i < 128; i++) {
            if (node->children[i]) {
                clearNode(node->children[i]);
            }
        }
        delete node;
    }

    /**
     * @brief Đệ quy sao chép một nút và toàn bộ các nút con của nó.
     * @param node Nút gốc cần sao chép.
     * @return TrieNode* Con trỏ tới nút mới được sao chép.
     */
    TrieNode* copyNode(TrieNode* node) {
        if (!node) {
            return nullptr;
        }
        TrieNode* newNode = new TrieNode();
        newNode->isEnd = node->isEnd;
        for (int i = 0; i < 128; i++) {
            if (node->children[i]) {
                newNode->children[i] = copyNode(node->children[i]);
            }
        }
        return newNode;
    }

public:
    /**
     * @brief Khởi tạo cây Trie rỗng.
     */
    Trie() { root = new TrieNode(); }

    /**
     * @brief Hủy đối tượng và giải phóng toàn bộ bộ nhớ của cây.
     */
    ~Trie() {
        clearNode(root);
        root = nullptr;
    }

    /**
     * @brief Constructor sao chép (Copy constructor).
     * @param other Cây Trie cần sao chép.
     */
    Trie(const Trie& other) { root = copyNode(other.root); }

    /**
     * @brief Toán tử gán.
     * @param other Cây Trie cần gán.
     * @return Trie& Tham chiếu tới chính đối tượng này.
     */
    Trie& operator=(const Trie& other) {
        if (this != &other) {
            clearNode(root);
            root = copyNode(other.root);
        }
        return *this;
    }

    /**
     * @brief Chèn một chuỗi (từ khóa) vào trong cây Trie.
     * @param word Từ khóa cần chèn.
     */
    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char character : word) {
            int charIndex = static_cast<unsigned char>(character);
            if (charIndex >= 128) {
                continue;
            }
            if (!current->children[charIndex]) {
                current->children[charIndex] = new TrieNode();
            }
            current = current->children[charIndex];
        }
        current->isEnd = true;
    }

    /**
     * @brief Tìm kiếm chính xác một từ trong cây Trie.
     * @param word Từ cần tìm.
     * @return true Nếu từ tồn tại.
     */
    bool search(const std::string& word) const {
        TrieNode* current = root;
        for (char character : word) {
            int charIndex = static_cast<unsigned char>(character);
            if (charIndex >= 128) {
                continue;
            }
            if (!current->children[charIndex]) {
                return false;
            }
            current = current->children[charIndex];
        }
        return current != nullptr && current->isEnd;
    }

    /**
     * @brief Kiểm tra xem có bất kỳ từ khóa nào trong cây bắt đầu bằng tiền tố cho trước hay không.
     * @param prefix Chuỗi tiền tố cần kiểm tra.
     * @return true Nếu có ít nhất một từ bắt đầu bằng tiền tố này.
     */
    bool startsWith(const std::string& prefix) const {
        TrieNode* current = root;
        for (char character : prefix) {
            int charIndex = static_cast<unsigned char>(character);
            if (charIndex >= 128) {
                continue;
            }
            if (!current->children[charIndex]) {
                return false;
            }
            current = current->children[charIndex];
        }
        return current != nullptr;
    }

    /**
     * @brief Xóa sạch toàn bộ dữ liệu trong cây Trie.
     */
    void clear() {
        clearNode(root);
        root = new TrieNode();
    }
};

#endif  // TRIE_HPP