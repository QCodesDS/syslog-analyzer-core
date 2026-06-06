/**
 * @file test_memory_safety.cpp
 * @brief Tests memory allocation, deallocation, copy semantics, and leak prevention.
 * Expected to be run with AddressSanitizer (-fsanitize=address).
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../lib/AVL.hpp"
#include "../lib/LinkedList.hpp"
#include "../lib/HashTable.hpp"
#include "../lib/Trie.hpp"
#include "../lib/Stack.hpp"

/**
 * @brief Test Suite for Memory Safety
 * Verifies that destructors, copy constructors, and assignment operators
 * correctly manage heap memory without double frees or leaks.
 */
TEST_SUITE("Memory Safety") {
    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("1. AVL: insert 1000, destroy - no crash (destructor test)") {
        AVL<int>* tree = new AVL<int>();
        for(int i=0; i<1000; i++) {
            tree->insert(i);
        }
        delete tree; // Should cleanly free all 1000 nodes
        CHECK(true);
    }

    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("2. LinkedList: copy constructor - deep copy verified") {
        LinkedList<int> original;
        original.insertBack(10);
        original.insertBack(20);
        
        LinkedList<int> copy(original);
        copy.insertBack(30);
        
        CHECK(original.size() == 2);
        CHECK(copy.size() == 3);
        // Clean up handles both safely
    }

    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("3. LinkedList: operator= - deep copy, no double free") {
        LinkedList<int> original;
        original.insertBack(10);
        
        LinkedList<int> assigned;
        assigned.insertBack(99);
        assigned = original;
        assigned.insertBack(20);
        
        CHECK(original.size() == 1);
        CHECK(assigned.size() == 2);
    }

    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("4. HashTable: insert 500 pairs, clear(), insert 500 more - no crash") {
        HashTable<int, int> ht;
        for(int i=0; i<500; i++) {
            ht.insert(Pair<int, int>(i, i*10));
        }
        ht.clear();
        for(int i=0; i<500; i++) {
            ht.insert(Pair<int, int>(i+1000, i*10));
        }
        CHECK(ht.size() == 500);
    }

    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("5. Trie: insert 100 words, clear(), search all - returns false") {
        Trie t;
        for(int i=0; i<100; i++) {
            t.insert("word" + std::to_string(i));
        }
        t.clear();
        for(int i=0; i<100; i++) {
            CHECK(t.search("word" + std::to_string(i)) == false);
        }
    }

    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("6. Nested scope: create AVL inside block, goes out of scope - no leak") {
        {
            AVL<int> scopedTree;
            for(int i=0; i<100; i++) scopedTree.insert(i);
        }
        // ASAN will flag if memory from scopedTree wasn't freed
        CHECK(true);
    }

    // Run with -fsanitize=address on Linux for full leak detection
    TEST_CASE("7. Stack: push 1000, pop all, push 1000 again - no crash") {
        Stack<int> s;
        for(int i=0; i<1000; i++) s.push(i);
        for(int i=0; i<1000; i++) s.pop();
        for(int i=0; i<1000; i++) s.push(i);
        CHECK(s.size() == 1000);
    }
}
