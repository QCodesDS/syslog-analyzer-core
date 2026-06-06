#ifndef STACK_HPP
#define STACK_HPP
#include <stdexcept>

#include "LinkedList.hpp"

template<typename T>
struct Stack {
    LinkedList<T> ll;
    Stack() {}

    void push(const T& value) { this->ll.insertFront(value); }
    bool empty() { return this->ll.empty(); }
    int size() { return this->ll.size(); }
    bool pop() { return this->ll.removeAt(0); }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("There is nothing in stack to operate top()");
        }
        return this->ll.head->value;
    }

    T& top() {
        if (empty()) {
            throw std::runtime_error("There is nothing in stack to operate top()");
        }
        return this->ll.head->value;
    }
};

#endif  // STACK_HPP
