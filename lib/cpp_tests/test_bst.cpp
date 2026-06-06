#include "../BST.hpp"
#include "test_utils.hpp"

template<typename T>
const int Vector<T>::DEFAULT_CAPACITY;
template<typename T>
const int Vector<T>::EXPAND_COEFFICIENT;

void expect_vector_equals(const Vector<int>& v, std::initializer_list<int> expected) {
    EXPECT_EQ(v.size, static_cast<int>(expected.size()));
    auto i = 0;
    for (auto x : expected) {
        EXPECT_EQ(v[i], x);
        i++;
    }
}

void test_bst_insert_search_traversals() {
    auto tree = BST<int>();
    for (auto x : {5, 3, 7, 2, 4, 6, 8}) {
        tree.insert(x);
    }

    EXPECT_TRUE(tree.search(5));
    EXPECT_TRUE(tree.search(2));
    EXPECT_TRUE(tree.search(8));
    EXPECT_FALSE(tree.search(99));

    expect_vector_equals(tree.lnr(), {2, 3, 4, 5, 6, 7, 8});
    expect_vector_equals(tree.nlr(), {5, 3, 2, 4, 7, 6, 8});
    expect_vector_equals(tree.lrn(), {2, 4, 3, 6, 8, 7, 5});
}

void test_bst_remove_leaf_one_child_two_children() {
    auto tree = BST<int>();
    for (auto x : {5, 3, 7, 2, 4, 6, 8}) {
        tree.insert(x);
    }

    tree.remove(2);  // leaf
    EXPECT_FALSE(tree.search(2));
    expect_vector_equals(tree.lnr(), {3, 4, 5, 6, 7, 8});

    tree.remove(7);  // two children
    EXPECT_FALSE(tree.search(7));
    expect_vector_equals(tree.lnr(), {3, 4, 5, 6, 8});

    tree.remove(3);  // one child after previous removals
    EXPECT_FALSE(tree.search(3));
    expect_vector_equals(tree.lnr(), {4, 5, 6, 8});
}

void test_bst_copy_and_assignment() {
    auto a = BST<int>();
    for (auto x : {5, 3, 7}) {
        a.insert(x);
    }

    auto b = BST<int>(a);
    b.insert(9);
    EXPECT_FALSE(a.search(9));
    EXPECT_TRUE(b.search(9));

    auto c = BST<int>();
    c = a;
    c.remove(3);
    EXPECT_TRUE(a.search(3));
    EXPECT_FALSE(c.search(3));
}

int main() {
    RUN_TEST(test_bst_insert_search_traversals);
    RUN_TEST(test_bst_remove_leaf_one_child_two_children);
    RUN_TEST(test_bst_copy_and_assignment);
    std::cout << "All BST tests passed.\n";
}
