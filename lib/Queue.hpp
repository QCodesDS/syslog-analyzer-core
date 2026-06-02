#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>

#include "LinkedList.hpp"

template<typename T>
struct Stack {
    LinkedList<T> ll;
    Stack() {}

    void enqueue(const T& value) { this->ll.insertBack(value); }
    bool empty() { return this->ll.empty(); }
    int size() { return this->ll.size(); }
    bool dequeue() { return this->ll.removeAt(0); }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("There is nothing in queue to operate front()");
        }
        return this->ll.head->value;
    }

    T& top() {
        if (empty()) {
            throw std::runtime_error("There is nothing in queue to operate front()");
        }
        return this->ll.head->value;
    }
};

#endif  // QUEUE_HPP
