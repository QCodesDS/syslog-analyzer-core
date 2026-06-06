#include "../Stack.hpp"
#include "test_utils.hpp"

void test_stack_empty() {
    auto st = Stack<int>();
    EXPECT_TRUE(st.empty());
    EXPECT_EQ(st.size(), 0);
}

void test_stack_push_top_pop_lifo() {
    auto st = Stack<int>();
    st.push(1);
    st.push(2);
    st.push(3);

    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.top(), 3);

    EXPECT_TRUE(st.pop());
    EXPECT_EQ(st.top(), 2);

    EXPECT_TRUE(st.pop());
    EXPECT_EQ(st.top(), 1);

    EXPECT_TRUE(st.pop());
    EXPECT_TRUE(st.empty());
    EXPECT_FALSE(st.pop());
}

void test_stack_top_empty_throws() {
    auto st = Stack<int>();
    EXPECT_THROW(st.top());
}

int main() {
    RUN_TEST(test_stack_empty);
    RUN_TEST(test_stack_push_top_pop_lifo);
    RUN_TEST(test_stack_top_empty_throws);
    std::cout << "All Stack tests passed.\n";
}
