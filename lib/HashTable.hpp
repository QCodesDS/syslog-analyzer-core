#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstdlib>

#include "AVL.hpp"
#include "Vector.hpp"

template<typename T>
struct Hasher {
    virtual int hash(const T& key) = 0;
};

template<typename K, typename V>
struct Pair {
    K key;
    V value;
    Pair(const K& key, const V& value) {
        this->key = key;
        this->value = value;
    }

    bool operator<(const Pair& other) const { return key < other.key; }

    bool operator>(const Pair& other) const { return key > other.key; }

    bool operator==(const Pair& other) const { return key == other.key; }
};

template<typename K, typename V>
struct HashTable {
    static const int TABLE_SIZE = 100;
    Hasher<K>* hasher;
    int count;
    Vector<AVL<Pair<K, V>>> table;
    HashTable(Hasher<K> hasher) {
        this->table = Vector<AVL<Pair<K, V>>>(TABLE_SIZE, AVL<Pair<K, V>>());
        this->hasher = hasher;
        this->count = 0;
    }

    void insert(const Pair<K, V>& value) {
        int idx = getIdx(value.key);
        if (!contains(value.key)) {
            count++;
        }
        this->table[idx].insert(value);
    }

    bool remove(const K& key) {
        int idx = getIdx(key);
        if (contains(key)) {
            this->count--;
            this->table[idx].remove(Pair<K, V>(key, V()));
            return true;
        }
        return false;
    }

    bool contains(const K& key) {
        int idx = getIdx(key);
        return this->table[idx].search(Pair<K, V>(key, V()));
    }

    V* find(const K& key) {
        int idx = getIdx(key);
        Pair<K, V>* found = this->table[idx].find(Pair<K, V>(key, V()));
        if (found) {
            return &found->value;
        }
        return nullptr;
    }

    int size() const { return this->count; }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            this->table[i].clear();
        }
        this->count = 0;
    }

private:
    int getIdx(const K& key) {
        int idx = std::abs(this->hasher->hash(key)) % TABLE_SIZE;
        return idx;
    }
};

#endif  // HASHTABLE_HPP
