#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include "Vector.hpp"
#include <stdexcept>
#include <functional>

template <typename T, typename Comp = std::less<T>>
class PriorityQueue {
private:
    Vector<T> heap;
    Comp cmp;

public:

    // Initializes an empty priority queue
    PriorityQueue() : heap(), cmp() {}

    // Destructor
    ~PriorityQueue() = default;

    // Copy constructor
    PriorityQueue(const PriorityQueue& other) : heap(other.heap), cmp(other.cmp) {}

    // Assignment operator
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            heap = other.heap;
            cmp = other.cmp;
        }
        return *this;
    }

    // Inserts a value into the priority queue
    void insert(const T& value) {
        heap.pushBack(value);
        heapifyUp(heap.getSize() - 1);
    }

    // Extracts and removes the top value from the priority queue
    void extract() {
        if (empty()) {
            throw std::out_of_range("Priority Queue is empty");
        }
        heap[0] = heap[heap.getSize() - 1];
        heap.popBack();
        if (!empty()) {
            heapifyDown(0);
        }
    }

    // Returns a reference to the top value in the priority queue
    T& peek() {
        if (empty()) {
            throw std::out_of_range("Priority Queue is empty");
        }
        return heap[0];
    }

    // Returns true if the priority queue has no elements
    bool empty() const {
        return heap.getSize() == 0;
    }

    // Returns the number of elements in the priority queue
    int size() const {
        return heap.getSize();
    }

private:
    // Moves the element at index up to maintain heap property
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (cmp(heap[parent], heap[index])) {
                T temp = heap[parent];
                heap[parent] = heap[index];
                heap[index] = temp;
                index = parent;
            } else {
                break;
            }
        }
    }

    // Moves the element at index down to maintain heap property
    void heapifyDown(int index) {
        int n = heap.getSize();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < n && cmp(heap[largest], heap[left])) {
                largest = left;
            }
            if (right < n && cmp(heap[largest], heap[right])) {
                largest = right;
            }
            if (largest != index) {
                T temp = heap[index];
                heap[index] = heap[largest];
                heap[largest] = temp;
                index = largest;
            } else {
                break;
            }
        }
    }
};

#endif // PRIORITYQUEUE_HPP
