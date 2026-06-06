#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>

#include "AVL.hpp"

template<typename K, typename V>
struct Pair {
    K key;
    V value;

    Pair() = default;
    Pair(const K& k, const V& v) : key(k), value(v) {}
    Pair(const K& k) : key(k), value(V()) {}  // For searching

    bool operator<(const Pair& other) const { return key < other.key; }
    bool operator>(const Pair& other) const { return key > other.key; }
    bool operator==(const Pair& other) const { return key == other.key; }
};

template<typename K, typename V>
class HashTable {
private:
    static const int TABLE_SIZE = 100;
    AVL<Pair<K, V>> buckets[TABLE_SIZE];
    int elementCount;

public:
    // Initializes a hash table
    HashTable() : elementCount(0) {}

    // Delete copy operations
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Destructor to free heap memory
    ~HashTable() {
        // buckets' destructors are automatically called
    }

    // Inserts a key-value pair into the hash table
    void insert(const Pair<K, V>& pair) {
        int index = getBucketIndex(pair.key);
        Pair<K, V>* existing = buckets[index].find(Pair<K, V>(pair.key));
        if (existing) {
            existing->value = pair.value;  // Update value if key exists
        } else {
            buckets[index].insert(pair);
            elementCount++;
        }
    }

    // Removes a key from the hash table
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

    // Finds and returns a pointer to the value associated with the key, or nullptr if not found
    V* find(const K& key) {
        int index = getBucketIndex(key);
        Pair<K, V>* found = buckets[index].find(Pair<K, V>(key));
        if (found) {
            return &(found->value);
        }
        return nullptr;
    }

    // Returns true if the key exists in the hash table
    bool contains(const K& key) {
        int index = getBucketIndex(key);
        return buckets[index].search(Pair<K, V>(key));
    }

    // Returns the number of elements in the hash table
    int size() const { return elementCount; }

    // Removes all elements from the hash table
    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            buckets[i].clearTree();
        }
        elementCount = 0;
    }

    // Returns all elements by traversing each bucket
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
    // Helper to compute valid bucket index
    int getBucketIndex(const K& key) const {
        int h = static_cast<int>(std::hash<K>{}(key)) % TABLE_SIZE;
        if (h < 0)
            h += TABLE_SIZE;
        return h;
    }
};

#endif  // HASHTABLE_HPP
