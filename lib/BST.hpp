#ifndef BST_HPP
#define BST_HPP

#include "Vector.hpp"

template <typename T>
struct BSTNode {
    T value;
    BSTNode* left;
    BSTNode* right;
    int height; // set to 0 for reuse in AVL

    BSTNode(const T& val) : value(val), left(nullptr), right(nullptr), height(0) {}
};

template <typename T>
class BST {
private:
    BSTNode<T>* root;

public:
    BSTNode<T>* getRoot() const { return root; }

    // Initializes an empty Binary Search Tree
    BST() : root(nullptr) {}

    // Delete copy operations to prevent double free
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    // Destructor to free all heap memory
    ~BST() {
        clearTree();
    }

    // Fully clears the tree
    void clearTree() {
        clear(root);
        root = nullptr;
    }

    // Inserts a value into the BST
    void insert(const T& value) {
        root = insertNode(root, value);
    }

    // Removes a value from the BST
    void remove(const T& value) {
        root = removeNode(root, value);
    }

    // Searches for a value in the BST
    bool search(const T& value) const {
        return searchNode(root, value);
    }

    // Returns a vector containing the in-order traversal (LNR) of the BST
    Vector<T> lnr() const {
        Vector<T> result;
        inOrder(root, result);
        return result;
    }

    // Returns a vector containing the pre-order traversal (NLR) of the BST
    Vector<T> nlr() const {
        Vector<T> result;
        preOrder(root, result);
        return result;
    }

    // Returns a vector containing the post-order traversal (LRN) of the BST
    Vector<T> lrn() const {
        Vector<T> result;
        postOrder(root, result);
        return result;
    }

private:
    // Helper to clear the tree
    void clear(BSTNode<T>* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // Helper to insert a node
    BSTNode<T>* insertNode(BSTNode<T>* node, const T& value) {
        if (!node) return new BSTNode<T>(value);
        if (value < node->value) {
            node->left = insertNode(node->left, value);
        } else if (value > node->value) {
            node->right = insertNode(node->right, value);
        }
        return node;
    }

    // Helper to find the minimum value node in a subtree
    BSTNode<T>* findMin(BSTNode<T>* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // Helper to remove a node
    BSTNode<T>* removeNode(BSTNode<T>* node, const T& value) {
        if (!node) return nullptr;

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        } else if (value > node->value) {
            node->right = removeNode(node->right, value);
        } else {
            if (!node->left) {
                BSTNode<T>* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (!node->right) {
                BSTNode<T>* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                BSTNode<T>* minNode = findMin(node->right);
                node->value = minNode->value;
                node->right = removeNode(node->right, minNode->value);
            }
        }
        return node;
    }

    // Helper to search for a value
    bool searchNode(BSTNode<T>* node, const T& value) const {
        if (!node) return false;
        if (value == node->value) return true;
        if (value < node->value) return searchNode(node->left, value);
        return searchNode(node->right, value);
    }

    // Helpers for traversal
    void inOrder(BSTNode<T>* node, Vector<T>& vec) const {
        if (node) {
            inOrder(node->left, vec);
            vec.pushBack(node->value);
            inOrder(node->right, vec);
        }
    }

    void preOrder(BSTNode<T>* node, Vector<T>& vec) const {
        if (node) {
            vec.pushBack(node->value);
            preOrder(node->left, vec);
            preOrder(node->right, vec);
        }
    }

    void postOrder(BSTNode<T>* node, Vector<T>& vec) const {
        if (node) {
            postOrder(node->left, vec);
            postOrder(node->right, vec);
            vec.pushBack(node->value);
        }
    }
};

#endif // BST_HPP
