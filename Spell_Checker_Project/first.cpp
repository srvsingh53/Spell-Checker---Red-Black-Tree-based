#include <iostream>
#include <string>
#include <algorithm>

// Define the Red-Black Tree Node
struct Node {
    std::string word;
    Node* parent;
    Node* left;
    Node* right;
    bool is_red; // True for red, false for black

    // Constructor
    Node(std::string word) : word(word), parent(nullptr), left(nullptr), right(nullptr), is_red(true) {}
};

class RedBlackTree {
private:
    Node* root;

    // Helper functions
    void rotateLeft(Node* node);
    void rotateRight(Node* node);
    void fixInsertion(Node* node);
    void insertNode(Node* node);
    bool searchWord(Node* node, const std::string& word) const;
    void inOrderTraversal(Node* node) const;

public:
    RedBlackTree() : root(nullptr) {}
    void insertWord(const std::string& word);
    bool isWordCorrect(const std::string& word) const;
    void printDictionary() const;
};

// Rotate the tree left
void RedBlackTree::rotateLeft(Node* node) {
    if (!node || !node->right) return;

    Node* right_child = node->right;
    node->right = right_child->left;

    if (right_child->left)
        right_child->left->parent = node;

    right_child->parent = node->parent;

    if (!node->parent)
        root = right_child;
    else if (node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;

    right_child->left = node;
    node->parent = right_child;
}

// Rotate the tree right
void RedBlackTree::rotateRight(Node* node) {
    if (!node || !node->left) return;

    Node* left_child = node->left;
    node->left = left_child->right;

    if (left_child->right)
        left_child->right->parent = node;

    left_child->parent = node->parent;

    if (!node->parent)
        root = left_child;
    else if (node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;

    left_child->right = node;
    node->parent = left_child;
}

// Fix the tree after node insertion
void RedBlackTree::fixInsertion(Node* node) {
    while (node != root && node->parent->is_red) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;

        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle && uncle->is_red) {
                parent->is_red = false;
                uncle->is_red = false;
                grandparent->is_red = true;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                }
                parent->is_red = false;
                grandparent->is_red = true;
                rotateRight(grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            if (uncle && uncle->is_red) {
                parent->is_red = false;
                uncle->is_red = false;
                grandparent->is_red = true;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                }
                parent->is_red = false;
                grandparent->is_red = true;
                rotateLeft(grandparent);
            }
        }
    }
    root->is_red = false;
}

// Insert a word into the Red-Black Tree
void RedBlackTree::insertNode(Node* node) {
    if (!root) {
        root = node;
        root->is_red = false;
    } else {
        Node* current = root;
        Node* parent = nullptr;
        while (current) {
            parent = current;
            if (node->word < current->word)
                current = current->left;
            else
                current = current->right;
        }

        node->parent = parent;
        if (node->word < parent->word)
            parent->left = node;
        else
            parent->right = node;

        fixInsertion(node);
    }
}

// Public function to insert a word into the Red-Black Tree
void RedBlackTree::insertWord(const std::string& word) {
    Node* new_node = new Node(word);
    insertNode(new_node);
}

// Search for a word in the Red-Black Tree
bool RedBlackTree::searchWord(Node* node, const std::string& word) const {
    if (!node) return false;

    if (word == node->word)
        return true;
    else if (word < node->word)
        return searchWord(node->left, word);
    else
        return searchWord(node->right, word);
}

// Public function to check if a word is correct (exists in the dictionary)
bool RedBlackTree::isWordCorrect(const std::string& word) const {
    return searchWord(root, word);
}

// In-order traversal to print the dictionary
void RedBlackTree::inOrderTraversal(Node* node) const {
    if (!node) return;

    inOrderTraversal(node->left);
    std::cout << node->word << " ";
    inOrderTraversal(node->right);
}

// Public function to print the dictionary
void RedBlackTree::printDictionary() const {
    std::cout << "Dictionary: ";
    inOrderTraversal(root);
    std::cout << std::endl;
}

int main() {
    RedBlackTree dictionary;

    // Populate the dictionary with some sample words
    dictionary.insertWord("apple");
    dictionary.insertWord("banana");
    dictionary.insertWord("cherry");
    dictionary.insertWord("orange");
    dictionary.insertWord("pear");

    dictionary.printDictionary();

    // Check if words are correct
    std::string word;
    std::cout << "Enter a word to check its correctness: ";
    std::cin >> word;
    if (dictionary.isWordCorrect(word))
        std::cout << word << " is spelled correctly." << std::endl;
    else
        std::cout << word << " is misspelled or not found in the dictionary." << std::endl;

    return 0;
}
