/**
 * @file test_dsa.cpp
 * @brief Tests all core library data structures and algorithms (lib/) for correct behavior.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <string>

#include "../../lib/Algorithms.hpp"
#include "../../lib/AVL.hpp"
#include "../../lib/BST.hpp"
#include "../../lib/HashTable.hpp"
#include "../../lib/LinkedList.hpp"
#include "../../lib/PriorityQueue.hpp"
#include "../../lib/Queue.hpp"
#include "../../lib/Stack.hpp"
#include "../../lib/Trie.hpp"
#include "../doctest.h"

/**
 * @brief Kiểm tra xem mảng đã được sắp xếp tăng dần chưa.
 * @param arr Mảng Vector cần kiểm tra.
 * @return true Nếu mảng đã được sắp xếp tăng dần.
 * @return false Nếu chưa.
 */
static bool isSortedAsc(const Vector<int>& arr) {
    for (int i = 0; i < arr.getSize() - 1; ++i)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}

/**
 * @brief Kiểm tra xem mảng đã được sắp xếp giảm dần chưa.
 * @param arr Mảng Vector cần kiểm tra.
 * @return true Nếu mảng đã được sắp xếp giảm dần.
 * @return false Nếu chưa.
 */
static bool isSortedDesc(const Vector<int>& arr) {
    for (int i = 0; i < arr.getSize() - 1; ++i)
        if (arr[i] < arr[i + 1])
            return false;
    return true;
}

/**
 * @brief Test Suite for AVL Tree
 */
TEST_SUITE("AVL Tree") {
    TEST_CASE("1. Insert 10 elements in sorted order - must not degrade to O(N)") {
        AVL<int> tree;
        for (int i = 1; i <= 10; i++) {
            tree.insert(i);
        }
        CHECK(tree.height() <= 4);
    }

    TEST_CASE("2. Insert duplicates - size must not increase") {
        AVL<int> tree;
        tree.insert(5);
        tree.insert(5);
        tree.insert(5);
        CHECK(tree.lnr().getSize() == 1);
    }

    TEST_CASE("3. Remove leaf node - search returns false") {
        AVL<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.remove(5);
        CHECK(tree.search(5) == false);
        CHECK(tree.search(10) == true);
        CHECK(tree.search(15) == true);
    }

    TEST_CASE("4. Remove node with two children - tree still searchable") {
        AVL<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.insert(3);
        tree.insert(7);
        tree.remove(5);
        CHECK(tree.search(5) == false);
        CHECK(tree.search(10) == true);
        CHECK(tree.search(15) == true);
        CHECK(tree.search(3) == true);
        CHECK(tree.search(7) == true);
    }

    TEST_CASE("5. Search non-existent - returns false") {
        AVL<int> tree;
        tree.insert(10);
        CHECK(tree.search(99) == false);
    }

    TEST_CASE("6. Insert then remove all - tree is empty (root == nullptr)") {
        AVL<int> tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.remove(10);
        tree.remove(20);
        tree.remove(30);
        CHECK(tree.getRoot() == nullptr);
    }
}

/**
 * @brief Test Suite for BST (Binary Search Tree)
 */
TEST_SUITE("BST") {
    TEST_CASE("1. Insert and search") {
        BST<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        CHECK(tree.search(10) == true);
        CHECK(tree.search(5) == true);
        CHECK(tree.search(15) == true);
        CHECK(tree.search(99) == false);
    }

    TEST_CASE("2. Inorder traversal (LNR) returns sorted elements") {
        BST<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        Vector<int> sorted = tree.lnr();
        REQUIRE(sorted.getSize() == 3);
        CHECK(sorted[0] == 5);
        CHECK(sorted[1] == 10);
        CHECK(sorted[2] == 15);
    }

    TEST_CASE("3. Remove leaf") {
        BST<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.remove(5);
        CHECK(tree.search(5) == false);
    }
}

/**
 * @brief Test Suite for HashTable
 */
TEST_SUITE("HashTable") {
    TEST_CASE("1. Insert 5 pairs - find() returns correct values") {
        HashTable<std::string, int> ht;
        ht.insert(Pair<std::string, int>("one", 1));
        ht.insert(Pair<std::string, int>("two", 2));
        ht.insert(Pair<std::string, int>("three", 3));
        ht.insert(Pair<std::string, int>("four", 4));
        ht.insert(Pair<std::string, int>("five", 5));

        REQUIRE(ht.find("three") != nullptr);
        CHECK(*(ht.find("three")) == 3);
        REQUIRE(ht.find("five") != nullptr);
        CHECK(*(ht.find("five")) == 5);
    }

    TEST_CASE("2. Insert duplicate key - value updated, size unchanged") {
        HashTable<std::string, int> ht;
        ht.insert(Pair<std::string, int>("key1", 10));
        ht.insert(Pair<std::string, int>("key1", 20));

        CHECK(ht.size() == 1);
        REQUIRE(ht.find("key1") != nullptr);
        CHECK(*(ht.find("key1")) == 20);
    }

    TEST_CASE("3. Remove existing key - contains() false after removal") {
        HashTable<std::string, int> ht;
        ht.insert(Pair<std::string, int>("key1", 10));
        ht.remove("key1");
        CHECK(ht.contains("key1") == false);
        CHECK(ht.size() == 0);
    }

    TEST_CASE("4. Remove non-existent - returns false, size unchanged") {
        HashTable<std::string, int> ht;
        ht.insert(Pair<std::string, int>("key1", 10));
        bool result = ht.remove("key2");
        CHECK(result == false);
        CHECK(ht.size() == 1);
    }

    TEST_CASE("5. find() on empty table - returns nullptr") {
        HashTable<std::string, int> ht;
        CHECK(ht.find("key1") == nullptr);
    }

    TEST_CASE("6. Force collision & rehash: insert 200 string keys - all retrievable") {
        HashTable<std::string, int> ht;
        for (int i = 0; i < 200; i++) {
            ht.insert(Pair<std::string, int>("key_" + std::to_string(i), i));
        }
        CHECK(ht.size() == 200);
        for (int i = 0; i < 200; i++) {
            std::string k = "key_" + std::to_string(i);
            REQUIRE(ht.find(k) != nullptr);
            CHECK(*(ht.find(k)) == i);
        }
    }

    TEST_CASE("7. Clear - size() == 0, find() returns nullptr for all keys") {
        HashTable<std::string, int> ht;
        ht.insert(Pair<std::string, int>("key1", 10));
        ht.insert(Pair<std::string, int>("key2", 20));
        ht.clear();
        CHECK(ht.size() == 0);
        CHECK(ht.find("key1") == nullptr);
        CHECK(ht.find("key2") == nullptr);
    }
}

/**
 * @brief Test Suite for PriorityQueue
 */
TEST_SUITE("PriorityQueue") {
    TEST_CASE("1. Insert out of order - peek() returns max") {
        PriorityQueue<int> pq;
        pq.insert(10);
        pq.insert(50);
        pq.insert(30);
        pq.insert(20);
        CHECK(pq.peek() == 50);
    }

    TEST_CASE("2. Extract max - next peek() is second largest") {
        PriorityQueue<int> pq;
        pq.insert(10);
        pq.insert(50);
        pq.insert(30);
        pq.extract();
        CHECK(pq.peek() == 30);
    }

    TEST_CASE("3. Extract all - comes out in descending order") {
        PriorityQueue<int> pq;
        pq.insert(10);
        pq.insert(50);
        pq.insert(30);
        pq.insert(20);

        int prev = pq.peek();
        pq.extract();
        while (!pq.empty()) {
            int current = pq.peek();
            CHECK(current <= prev);
            prev = current;
            pq.extract();
        }
    }

    TEST_CASE("4. peek() on empty - throws exception") {
        PriorityQueue<int> pq;
        CHECK_THROWS_AS(pq.peek(), std::out_of_range);
    }

    TEST_CASE("5. Min-heap via custom comparator - smallest extracted first") {
        PriorityQueue<int, std::greater<int>> pq;
        pq.insert(50);
        pq.insert(10);
        pq.insert(30);
        CHECK(pq.peek() == 10);
        pq.extract();
        CHECK(pq.peek() == 30);
    }
}

/**
 * @brief Test Suite for LinkedList
 */
TEST_SUITE("LinkedList") {
    TEST_CASE("1. insertFront, insertBack, insertAt - size correct") {
        LinkedList<int> ll;
        ll.insertFront(10);
        ll.insertBack(20);
        ll.insertAt(1, 15);
        CHECK(ll.size() == 3);
        CHECK(ll.getHead()->value == 10);
        CHECK(ll.getTail()->value == 20);
    }

    TEST_CASE("2. remove(value) - first occurrence only") {
        LinkedList<int> ll;
        ll.insertBack(10);
        ll.insertBack(20);
        ll.insertBack(10);
        ll.remove(10);
        CHECK(ll.size() == 2);
        CHECK(ll.getHead()->value == 20);
    }

    TEST_CASE("3. removeAt(index) - correct element removed") {
        LinkedList<int> ll;
        ll.insertBack(10);
        ll.insertBack(20);
        ll.insertBack(30);
        ll.removeAt(1);  // removes 20
        CHECK(ll.size() == 2);
        CHECK(ll.getHead()->next->value == 30);
    }

    TEST_CASE("4. Copy constructor & Assignment - deep copy, no double free") {
        LinkedList<int> ll;
        ll.insertBack(10);
        ll.insertBack(20);

        LinkedList<int> ll_copy(ll);
        ll_copy.insertBack(30);

        CHECK(ll.size() == 2);
        CHECK(ll_copy.size() == 3);

        LinkedList<int> assigned;
        assigned = ll;
        assigned.insertBack(40);
        CHECK(ll.size() == 2);
        CHECK(assigned.size() == 3);
    }
}

/**
 * @brief Test Suite for Stack and Queue
 */
TEST_SUITE("Stack and Queue") {
    TEST_CASE("Stack: push/pop/top/empty/size") {
        Stack<int> s;
        CHECK(s.empty() == true);
        s.push(10);
        s.push(20);
        CHECK(s.size() == 2);
        CHECK(s.top() == 20);
        s.pop();
        CHECK(s.top() == 10);
        s.pop();
        CHECK(s.empty() == true);
    }

    TEST_CASE("Queue: enqueue/dequeue/front/empty/size") {
        Queue<int> q;
        CHECK(q.empty() == true);
        q.enqueue(10);
        q.enqueue(20);
        CHECK(q.size() == 2);
        CHECK(q.front() == 10);
        q.dequeue();
        CHECK(q.front() == 20);
        q.dequeue();
        CHECK(q.empty() == true);
    }
}

/**
 * @brief Test Suite for Trie
 */
TEST_SUITE("Trie") {
    TEST_CASE("1. insert \"ERROR\", \"FATAL\", \"TIMEOUT\" - search() true for all") {
        Trie t;
        t.insert("ERROR");
        t.insert("FATAL");
        t.insert("TIMEOUT");
        CHECK(t.search("ERROR") == true);
        CHECK(t.search("FATAL") == true);
        CHECK(t.search("TIMEOUT") == true);
    }

    TEST_CASE("2. startsWith(\"ERR\") - true (prefix of \"ERROR\")") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.startsWith("ERR") == true);
    }

    TEST_CASE("3. clear() - search returns false for all previously inserted") {
        Trie t;
        t.insert("ERROR");
        t.clear();
        CHECK(t.search("ERROR") == false);
    }

    TEST_CASE("4. Case sensitivity: \"error\" != \"ERROR\"") {
        Trie t;
        t.insert("ERROR");
        CHECK(t.search("error") == false);
    }
}

/**
 * @brief Test Suite for Sorting Algorithms
 */
TEST_SUITE("Sorting Algorithms") {
    TEST_CASE("Test 1: random order - sorted ascending") {
        Vector<int> arr;
        arr.pushBack(5);
        arr.pushBack(2);
        arr.pushBack(9);
        arr.pushBack(1);
        Vector<int> a = arr;
        bubbleSort(a);
        CHECK(isSortedAsc(a));
        a = arr;
        selectionSort(a);
        CHECK(isSortedAsc(a));
        a = arr;
        insertionSort(a);
        CHECK(isSortedAsc(a));
        a = arr;
        heapSort(a);
        CHECK(isSortedAsc(a));
        a = arr;
        quickSort(a, 0, a.getSize() - 1);
        CHECK(isSortedAsc(a));
        a = arr;
        mergeSort(a, 0, a.getSize() - 1);
        CHECK(isSortedAsc(a));
    }

    TEST_CASE("Test 2: with custom comparator - sorted descending") {
        Vector<int> arr;
        arr.pushBack(5);
        arr.pushBack(2);
        arr.pushBack(9);
        arr.pushBack(1);
        std::greater<int> cmp;
        Vector<int> a = arr;
        bubbleSort(a, cmp);
        CHECK(isSortedDesc(a));
        a = arr;
        quickSort(a, 0, a.getSize() - 1, cmp);
        CHECK(isSortedDesc(a));
    }
}

/**
 * @brief Test Suite for Search Algorithms
 */
TEST_SUITE("Search Algorithms") {
    TEST_CASE("linearSearch & binarySearch") {
        Vector<int> arr;
        arr.pushBack(10);
        arr.pushBack(20);
        arr.pushBack(30);
        CHECK(linearSearch(arr, 20) == 1);
        CHECK(linearSearch(arr, 99) == -1);
        CHECK(binarySearch(arr, 30) == 2);
        CHECK(binarySearch(arr, 99) == -1);
    }
}
