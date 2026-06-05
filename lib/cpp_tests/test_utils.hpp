#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <algorithm>

#define EXPECT_TRUE(expr) assert((expr))
#define EXPECT_FALSE(expr) assert(!(expr))
#define EXPECT_EQ(actual, expected) assert((actual) == (expected))
#define EXPECT_NE(actual, expected) assert((actual) != (expected))

#define EXPECT_THROW(stmt)                 \
    do {                                   \
        auto thrown = false;               \
        try {                              \
            stmt;                          \
        } catch (...) {                    \
            thrown = true;                 \
        }                                  \
        assert(thrown);                    \
    } while (false)

#define RUN_TEST(test_name)                \
    do {                                   \
        std::cout << "[ RUN      ] "       \
                  << #test_name << '\n';   \
        test_name();                       \
        std::cout << "[       OK ] "       \
                  << #test_name << '\n';   \
    } while (false)

#endif
