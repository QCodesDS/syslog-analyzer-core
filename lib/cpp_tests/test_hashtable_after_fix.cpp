#include "../HashTable.hpp"
#include "test_utils.hpp"

template<typename T>
const int Vector<T>::DEFAULT_CAPACITY;
template<typename T>
const int Vector<T>::EXPAND_COEFFICIENT;

// This file compiles only after you fix HashTable.hpp constructor/clear:
//   HashTable(Hasher<K>* hasher) { this->hasher = hasher; ... }
// and either add AVL::clear() with no argument, or remove HashTable::clear for now.

struct IntHasher : Hasher<int> {
    int hash(const int& key) override { return key; }
};

void test_pair_comparison_by_key() {
    auto a = Pair<int, std::string>(1, "one");
    auto b = Pair<int, std::string>(2, "two");
    auto same_key = Pair<int, std::string>(1, "ONE");

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a == same_key);
}

void test_hashtable_insert_contains_find_size() {
    auto hasher = IntHasher();
    auto table = HashTable<int, std::string>(&hasher);

    table.insert(Pair<int, std::string>(1, "one"));
    table.insert(Pair<int, std::string>(2, "two"));

    EXPECT_EQ(table.size(), 2);
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(2));
    EXPECT_FALSE(table.contains(99));

    auto found = table.find(2);
    EXPECT_TRUE(found != nullptr);
    EXPECT_EQ(*found, "two");
}

void test_hashtable_update_existing_key_does_not_increase_size() {
    auto hasher = IntHasher();
    auto table = HashTable<int, std::string>(&hasher);

    table.insert(Pair<int, std::string>(1, "one"));
    table.insert(Pair<int, std::string>(1, "ONE"));

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(*table.find(1), "ONE");
}

void test_hashtable_remove() {
    auto hasher = IntHasher();
    auto table = HashTable<int, std::string>(&hasher);

    table.insert(Pair<int, std::string>(1, "one"));
    table.insert(Pair<int, std::string>(2, "two"));

    EXPECT_TRUE(table.remove(1));
    EXPECT_FALSE(table.contains(1));
    EXPECT_EQ(table.size(), 1);
    EXPECT_FALSE(table.remove(99));
}

int main() {
    RUN_TEST(test_pair_comparison_by_key);
    RUN_TEST(test_hashtable_insert_contains_find_size);
    RUN_TEST(test_hashtable_update_existing_key_does_not_increase_size);
    RUN_TEST(test_hashtable_remove);
    std::cout << "All HashTable tests passed.\n";
}
