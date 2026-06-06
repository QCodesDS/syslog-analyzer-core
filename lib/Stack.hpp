#ifndef STACK_HPP
#define STACK_HPP

#include "LinkedList.hpp"

template<typename T>
class Stack {
private:
    LinkedList<T> list;

public:
    // Initializes an empty stack
    Stack() = default;

    // Destructor (automatically calls LinkedList destructor)
    ~Stack() = default;

    // Copy constructor (automatically uses LinkedList copy constructor)
    Stack(const Stack& other) : list(other.list) {}

    // Assignment operator (automatically uses LinkedList operator=)
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    // Pushes a value onto the top of the stack
    void push(const T& value) { list.insertFront(value); }

    // Pops the top value from the stack
    bool pop() {
        if (empty())
            return false;
        list.removeAt(0);
        return true;
    }

    // Returns a reference to the top value of the stack
    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list.getHead()->value;
    }

    // Returns true if the stack has no elements
    bool empty() const { return list.size() == 0; }

    // Returns the number of elements in the stack
    int size() const { return list.size(); }
};

#endif  // STACK_HPP
