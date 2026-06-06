#include "../LinkedList.hpp"
#include "test_utils.hpp"

void test_linkedlist_empty() {
    auto list = LinkedList<int>();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

void test_linkedlist_insert_front() {
    auto list = LinkedList<int>();
    list.insertFront(2);
    list.insertFront(1);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.tail->value, 2);
}

void test_linkedlist_insert_back() {
    auto list = LinkedList<int>();
    list.insertBack(1);
    list.insertBack(2);
    list.insertBack(3);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.tail->value, 3);
}

void test_linkedlist_insert_at() {
    auto list = LinkedList<int>();
    list.insertBack(1);
    list.insertBack(3);
    list.insertAt(1, 2);
    list.insertAt(3, 4);

    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.head->next->value, 2);
    EXPECT_EQ(list.head->next->next->value, 3);
    EXPECT_EQ(list.tail->value, 4);
}

void test_linkedlist_insert_at_invalid() {
    auto list = LinkedList<int>();
    EXPECT_THROW(list.insertAt(-1, 10));
    EXPECT_THROW(list.insertAt(1, 10));
}

void test_linkedlist_find() {
    auto list = LinkedList<int>();
    list.insertBack(10);
    list.insertBack(20);

    EXPECT_TRUE(list.find(10));
    EXPECT_TRUE(list.find(20));
    EXPECT_FALSE(list.find(99));
}

void test_linkedlist_remove_value() {
    auto list = LinkedList<int>();
    list.insertBack(1);
    list.insertBack(2);
    list.insertBack(3);

    EXPECT_TRUE(list.remove(2));
    EXPECT_EQ(list.size(), 2);
    EXPECT_FALSE(list.find(2));
    EXPECT_FALSE(list.remove(99));
}

void test_linkedlist_remove_at() {
    auto list = LinkedList<int>();
    list.insertBack(1);
    list.insertBack(2);
    list.insertBack(3);

    EXPECT_TRUE(list.removeAt(0));
    EXPECT_EQ(list.head->value, 2);
    EXPECT_TRUE(list.removeAt(1));
    EXPECT_EQ(list.tail->value, 2);
    EXPECT_FALSE(list.removeAt(10));
    EXPECT_FALSE(list.removeAt(-1));
}

void test_linkedlist_clear() {
    auto list = LinkedList<int>();
    list.insertBack(1);
    list.insertBack(2);
    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.head == nullptr);
    EXPECT_TRUE(list.tail == nullptr);
}

void test_linkedlist_copy_and_assignment() {
    auto a = LinkedList<int>();
    a.insertBack(1);
    a.insertBack(2);

    auto b = LinkedList<int>(a);
    b.head->value = 99;
    EXPECT_EQ(a.head->value, 1);
    EXPECT_EQ(b.head->value, 99);

    auto c = LinkedList<int>();
    c = a;
    c.tail->value = 88;
    EXPECT_EQ(a.tail->value, 2);
    EXPECT_EQ(c.tail->value, 88);
}

int main() {
    RUN_TEST(test_linkedlist_empty);
    RUN_TEST(test_linkedlist_insert_front);
    RUN_TEST(test_linkedlist_insert_back);
    RUN_TEST(test_linkedlist_insert_at);
    RUN_TEST(test_linkedlist_insert_at_invalid);
    RUN_TEST(test_linkedlist_find);
    RUN_TEST(test_linkedlist_remove_value);
    RUN_TEST(test_linkedlist_remove_at);
    RUN_TEST(test_linkedlist_clear);
    RUN_TEST(test_linkedlist_copy_and_assignment);
    std::cout << "All LinkedList tests passed.\n";
}
