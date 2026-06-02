#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <stdexcept>

template<typename T>
struct Vector {
    int capacity;
    int size;
    T* arr;
    static const int DEFAULT_CAPACITY = 100;
    static const int EXPAND_COEFFICIENT = 2;

    Vector(int len, const T& defaultValue) {
        this->capacity = std::max(len, DEFAULT_CAPACITY);
        this->arr = new T[this->capacity];
        this->size = len;
        for (int i = 0; i < len; i++) {
            this->arr[i] = defaultValue;
        }
    }

    Vector() {
        this->arr = new T[DEFAULT_CAPACITY];
        this->capacity = DEFAULT_CAPACITY;
        this->size = 0;
    }

    void pushBack(const T& value) {
        if ((this->size + 1) > this->capacity) {
            int oldSize = this->size;
            this->capacity *= EXPAND_COEFFICIENT;
            T* newArr = new T[this->capacity];
            // Copy phan cu qua mang moi
            for (int i = 0; i < oldSize; i++) {
                newArr[i] = this->arr[i];
            }
            delete[] this->arr;
            this->arr = newArr;
        }
        this->arr[this->size] = value;
        this->size++;
    }

    void popBack() { this->size = std::max(this->size - 1, 0); }

    T& operator[](int idx) {
        if (idx >= 0 && idx < this->size) {
            return this->arr[idx];
        }
        throw std::runtime_error("Index out of bound");
    }

    const T& operator[](int idx) const {
        if (idx >= 0 && idx < this->size) {
            return this->arr[idx];
        }
        throw std::runtime_error("Index out of bound");
    }

    ~Vector() { clear(); }

    Vector(const Vector& other) {
        this->capacity = other.capacity;
        this->size = other.size;
        this->arr = new T[capacity];

        for (int i = 0; i < size; i++) {
            this->arr[i] = other.arr[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this == &other)
            return *this;

        clear();

        this->capacity = other.capacity;
        this->size = other.size;
        this->arr = new T[capacity];

        for (int i = 0; i < size; i++) {
            this->arr[i] = other.arr[i];
        }

        return *this;
    }

private:
    void clear() {
        if (this->arr) {
            delete[] arr;
            this->arr = nullptr;
            this->capacity = 0;
            this->size = 0;
        }
    }
};

#endif  // VECTOR_HPP
