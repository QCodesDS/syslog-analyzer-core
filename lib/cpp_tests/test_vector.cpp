#include "../Vector.hpp"
#include "test_utils.hpp"

template<typename T>
const int Vector<T>::DEFAULT_CAPACITY;
template<typename T>
const int Vector<T>::EXPAND_COEFFICIENT;

void test_vector_default_constructor() {
    auto v = Vector<int>();
    EXPECT_EQ(v.size, 0);
    EXPECT_TRUE(v.capacity >= Vector<int>::DEFAULT_CAPACITY);
}

void test_vector_push_back_and_index() {
    auto v = Vector<int>();
    v.pushBack(10);
    v.pushBack(20);

    EXPECT_EQ(v.size, 2);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
}

void test_vector_fill_constructor() {
    auto v = Vector<int>(5, 7);

    EXPECT_EQ(v.size, 5);
    for (auto i = 0; i < v.size; i++) {
        EXPECT_EQ(v[i], 7);
    }
}

void test_vector_pop_back() {
    auto v = Vector<int>();
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();

    EXPECT_EQ(v.size, 1);
    EXPECT_EQ(v[0], 1);

    v.popBack();
    v.popBack();
    EXPECT_EQ(v.size, 0);
}

void test_vector_out_of_bounds() {
    auto v = Vector<int>();
    EXPECT_THROW(v[0]);
    v.pushBack(1);
    EXPECT_THROW(v[-1]);
    EXPECT_THROW(v[1]);
}

void test_vector_copy_constructor_deep_copy() {
    auto a = Vector<int>();
    a.pushBack(1);
    a.pushBack(2);

    auto b = Vector<int>(a);
    b[0] = 99;

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b[0], 99);
    EXPECT_EQ(b[1], 2);
}

void test_vector_assignment_deep_copy() {
    auto a = Vector<int>();
    a.pushBack(1);
    a.pushBack(2);

    auto b = Vector<int>();
    b = a;
    b[1] = 88;

    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(b[1], 88);
}

int main() {
    RUN_TEST(test_vector_default_constructor);
    RUN_TEST(test_vector_push_back_and_index);
    RUN_TEST(test_vector_fill_constructor);
    RUN_TEST(test_vector_pop_back);
    RUN_TEST(test_vector_out_of_bounds);
    RUN_TEST(test_vector_copy_constructor_deep_copy);
    RUN_TEST(test_vector_assignment_deep_copy);
    std::cout << "All Vector tests passed.\n";
}
