#include "../PriorityQueue.hpp"
#include "test_utils.hpp"

template<typename T>
const int Vector<T>::DEFAULT_CAPACITY;
template<typename T>
const int Vector<T>::EXPAND_COEFFICIENT;

void test_priority_queue_empty() {
    auto pq = PriorityQueue<int>();
    EXPECT_TRUE(pq.empty());
    EXPECT_EQ(pq.size(), 0);
}

void test_priority_queue_max_heap_default() {
    auto pq = PriorityQueue<int>();
    pq.insert(10);
    pq.insert(30);
    pq.insert(20);

    EXPECT_EQ(pq.size(), 3);
    EXPECT_EQ(pq.peek(), 30);

    pq.extract();
    EXPECT_EQ(pq.peek(), 20);

    pq.extract();
    EXPECT_EQ(pq.peek(), 10);

    pq.extract();
    EXPECT_TRUE(pq.empty());
}

void test_priority_queue_min_heap_custom_comparator() {
    auto pq = PriorityQueue<int, std::greater<int>>();
    pq.insert(10);
    pq.insert(30);
    pq.insert(20);

    EXPECT_EQ(pq.peek(), 10);
    pq.extract();
    EXPECT_EQ(pq.peek(), 20);
}

void test_priority_queue_peek_empty_throws() {
    auto pq = PriorityQueue<int>();
    EXPECT_THROW(pq.peek());
}

int main() {
    RUN_TEST(test_priority_queue_empty);
    RUN_TEST(test_priority_queue_max_heap_default);
    RUN_TEST(test_priority_queue_min_heap_custom_comparator);
    RUN_TEST(test_priority_queue_peek_empty_throws);
    std::cout << "All PriorityQueue tests passed.\n";
}
