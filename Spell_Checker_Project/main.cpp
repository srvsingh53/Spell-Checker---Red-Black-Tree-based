#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <limits>
#include <sstream>

using namespace std;

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

    // Helper function to find suggestions for misspelled words
    void findSuggestions(Node* node, const std::string& word, std::vector<std::string>& suggestions, size_t max_distance) const;
    
    // Helper function to calculate Levenshtein distance
    size_t levenshteinDistance(const std::string& word1, const std::string& word2) const;

public:
    RedBlackTree() : root(nullptr) {}
    void insertWord(const std::string& word);
    bool isWordCorrect(const std::string& word) const;
    void printDictionary() const;
    void loadDictionaryFromFile(const std::string& filename);
    std::vector<std::string> suggestCorrections(const std::string& word, size_t max_distance = 2) const;
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
            else if(node->word > current->word){
                current = current->right;}
                else{
                    return;
                }
        }

        node->parent = parent;
        if (node->word < parent->word)
            parent->left = node;
        else
            parent->right = node;

        fixInsertion(node);
    }
}

// Search for a word in the Red-Black Tree
bool RedBlackTree::searchWord(Node* node, const std::string& word) const {
    if (!node) {
        //cout<<"returning"<<endl;
        return false;}

    if (word == node->word)
        return true;
    else if (word < node->word)
        return searchWord(node->left, word);
    else
        return searchWord(node->right, word);
}

// In-order traversal to print the dictionary
void RedBlackTree::inOrderTraversal(Node* node) const {
    if (!node) return;

    inOrderTraversal(node->left);
    std::cout << node->word << " ";
    inOrderTraversal(node->right);
}

// Calculate Levenshtein distance between two strings
// the Levenshtein distance algorithm using dynamic programming to calculate the minimum number of 
//single-character edits (insertions, deletions, or substitutions) required to convert one string (word1) into another string (word2).

size_t RedBlackTree::levenshteinDistance(const std::string& word1, const std::string& word2) const {
    size_t m = word1.length();
    size_t n = word2.length();
    std::vector<std::vector<size_t>> dp(m + 1, std::vector<size_t>(n + 1, 0));

    for (size_t i = 0; i <= m; ++i)
        dp[i][0] = i;

    for (size_t j = 0; j <= n; ++j)
        dp[0][j] = j;

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (word1[i - 1] == word2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
    }

    return dp[m][n];
}

// Helper function to find suggestions for misspelled words
void RedBlackTree::findSuggestions(Node* node, const std::string& word, std::vector<std::string>& suggestions, size_t max_distance) const {
    if (!node) return;

    // Calculate Levenshtein distance between the word and the current node's word
    size_t distance = levenshteinDistance(word, node->word);

    // If the distance is within the threshold, add it to the suggestions
    if (distance <= max_distance)
        suggestions.push_back(node->word);
        

    // Recursively search the left and right subtrees for more suggestions
    if (word < node->word)
        findSuggestions(node->left, word, suggestions, max_distance);
    else
        findSuggestions(node->right, word, suggestions, max_distance);
}

// Public function to insert a word into the Red-Black Tree
void RedBlackTree::insertWord(const std::string& word) {
    //cout<<"call for inserting->"<<word<<endl;
    Node* new_node = new Node(word);
    insertNode(new_node);
}

// Public function to check if a word is correct (exists in the dictionary)
bool RedBlackTree::isWordCorrect(const std::string& word) const {
    return searchWord(root, word);
}

// Public function to print the dictionary
void RedBlackTree::printDictionary() const {
    std::cout << "Dictionary: ";
    inOrderTraversal(root);
    std::cout << std::endl;
}



void RedBlackTree::loadDictionaryFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line); // Create a string stream for the line
        std::string word;
        while (iss >> word) { // Extract individual words from the line
            // Remove leading/trailing whitespaces
            size_t first_non_whitespace = word.find_first_not_of(" \t\r\n");
            size_t last_non_whitespace = word.find_last_not_of(" \t\r\n");
            if (first_non_whitespace != std::string::npos && last_non_whitespace != std::string::npos) {
                word = word.substr(first_non_whitespace, last_non_whitespace - first_non_whitespace + 1);

                // Ignore empty words
                if (!word.empty())
                    insertWord(word);
            }
        }
    }

    file.close();
}



// Public function to find suggestions for misspelled words
std::vector<std::string> RedBlackTree::suggestCorrections(const std::string& word, size_t max_distance) const {
    std::vector<std::string> suggestions;
    findSuggestions(root, word, suggestions, max_distance);
    return suggestions;
}

int main() {
    RedBlackTree dictionary;

    // Load dictionary from file (assuming "dictionary.txt" exists in the current directory)
     dictionary.loadDictionaryFromFile("dictionary2.txt");
   

    //dictionary.printDictionary();

    // Check if words are correct and provide suggestions for misspelled words
    while(true){
    std::string word;
    cout << "Enter a word to check its correctness or enter -1 to exit : ";
    cin >> word;
    if(word == "-1"){
        cout<<"Taking exit-------Thanks for using"<<endl;
        break;
    }

    if (dictionary.isWordCorrect(word))
        cout << word << " is spelled correctly." << endl;
    else {
        cout<<word<<" is spelled incorrectly"<<endl;
        cout<<"Do you want some suggestions for the corrections for the above incorrect word---> 0/1 : ";
        int input;
        cin>>input;
        if(input==1){
        vector<std::string> suggestions = dictionary.suggestCorrections(word, 2);
        if (!suggestions.empty()) {
            std::cout << "Suggestions for the word: ";
            for (const std::string& suggestion : suggestions) {
                std::cout << suggestion << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "No suggestions found." << std::endl;
        }
         } }
    }
    return 0;
}
