#include "../Queue.hpp"
#include "test_utils.hpp"

// This file compiles only after you fix Queue.hpp:
//   template<typename T> struct Stack  ->  template<typename T> struct Queue
//   Stack() {}                         ->  Queue() {}
//   T& top()                           ->  T& front()

void test_queue_empty() {
    auto q = Queue<int>();
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);
}

void test_queue_enqueue_front_dequeue_fifo() {
    auto q = Queue<int>();
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(q.front(), 1);

    EXPECT_TRUE(q.dequeue());
    EXPECT_EQ(q.front(), 2);

    EXPECT_TRUE(q.dequeue());
    EXPECT_EQ(q.front(), 3);

    EXPECT_TRUE(q.dequeue());
    EXPECT_TRUE(q.empty());
    EXPECT_FALSE(q.dequeue());
}

void test_queue_front_empty_throws() {
    auto q = Queue<int>();
    EXPECT_THROW(q.front());
}

int main() {
    RUN_TEST(test_queue_empty);
    RUN_TEST(test_queue_enqueue_front_dequeue_fifo);
    RUN_TEST(test_queue_front_empty_throws);
    std::cout << "All Queue tests passed.\n";
}
