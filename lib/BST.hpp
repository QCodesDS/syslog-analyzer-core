#ifndef BST_HPP
#define BST_HPP

#include "Vector.hpp"

template<typename T>
struct BSTNode {
    T value;
    BSTNode<T>* left;
    BSTNode<T>* right;
    BSTNode(const T& value) {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template<typename T>
struct BST {
    BSTNode<T>* root;
    BST() { this->root = nullptr; }
    BST(const BST& other) { this->root = copyTree(other.root); }
    void insert(const T& value) { this->root = insertValue(value, this->root); }
    void remove(const T& value) { this->root = removeValue(value, this->root); }
    bool search(const T& value) const {
        BSTNode<T>* searchNode = this->root;
        while (searchNode) {
            if (searchNode->value == value) {
                return true;
            } else if (searchNode->value > value) {
                searchNode = searchNode->left;
            } else {
                searchNode = searchNode->right;
            }
        }
        return false;
    }

    Vector<T> lnr() const {
        Vector<T> result;
        lnr(this->root, result);
        return result;
    }
    Vector<T> nlr() const {
        Vector<T> result;
        nlr(this->root, result);
        return result;
    }
    Vector<T> lrn() const {
        Vector<T> result;
        lrn(this->root, result);
        return result;
    }

    ~BST() {
        clear(this->root);
        this->root = nullptr;
    }

    BST& operator=(const BST& other) {
        if (this == &other) {
            return *this;
        }
        clear(this->root);
        this->root = copyTree(other.root);
        return *this;
    }

private:
    BSTNode<T>* insertValue(const T& val, BSTNode<T>* root) {
        if (!root) {
            return new BSTNode<T>(val);
        }
        if (val < root->value) {
            root->left = insertValue(val, root->left);
        } else if (val > root->value) {
            root->right = insertValue(val, root->right);
        }
        return root;
    }
    BSTNode<T>* removeValue(const T& val, BSTNode<T>* root) {
        if (!root) {
            return nullptr;
        }
        if (val < root->value) {
            root->left = removeValue(val, root->left);
        } else if (val > root->value) {
            root->right = removeValue(val, root->right);
        } else {
            // 2 con
            if (root->left && root->right) {
                BSTNode<T>* minRight = findLeftMost(root->right);
                root->value = minRight->value;
                root->right = removeValue(minRight->value, root->right);
            } else {
                BSTNode<T>* child = root->left ? root->left : root->right;
                delete root;
                root = child;
            }
        }
        return root;
    }

    void clear(BSTNode<T>* root) {
        if (!root) {
            return;
        }
        clear(root->left);
        clear(root->right);
        delete root;
    }

    BSTNode<T>* findLeftMost(BSTNode<T>* root) {
        if (!root) {
            return nullptr;
        }
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    void lnr(BSTNode<T>* root, Vector<T>& result) const {
        if (!root) {
            return;
        }
        lnr(root->left, result);
        result.pushBack(root->value);
        lnr(root->right, result);
    }

    void nlr(BSTNode<T>* root, Vector<T>& result) const {
        if (!root) {
            return;
        }
        result.pushBack(root->value);
        nlr(root->left, result);
        nlr(root->right, result);
    }

    void lrn(BSTNode<T>* root, Vector<T>& result) const {
        if (!root) {
            return;
        }
        lrn(root->left, result);
        lrn(root->right, result);
        result.pushBack(root->value);
    }

    BSTNode<T>* copyTree(BSTNode<T>* otherRoot) {
        if (!otherRoot) {
            return nullptr;
        }
        BSTNode<T>* newRoot = new BSTNode<T>(otherRoot->value);
        newRoot->left = copyTree(otherRoot->left);
        newRoot->right = copyTree(otherRoot->right);
        return newRoot;
    }
};

#endif  // BST_HPP
