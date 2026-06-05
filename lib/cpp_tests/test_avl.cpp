#include "../AVL.hpp"
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

int check_height_and_balance(AVLNode<int>* root) {
    if (!root) {
        return -1;
    }
    auto left = check_height_and_balance(root->left);
    auto right = check_height_and_balance(root->right);
    EXPECT_TRUE(std::abs(right - left) <= 1);
    EXPECT_EQ(root->height, 1 + std::max(left, right));
    return root->height;
}

void test_avl_insert_search_inorder_balance() {
    auto tree = AVL<int>();
    for (auto x : {10, 20, 30, 40, 50, 25}) {
        tree.insert(x);
    }

    EXPECT_TRUE(tree.search(25));
    EXPECT_FALSE(tree.search(99));
    expect_vector_equals(tree.lnr(), {10, 20, 25, 30, 40, 50});
    check_height_and_balance(tree.root);
}

void test_avl_remove_and_balance() {
    auto tree = AVL<int>();
    for (auto x : {10, 20, 30, 40, 50, 25}) {
        tree.insert(x);
    }

    tree.remove(40);
    tree.remove(10);

    EXPECT_FALSE(tree.search(40));
    EXPECT_FALSE(tree.search(10));
    expect_vector_equals(tree.lnr(), {20, 25, 30, 50});
    check_height_and_balance(tree.root);
}

void test_avl_find() {
    auto tree = AVL<int>();
    tree.insert(1);
    tree.insert(2);

    auto found = tree.find(2);
    EXPECT_TRUE(found != nullptr);
    EXPECT_EQ(*found, 2);
    EXPECT_TRUE(tree.find(99) == nullptr);
}

void test_avl_copy_and_assignment() {
    auto a = AVL<int>();
    for (auto x : {5, 3, 7}) {
        a.insert(x);
    }

    auto b = AVL<int>(a);
    b.insert(9);
    EXPECT_FALSE(a.search(9));
    EXPECT_TRUE(b.search(9));

    auto c = AVL<int>();
    c = a;
    c.remove(3);
    EXPECT_TRUE(a.search(3));
    EXPECT_FALSE(c.search(3));
}

int main() {
    RUN_TEST(test_avl_insert_search_inorder_balance);
    RUN_TEST(test_avl_remove_and_balance);
    RUN_TEST(test_avl_find);
    RUN_TEST(test_avl_copy_and_assignment);
    std::cout << "All AVL tests passed.\n";
}
