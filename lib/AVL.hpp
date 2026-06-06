#ifndef AVL_HPP
#define AVL_HPP

#include <algorithm>

#include "Vector.hpp"

template<typename T>
struct AVLNode {
    T value;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const T& val) : value(val), left(nullptr), right(nullptr), height(0) {}
};

template<typename T>
class AVL {
private:
    AVLNode<T>* root;

public:
    AVLNode<T>* getRoot() const { return root; }
    int height() const { return getHeight(root); }

    // Initializes an empty AVL Tree
    AVL() : root(nullptr) {}

    // Delete copy operations to prevent double free
    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    // Destructor to free all heap memory
    ~AVL() { clearTree(); }

    // Fully clears the tree
    void clearTree() {
        clear(root);
        root = nullptr;
    }

    // Inserts a value into the AVL tree, rebalancing as necessary
    void insert(const T& value) { root = insertNode(root, value); }

    // Removes a value from the AVL tree, rebalancing as necessary
    void remove(const T& value) { root = removeNode(root, value); }

    // Searches for a value in the AVL tree
    bool search(const T& value) const { return searchNode(root, value); }

    // Finds and returns a pointer to the stored value, or nullptr if not found
    T* find(const T& value) { return findNode(root, value); }

    // Returns a vector containing the in-order traversal (LNR) of the AVL tree
    Vector<T> lnr() const {
        Vector<T> result;
        inOrder(root, result);
        return result;
    }

    // Returns a vector containing the pre-order traversal (NLR) of the AVL tree
    Vector<T> nlr() const {
        Vector<T> result;
        preOrder(root, result);
        return result;
    }

    // Returns a vector containing the post-order traversal (LRN) of the AVL tree
    Vector<T> lrn() const {
        Vector<T> result;
        postOrder(root, result);
        return result;
    }

private:
    // Helper to get the height of a node
    int getHeight(AVLNode<T>* node) const { return node ? node->height : -1; }

    // Helper to get the balance factor of a node
    int getBalanceFactor(AVLNode<T>* node) const {
        if (!node)
            return 0;
        return getHeight(node->right) - getHeight(node->left);
    }

    // Helper to recalculate the height of a node
    void recalculateHeight(AVLNode<T>* node) {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Performs a right rotation
    AVLNode<T>* rotateRight(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        recalculateHeight(y);
        recalculateHeight(x);

        return x;
    }

    // Performs a left rotation
    AVLNode<T>* rotateLeft(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        recalculateHeight(x);
        recalculateHeight(y);

        return y;
    }

    // Rebalances a node if its balance factor is out of bounds
    AVLNode<T>* rebalance(AVLNode<T>* node) {
        recalculateHeight(node);
        int balance = getBalanceFactor(node);

        // Right heavy
        if (balance > 1) {
            if (getBalanceFactor(node->right) < 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        // Left heavy
        if (balance < -1) {
            if (getBalanceFactor(node->left) > 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        return node;
    }

    // Helper to insert a node and rebalance
    AVLNode<T>* insertNode(AVLNode<T>* node, const T& value) {
        if (!node)
            return new AVLNode<T>(value);
        if (value < node->value) {
            node->left = insertNode(node->left, value);
        } else if (value > node->value) {
            node->right = insertNode(node->right, value);
        } else {
            return node;  // Duplicates not allowed or simply ignored
        }
        return rebalance(node);
    }

    // Helper to find the minimum value node in a subtree
    AVLNode<T>* findMin(AVLNode<T>* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // Helper to remove a node and rebalance
    AVLNode<T>* removeNode(AVLNode<T>* node, const T& value) {
        if (!node)
            return nullptr;

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        } else if (value > node->value) {
            node->right = removeNode(node->right, value);
        } else {
            if (!node->left) {
                AVLNode<T>* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (!node->right) {
                AVLNode<T>* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                AVLNode<T>* minNode = findMin(node->right);
                node->value = minNode->value;
                node->right = removeNode(node->right, minNode->value);
            }
        }
        return rebalance(node);
    }

    // Helper to clear the tree
    void clear(AVLNode<T>* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // Helper to search for a value
    bool searchNode(AVLNode<T>* node, const T& value) const {
        if (!node)
            return false;
        if (value == node->value)
            return true;
        if (value < node->value)
            return searchNode(node->left, value);
        return searchNode(node->right, value);
    }

    // Helper to find a value
    T* findNode(AVLNode<T>* node, const T& value) {
        if (!node)
            return nullptr;
        if (value == node->value)
            return &(node->value);
        if (value < node->value)
            return findNode(node->left, value);
        return findNode(node->right, value);
    }

    // Helpers for traversal
    void inOrder(AVLNode<T>* node, Vector<T>& vec) const {
        if (node) {
            inOrder(node->left, vec);
            vec.pushBack(node->value);
            inOrder(node->right, vec);
        }
    }

    void preOrder(AVLNode<T>* node, Vector<T>& vec) const {
        if (node) {
            vec.pushBack(node->value);
            preOrder(node->left, vec);
            preOrder(node->right, vec);
        }
    }

    void postOrder(AVLNode<T>* node, Vector<T>& vec) const {
        if (node) {
            postOrder(node->left, vec);
            postOrder(node->right, vec);
            vec.pushBack(node->value);
        }
    }
};

#endif  // AVL_HPP
