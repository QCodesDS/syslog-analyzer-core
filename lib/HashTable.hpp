/**
 * @file HashTable.hpp
 * @brief Cài đặt Bảng Băm (Hash Table) kết hợp xử lý xung đột bằng cây AVL (Chaining-based).
 */

#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "../app/config/Constants.h"
#include "AVL.hpp"

/**
 * @struct Pair
 * @brief Cấu trúc lưu trữ cặp Khóa - Giá trị (Key-Value) dùng cho bảng băm.
 * @tparam K Kiểu dữ liệu của khóa.
 * @tparam V Kiểu dữ liệu của giá trị.
 */
template<typename K, typename V>
struct Pair {
    K key;   /**< @brief Khóa định danh. */
    V value; /**< @brief Giá trị được lưu trữ. */

    Pair() = default;
    Pair(const K& k, const V& v) : key(k), value(v) {}
    Pair(const K& k) : key(k), value(V()) {}

    bool operator<(const Pair& other) const { return key < other.key; }
    bool operator>(const Pair& other) const { return key > other.key; }
    bool operator==(const Pair& other) const { return key == other.key; }
};

/**
 * @class HashTable
 * @brief Cấu trúc dữ liệu Bảng Băm, giải quyết đụng độ bằng mảng các cây AVL.
 * @tparam K Kiểu dữ liệu của khóa.
 * @tparam V Kiểu dữ liệu của giá trị.
 */
template<typename K, typename V>
class HashTable {
private:
    static const int TABLE_SIZE = HASHTABLE_SIZE; /**< @brief Số lượng bucket trong bảng băm. */
    AVL<Pair<K, V>> buckets[TABLE_SIZE];          /**< @brief Mảng các cây AVL đóng vai trò là các bucket. */
    int elementCount;                             /**< @brief Tổng số phần tử hiện có trong bảng băm. */

public:
    /**
     * @brief Khởi tạo bảng băm rỗng.
     */
    HashTable() : elementCount(0) {}

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    /**
     * @brief Hủy đối tượng bảng băm.
     */
    ~HashTable() {}

    /**
     * @brief Chèn một cặp Khóa-Giá trị vào bảng. Nếu Khóa đã tồn tại, cập nhật lại Giá trị.
     * @param pair Cặp dữ liệu cần chèn.
     */
    void insert(const Pair<K, V>& pair) {
        int index = getBucketIndex(pair.key);
        Pair<K, V>* existing = buckets[index].find(Pair<K, V>(pair.key));
        if (existing) {
            existing->value = pair.value;
        } else {
            buckets[index].insert(pair);
            elementCount++;
        }
    }

    /**
     * @brief Xóa một phần tử ra khỏi bảng băm thông qua khóa.
     * @param key Khóa cần xóa.
     * @return true Nếu phần tử tồn tại và đã được xóa.
     * @return false Nếu phần tử không tồn tại.
     */
    bool remove(const K& key) {
        int index = getBucketIndex(key);
        Pair<K, V> dummy(key);
        if (buckets[index].search(dummy)) {
            buckets[index].remove(dummy);
            elementCount--;
            return true;
        }
        return false;
    }

    /**
     * @brief Tìm kiếm một giá trị theo khóa.
     * @param key Khóa cần tìm.
     * @return V* Con trỏ trỏ tới giá trị nếu tìm thấy, hoặc nullptr nếu không tồn tại.
     */
    V* find(const K& key) {
        int index = getBucketIndex(key);
        Pair<K, V>* found = buckets[index].find(Pair<K, V>(key));
        if (found) {
            return &(found->value);
        }
        return nullptr;
    }

    /**
     * @brief Kiểm tra xem bảng băm có chứa một khóa cụ thể hay không.
     * @param key Khóa cần kiểm tra.
     * @return true Nếu khóa có tồn tại.
     */
    bool contains(const K& key) {
        int index = getBucketIndex(key);
        return buckets[index].search(Pair<K, V>(key));
    }

    /**
     * @brief Trả về số lượng phần tử hiện tại.
     * @return int Số phần tử.
     */
    int size() const { return elementCount; }

    /**
     * @brief Xóa toàn bộ dữ liệu trong bảng băm.
     */
    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            buckets[i].clearTree();
        }
        elementCount = 0;
    }

    /**
     * @brief Trả về danh sách tất cả các cặp dữ liệu trong bảng băm (duyệt LNR trên các bucket).
     * @return Vector<Pair<K, V>> Mảng động chứa tất cả các phần tử.
     */
    Vector<Pair<K, V>> lnr() {
        Vector<Pair<K, V>> result;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Vector<Pair<K, V>> bucketItems = buckets[i].lnr();
            for (int j = 0; j < bucketItems.getSize(); j++) {
                result.pushBack(bucketItems[j]);
            }
        }
        return result;
    }

private:
    /**
     * @brief Tính toán chỉ số bucket từ một khóa, sử dụng hàm băm chuẩn của C++.
     * @param key Khóa cần băm.
     * @return int Chỉ số của bucket (từ 0 đến TABLE_SIZE - 1).
     */
    int getBucketIndex(const K& key) const {
        size_t hashValue = std::hash<K>{}(key);
        return static_cast<int>(hashValue % static_cast<size_t>(TABLE_SIZE));
    }
};

#endif  // HASHTABLE_HPP
