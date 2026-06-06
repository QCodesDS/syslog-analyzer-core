#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include <stdexcept>

#include "Vector.hpp"

template<typename T, typename Comp = std::less<T>>
struct PriorityQueue {
    Vector<T> arr;
    Comp compare;
    PriorityQueue() {}

    void insert(const T& value) {
        this->arr.pushBack(value);
        heapifyUp(arr.size - 1);
    }

    bool empty() const { return this->arr.size == 0; }

    void extract() {
        if (this->arr.size == 0) {
            return;
        }

        std::swap(this->arr[0], this->arr[this->arr.size - 1]);
        this->arr.popBack();
        heapifyDown(0);
    }

    int size() const { return this->arr.size; }

    T& peek() {
        if (empty()) {
            throw std::runtime_error("There is no more element in PQ to peek");
        }
        return this->arr[0];
    }

private:
    void heapifyUp(int i) {
        while (i != 0) {
            int parent = (i - 1) / 2;
            if (compare(this->arr[parent], this->arr[i])) {
                std::swap(this->arr[i], this->arr[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int i) {
        while (2 * i + 1 < arr.size) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            int best = left;

            if (right < arr.size && compare(arr[best], arr[right])) {
                best = right;
            }

            if (compare(arr[i], arr[best])) {
                std::swap(arr[i], arr[best]);
                i = best;
            } else {
                break;
            }
        }
    }
};

#endif  // PRIORITYQUEUE_HPP
