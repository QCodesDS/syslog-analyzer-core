#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <stdexcept>

template<typename T>
class Vector {
private:
    int capacity;
    int size;
    T* arr;

public:
    static constexpr int DEFAULT_CAPACITY = 100;
    static constexpr int EXPAND_COEFFICIENT = 2;

    // Returns the current number of elements
    int getSize() const { return size; }

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

    // Copy constructor
    Vector(const Vector& other) {
        this->capacity = other.capacity;
        this->size = other.size;
        this->arr = new T[this->capacity];
        for (int i = 0; i < this->size; i++) {
            this->arr[i] = other.arr[i];
        }
    }

    // Assignment operator
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            freeMemory();
            this->capacity = other.capacity;
            this->size = other.size;
            if (this->capacity > 0) {
                this->arr = new T[this->capacity];
                for (int i = 0; i < this->size; i++) {
                    this->arr[i] = other.arr[i];
                }
            } else {
                this->arr = nullptr;
            }
        }
        return *this;
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

    ~Vector() { freeMemory(); }

    // Resets element count to 0 but keeps allocated buffer for reuse (fast reset)
    void clear() { this->size = 0; }

    // Fully deallocates the internal array — called by destructor only
    void freeMemory() {
        if (this->arr) {
            delete[] arr;
            this->arr = nullptr;
            this->capacity = 0;
            this->size = 0;
        }
    }
};

#endif  // VECTOR_HPP
