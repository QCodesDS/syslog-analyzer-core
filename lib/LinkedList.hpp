#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <stdexcept>

template<typename T>
struct LinkedListNode {
    T value;
    LinkedListNode* next;
    LinkedListNode* prev;

    LinkedListNode(const T& val) : value(val), next(nullptr), prev(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    LinkedListNode<T>* head;
    LinkedListNode<T>* tail;
    int listSize;

public:
    LinkedListNode<T>* getHead() const { return head; }
    LinkedListNode<T>* getTail() const { return tail; }

    // Initializes an empty linked list
    LinkedList() : head(nullptr), tail(nullptr), listSize(0) {}

    // Destructor to free all heap memory
    ~LinkedList() { clear(); }

    // Copy constructor for deep copy
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), listSize(0) {
        LinkedListNode<T>* current = other.head;
        while (current) {
            insertBack(current->value);
            current = current->next;
        }
    }

    // Assignment operator for deep copy
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            LinkedListNode<T>* current = other.head;
            while (current) {
                insertBack(current->value);
                current = current->next;
            }
        }
        return *this;
    }

    // Inserts a value at the front of the list
    void insertFront(const T& value) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        listSize++;
    }

    // Inserts a value at the back of the list
    void insertBack(const T& value) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        listSize++;
    }

    // Inserts a value at a specific index
    void insertAt(int index, const T& value) {
        if (index < 0 || index > listSize) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) {
            insertFront(value);
        } else if (index == listSize) {
            insertBack(value);
        } else {
            LinkedListNode<T>* current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
            newNode->prev = current->prev;
            newNode->next = current;
            current->prev->next = newNode;
            current->prev = newNode;
            listSize++;
        }
    }

    // Removes the first occurrence of a value from the list
    bool remove(const T& value) {
        LinkedListNode<T>* current = head;
        while (current) {
            if (current->value == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
                delete current;
                listSize--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Removes the element at a specific index
    void removeAt(int index) {
        if (index < 0 || index >= listSize) {
            throw std::out_of_range("Index out of bounds");
        }
        LinkedListNode<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }
        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }
        delete current;
        listSize--;
    }

    // Finds if a value exists in the list
    bool find(const T& value) const {
        LinkedListNode<T>* current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Returns the number of elements in the list
    int size() const { return listSize; }

    // Removes all elements from the list
    void clear() {
        LinkedListNode<T>* current = head;
        while (current) {
            LinkedListNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        listSize = 0;
    }
};

#endif  // LINKEDLIST_HPP
