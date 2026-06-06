#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "LinkedList.hpp"

template<typename T>
class Queue {
private:
    LinkedList<T> list;

public:
    // Initializes an empty queue
    Queue() = default;

    // Destructor (automatically calls LinkedList destructor)
    ~Queue() = default;

    // Copy constructor
    Queue(const Queue& other) : list(other.list) {}

    // Assignment operator
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    // Enqueues a value at the back of the queue
    void enqueue(const T& value) { list.insertBack(value); }

    // Dequeues the front value from the queue
    bool dequeue() {
        if (empty())
            return false;
        list.removeAt(0);
        return true;
    }

    // Returns a constant reference to the front value of the queue
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return list.getHead()->value;
    }

    // Returns true if the queue has no elements
    bool empty() const { return list.size() == 0; }

    // Returns the number of elements in the queue
    int size() const { return list.size(); }
};

#endif  // QUEUE_HPP
