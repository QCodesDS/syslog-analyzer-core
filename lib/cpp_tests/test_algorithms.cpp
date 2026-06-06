#include "../Algorithms.hpp"
#include "test_utils.hpp"

template<typename T>
const int Vector<T>::DEFAULT_CAPACITY;
template<typename T>
const int Vector<T>::EXPAND_COEFFICIENT;

Vector<int> make_vector(std::initializer_list<int> values) {
    auto v = Vector<int>();
    for (auto x : values) {
        v.pushBack(x);
    }
    return v;
}

void expect_vector_equals(const Vector<int>& v, std::initializer_list<int> expected) {
    EXPECT_EQ(v.size, static_cast<int>(expected.size()));
    auto i = 0;
    for (auto x : expected) {
        EXPECT_EQ(v[i], x);
        i++;
    }
}

void test_linear_search() {
    auto v = make_vector({10, 20, 30});
    EXPECT_EQ(linearSearch(v, 10), 0);
    EXPECT_EQ(linearSearch(v, 30), 2);
    EXPECT_EQ(linearSearch(v, 99), -1);
}

void test_binary_search() {
    auto v = make_vector({10, 20, 30, 40});
    EXPECT_EQ(binarySearch(v, 10), 0);
    EXPECT_EQ(binarySearch(v, 30), 2);
    EXPECT_EQ(binarySearch(v, 99), -1);
}

void test_selection_sort_ascending_and_descending() {
    auto v = make_vector({4, 1, 3, 2});
    selectionSort(v);
    expect_vector_equals(v, {1, 2, 3, 4});

    selectionSort(v, std::greater<int>());
    expect_vector_equals(v, {4, 3, 2, 1});
}

void test_bubble_sort_ascending_and_descending() {
    auto v = make_vector({4, 1, 3, 2});
    bubbleSort(v);
    expect_vector_equals(v, {1, 2, 3, 4});

    bubbleSort(v, std::greater<int>());
    expect_vector_equals(v, {4, 3, 2, 1});
}

void test_insertion_sort_ascending_and_descending() {
    auto v = make_vector({4, 1, 3, 2});
    insertionSort(v);
    expect_vector_equals(v, {1, 2, 3, 4});

    insertionSort(v, std::greater<int>());
    expect_vector_equals(v, {4, 3, 2, 1});
}

void test_merge_function() {
    auto v = make_vector({1, 3, 5, 2, 4, 6});
    merge(v, 0, 2, 5);
    expect_vector_equals(v, {1, 2, 3, 4, 5, 6});
}

void test_merge_sort() {
    auto v = make_vector({5, 1, 4, 2, 3});
    mergeSort(v, 0, v.size - 1);
    expect_vector_equals(v, {1, 2, 3, 4, 5});
}

void test_quick_sort() {
    auto v = make_vector({5, 1, 4, 2, 3});
    quickSort(v, 0, v.size - 1);
    expect_vector_equals(v, {1, 2, 3, 4, 5});
}

int main() {
    RUN_TEST(test_linear_search);
    RUN_TEST(test_binary_search);
    RUN_TEST(test_selection_sort_ascending_and_descending);
    RUN_TEST(test_bubble_sort_ascending_and_descending);
    RUN_TEST(test_insertion_sort_ascending_and_descending);
    RUN_TEST(test_merge_function);
    RUN_TEST(test_merge_sort);
    RUN_TEST(test_quick_sort);
    std::cout << "Algorithm tests completed.\n";
}
