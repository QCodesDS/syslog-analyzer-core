#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>

struct TrieNode {
    TrieNode* children[128];
    bool isEnd;

    TrieNode() : isEnd(false) {
        for (int i = 0; i < 128; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

    void clearNode(TrieNode* node) {
        if (!node)
            return;
        for (int i = 0; i < 128; i++) {
            if (node->children[i]) {
                clearNode(node->children[i]);
            }
        }
        delete node;
    }

    TrieNode* copyNode(TrieNode* node) {
        if (!node)
            return nullptr;
        TrieNode* newNode = new TrieNode();
        newNode->isEnd = node->isEnd;
        for (int i = 0; i < 128; i++) {
            if (node->children[i]) {
                newNode->children[i] = copyNode(node->children[i]);
            }
        }
        return newNode;
    }

public:
    Trie() { root = new TrieNode(); }

    ~Trie() {
        clearNode(root);
        root = nullptr;
    }

    Trie(const Trie& other) { root = copyNode(other.root); }

    Trie& operator=(const Trie& other) {
        if (this != &other) {
            clearNode(root);
            root = copyNode(other.root);
        }
        return *this;
    }

    // Inserts a keyword string into the trie
    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = static_cast<unsigned char>(c);
            if (index >= 128)
                continue;
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEnd = true;
    }

    // Returns true if the exact word exists in the trie
    bool search(const std::string& word) const {
        TrieNode* current = root;
        for (char c : word) {
            int index = static_cast<unsigned char>(c);
            if (index >= 128 || !current->children[index]) {
                return false;
            }
            current = current->children[index];
        }
        return current != nullptr && current->isEnd;
    }

    // Returns true if any inserted word starts with the given prefix
    bool startsWith(const std::string& prefix) const {
        TrieNode* current = root;
        for (char c : prefix) {
            int index = static_cast<unsigned char>(c);
            if (index >= 128 || !current->children[index]) {
                return false;
            }
            current = current->children[index];
        }
        return current != nullptr;
    }

    // Removes all keywords from the trie
    void clear() {
        clearNode(root);
        root = new TrieNode();
    }
};

#endif  // TRIE_HPP