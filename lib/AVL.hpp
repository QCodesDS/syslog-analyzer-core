#ifndef AVL_HPP
#define AVL_HPP

#include "Vector.hpp"

template<typename T>
struct AVLNode {
    T value;
    AVLNode<T>* left;
    AVLNode<T>* right;
    int height;
    AVLNode(const T& value) {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 0;
    }
};

template<typename T>
struct AVL {
    AVLNode<T>* root;
    AVL() { this->root = nullptr; }
    AVL(const AVL& other) { this->root = copyTree(other.root); }
    void insert(const T& value) { this->root = insertValue(value, this->root); }
    void remove(const T& value) { this->root = removeValue(value, this->root); }
    bool search(const T& value) const {
        AVLNode<T>* searchNode = this->root;
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

    T* find(const T& value) {
        AVLNode<T>* searchNode = this->root;
        while (searchNode) {
            if (searchNode->value == value) {
                return &searchNode->value;
            } else if (searchNode->value > value) {
                searchNode = searchNode->left;
            } else {
                searchNode = searchNode->right;
            }
        }
        return nullptr;
    }

    void clear(AVLNode<T>* root) {
        if (!root) {
            return;
        }
        clear(root->left);
        clear(root->right);
        delete root;
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

    ~AVL() {
        clear(this->root);
        this->root = nullptr;
    }

    AVL& operator=(const AVL& other) {
        if (this == &other) {
            return *this;
        }
        clear(this->root);
        this->root = copyTree(other.root);
        return *this;
    }

private:
    AVLNode<T>* insertValue(const T& val, AVLNode<T>* root) {
        if (!root) {
            return new AVLNode<T>(val);
        }
        if (val < root->value) {
            root->left = insertValue(val, root->left);
        } else if (val > root->value) {
            root->right = insertValue(val, root->right);
        } else {
            root->value = val;
        }
        root = rebalance(root);
        return root;
    }
    AVLNode<T>* removeValue(const T& val, AVLNode<T>* root) {
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
                AVLNode<T>* minRight = findLeftMost(root->right);
                root->value = minRight->value;
                root->right = removeValue(minRight->value, root->right);
            } else {
                AVLNode<T>* child = root->left ? root->left : root->right;
                delete root;
                root = child;
            }
        }
        root = rebalance(root);
        return root;
    }

    AVLNode<T>* findLeftMost(AVLNode<T>* root) {
        if (!root) {
            return nullptr;
        }
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    void lnr(AVLNode<T>* root, Vector<T>& result) const {
        if (!root) {
            return;
        }
        lnr(root->left, result);
        result.pushBack(root->value);
        lnr(root->right, result);
    }

    void nlr(AVLNode<T>* root, Vector<T>& result) const {
        if (!root) {
            return;
        }
        result.pushBack(root->value);
        nlr(root->left, result);
        nlr(root->right, result);
    }

    void lrn(AVLNode<T>* root, Vector<T>& result) const {
        if (!root) {
            return;
        }
        lrn(root->left, result);
        lrn(root->right, result);
        result.pushBack(root->value);
    }

    AVLNode<T>* copyTree(AVLNode<T>* otherRoot) {
        if (!otherRoot) {
            return nullptr;
        }
        AVLNode<T>* newRoot = new AVLNode<T>(otherRoot->value);
        newRoot->height = otherRoot->height;
        newRoot->left = copyTree(otherRoot->left);
        newRoot->right = copyTree(otherRoot->right);
        return newRoot;
    }

    int getHeight(AVLNode<T>* root) {
        if (!root) {
            return -1;
        }
        return root->height;
    }

    int getHeightFactor(AVLNode<T>* root) {
        if (!root) {
            return 0;
        }
        return getHeight(root->right) - getHeight(root->left);
    }

    void recalculateHeight(AVLNode<T>* root) {
        if (!root) {
            return;
        }
        root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    }

    AVLNode<T>* rotateLeft(AVLNode<T>* root) {
        if (!root || !root->right) {
            return root;
        }
        AVLNode<T>* right = root->right;
        root->right = right->left;
        right->left = root;
        recalculateHeight(root);
        recalculateHeight(right);
        return right;
    }

    AVLNode<T>* rotateRight(AVLNode<T>* root) {
        if (!root || !root->left) {
            return root;
        }
        AVLNode<T>* left = root->left;
        root->left = left->right;
        left->right = root;
        recalculateHeight(root);
        recalculateHeight(left);
        return left;
    }

    AVLNode<T>* rebalance(AVLNode<T>* root) {
        if (!root) {
            return nullptr;
        }
        recalculateHeight(root);
        int factor = getHeightFactor(root);
        if (factor > 1) {                            // right
            if (getHeightFactor(root->right) < 0) {  // left
                root->right = rotateRight(root->right);
            }
            root = rotateLeft(root);
        } else if (factor < -1) {                   // left
            if (getHeightFactor(root->left) > 0) {  // right
                root->left = rotateLeft(root->left);
            }
            root = rotateRight(root);
        }
        return root;
    }
};

#endif  // AVL_HPP
