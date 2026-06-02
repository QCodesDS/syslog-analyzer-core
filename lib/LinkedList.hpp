#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <stdexcept>
template<typename T>
struct LinkedListNode {
    T value;
    LinkedListNode* next;
    LinkedListNode* prev;
    LinkedListNode(const T& val) {
        this->value = val;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

template<typename T>
struct LinkedList {
    LinkedListNode<T>* head;
    LinkedListNode<T>* tail;

    LinkedList() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    LinkedList(const LinkedList& other) {
        this->head = nullptr;
        this->tail = nullptr;

        LinkedListNode<T>* otherP = other.head;
        while (otherP) {
            insertBack(otherP->value);
            otherP = otherP->next;
        }
    }

    void insertFront(const T& value) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
        newNode->next = this->head;
        if (!this->head) {
            this->tail = newNode;
        } else {
            this->head->prev = newNode;
        }
        this->head = newNode;
    }

    void insertBack(const T& value) {
        if (!this->tail) {
            insertFront(value);
        } else {
            LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
            this->tail->next = newNode;
            newNode->prev = this->tail;
            this->tail = newNode;
        }
    }

    void insertAt(int index, const T& value) {
        if (index == 0) {
            insertFront(value);
            return;
        } else if (index < 0) {
            throw std::runtime_error("Index must not be negative");
        }
        LinkedListNode<T>* p = this->head;
        for (int i = 0; i < index; i++) {
            if (!p) {  // loi truy cap qua index
                throw std::runtime_error("Index out of bound in insertion of LinkedList");
            }
            p = p->next;
        }
        if (p) {
            LinkedListNode<T>* newNode = new LinkedListNode<T>(value);
            newNode->prev = p->prev;
            newNode->next = p;
            if (p->prev) {
                p->prev->next = newNode;
            }
            p->prev = newNode;
        } else {  // Truong hop chen vao vi tri sau cung cua linkedlist
            insertBack(value);
        }
    }

    bool remove(const T& value) {
        LinkedListNode<T>* p = this->head;
        while (p) {
            if (p->value == value) {
                removeNode(p);
                return true;
            }
            p = p->next;
        }
        return false;
    }

    bool removeAt(int index) {
        if (index < 0) {
            return false;
        }
        LinkedListNode<T>* p = this->head;
        for (int i = 0; i < index; i++) {
            if (!p) {  // loi truy cap qua index
                return false;
            }
            p = p->next;
        }
        if (p) {
            removeNode(p);
            return true;
        }
        return false;
    }

    bool find(const T& value) {
        LinkedListNode<T>* p = this->head;
        while (p) {
            if (p->value == value) {
                return true;
            }
            p = p->next;
        }
        return false;
    }

    int size() {
        LinkedListNode<T>* p = this->head;
        int size = 0;
        while (p) {
            size++;
            p = p->next;
        }
        return size;
    }

    void clear() {
        LinkedListNode<T>* p = this->head;
        while (p) {
            LinkedListNode<T>* temp = p;
            p = p->next;
            delete temp;
        }
        this->head = nullptr;
        this->tail = nullptr;
    }

    ~LinkedList() { clear(); }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        LinkedListNode<T>* otherP = other.head;
        while (otherP) {
            insertBack(otherP->value);
            otherP = otherP->next;
        }
        return *this;
    }

private:
    void removeNode(LinkedListNode<T>* p) {
        if (!p) {
            return;
        }
        if (p == this->head) {
            this->head = this->head->next;
        }
        if (p == this->tail) {
            this->tail = this->tail->prev;
        }
        // dieu chinh node prev
        if (p->prev) {
            p->prev->next = p->next;
        }
        // dieu chinh node next
        if (p->next) {
            p->next->prev = p->prev;
        }
        delete p;
    }
};
#endif  // LINKEDLIST_HPP
