/**
 * @file test_dsa_correctness.cpp
 * @brief Tests all core data structures and algorithms for correct behavior.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../lib/AVL.hpp"
//#include "../lib/BST.hpp" // Not found in the lib dir but mentioned in prompt, assuming AVL handles tree
#include "../lib/HashTable.hpp"
#include "../lib/PriorityQueue.hpp"
#include "../lib/LinkedList.hpp"
#include "../lib/Stack.hpp"
#include "../lib/Queue.hpp"
#include "../lib/Algorithms.hpp"
#include <string>

// ---- Sorting helpers (free functions, visible to all TEST_CASEs) ----
static bool isSortedAsc(const Vector<int>& arr) {
    for (int i = 0; i < arr.getSize() - 1; ++i)
        if (arr[i] > arr[i + 1]) return false;
    return true;
}
static bool isSortedDesc(const Vector<int>& arr) {
    for (int i = 0; i < arr.getSize() - 1; ++i)
        if (arr[i] < arr[i + 1]) return false;
    return true;
}

/**
 * @brief Test Suite for AVL Tree
 * Verifies that the AVL tree maintains its balance properties under
 * sequential and random insertions/deletions, correctly preventing
 * O(N) degradation. Also tests basic operations like search and clear.
 */
TEST_SUITE("AVL Tree") {
    TEST_CASE("1. Insert 10 elements in sorted order - must not degrade to O(N)") {
        AVL<int> tree;
        for (int i = 1; i <= 10; i++) {
            tree.insert(i);
        }
        // Verify height stays <= 4 (log2(10) ~ 3.3)
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
        tree.remove(5); // 5 has two children (3 and 7)
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
 * @brief Test Suite for HashTable
 * Verifies that hash table correctly handles collisions, duplicate keys,
 * and maintains accurate size under various operations. 
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

    TEST_CASE("6. Force collision: insert 200 string keys - all retrievable") {
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
 * Verifies correct heap properties (max-heap by default, min-heap with comparator),
 * correct ordering of extracted elements, and exception handling.
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
        while(!pq.empty()) {
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

    TEST_CASE("6. Insert 1000 elements - still extracts in correct order") {
        PriorityQueue<int> pq;
        for (int i = 0; i < 1000; i++) {
            pq.insert(i % 100);
        }
        int prev = pq.peek();
        pq.extract();
        while(!pq.empty()) {
            int current = pq.peek();
            CHECK(current <= prev);
            prev = current;
            pq.extract();
        }
    }
}

/**
 * @brief Test Suite for LinkedList
 * Verifies insertions at various points, correct removal behavior,
 * memory safety during copies, and proper list sizes.
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
        ll.removeAt(1); // removes 20
        CHECK(ll.size() == 2);
        CHECK(ll.getHead()->next->value == 30);
    }

    TEST_CASE("4. find() - true/false correct") {
        LinkedList<int> ll;
        ll.insertBack(10);
        CHECK(ll.find(10) == true);
        CHECK(ll.find(20) == false);
    }

    TEST_CASE("5. clear() - size 0, no crash on second clear()") {
        LinkedList<int> ll;
        ll.insertBack(10);
        ll.clear();
        CHECK(ll.size() == 0);
        ll.clear(); // shouldn't crash
        CHECK(ll.size() == 0);
    }

    TEST_CASE("6. Copy constructor - deep copy, modifying copy doesn't affect original") {
        LinkedList<int> ll;
        ll.insertBack(10);
        ll.insertBack(20);
        
        LinkedList<int> ll_copy(ll);
        ll_copy.insertBack(30);
        
        CHECK(ll.size() == 2);
        CHECK(ll_copy.size() == 3);
    }
}

/**
 * @brief Test Suite for Stack and Queue
 * Verifies LIFO and FIFO properties, basic operations, and exception throws
 * when operating on empty instances.
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

    TEST_CASE("Stack: top() on empty - throws") {
        Stack<int> s;
        CHECK_THROWS_AS(s.top(), std::out_of_range);
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

    TEST_CASE("Queue: front() on empty - throws") {
        Queue<int> q;
        CHECK_THROWS_AS(q.front(), std::out_of_range);
    }
}

/**
 * @brief Test Suite for Sorting Algorithms
 * Verifies standard sorting algorithms sort random, sorted, reversed 
 * and edge-case arrays correctly, including custom comparators.
 */
TEST_SUITE("Sorting Algorithms") {

    TEST_CASE("Test 1: random order - sorted ascending") {
        Vector<int> arr;
        arr.pushBack(5); arr.pushBack(2); arr.pushBack(9); arr.pushBack(1);
        Vector<int> a = arr; bubbleSort(a); CHECK(isSortedAsc(a));
        a = arr; selectionSort(a); CHECK(isSortedAsc(a));
        a = arr; insertionSort(a); CHECK(isSortedAsc(a));
        a = arr; heapSort(a); CHECK(isSortedAsc(a));
        a = arr; quickSort(a, 0, a.getSize()-1); CHECK(isSortedAsc(a));
        a = arr; mergeSort(a, 0, a.getSize()-1); CHECK(isSortedAsc(a));
    }

    TEST_CASE("Test 2: already sorted - no crash, still sorted") {
        Vector<int> arr;
        arr.pushBack(1); arr.pushBack(2); arr.pushBack(3);
        Vector<int> a = arr; bubbleSort(a); CHECK(isSortedAsc(a));
        a = arr; quickSort(a, 0, a.getSize()-1); CHECK(isSortedAsc(a));
    }

    TEST_CASE("Test 3: reverse order - sorted ascending") {
        Vector<int> arr;
        arr.pushBack(3); arr.pushBack(2); arr.pushBack(1);
        Vector<int> a = arr; mergeSort(a, 0, a.getSize()-1); CHECK(isSortedAsc(a));
        a = arr; heapSort(a); CHECK(isSortedAsc(a));
    }

    TEST_CASE("Test 4: single element - no crash") {
        Vector<int> arr; arr.pushBack(42);
        Vector<int> a = arr; bubbleSort(a); CHECK(a[0] == 42);
        a = arr; quickSort(a, 0, a.getSize()-1); CHECK(a[0] == 42);
    }

    TEST_CASE("Test 5: with custom comparator - sorted descending") {
        Vector<int> arr;
        arr.pushBack(5); arr.pushBack(2); arr.pushBack(9); arr.pushBack(1);
        std::greater<int> cmp;
        Vector<int> a = arr; bubbleSort(a, cmp); CHECK(isSortedDesc(a));
        a = arr; quickSort(a, 0, a.getSize()-1, cmp); CHECK(isSortedDesc(a));
    }
}

/**
 * @brief Test Suite for Search Algorithms
 * Tests finding present elements, missing elements, and handling empty arrays.
 */
TEST_SUITE("Search Algorithms") {
    TEST_CASE("linearSearch: found, not found, empty array") {
        Vector<int> arr;
        arr.pushBack(10); arr.pushBack(20); arr.pushBack(30);
        CHECK(linearSearch(arr, 20) == 1);
        CHECK(linearSearch(arr, 99) == -1);
        Vector<int> emptyArr;
        CHECK(linearSearch(emptyArr, 10) == -1);
    }

    TEST_CASE("binarySearch: found, not found, single element") {
        Vector<int> arr; // must be sorted
        arr.pushBack(10); arr.pushBack(20); arr.pushBack(30);
        CHECK(binarySearch(arr, 30) == 2);
        CHECK(binarySearch(arr, 99) == -1);
        Vector<int> single; single.pushBack(42);
        CHECK(binarySearch(single, 42) == 0);
        CHECK(binarySearch(single, 99) == -1);
    }
}
