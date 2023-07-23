#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <limits>

struct Node {
    // Same as before
};

class RedBlackTree {
private:
    // Same as before

    // Helper function to find suggestions for misspelled words
    void findSuggestions(Node* node, const std::string& word, std::vector<std::string>& suggestions, size_t max_distance) const;
    
    // Helper function to calculate Levenshtein distance
    size_t levenshteinDistance(const std::string& word1, const std::string& word2) const;

public:
    // Same as before

    // Public function to find suggestions for misspelled words
    std::vector<std::string> suggestCorrections(const std::string& word, size_t max_distance = 2) const;
};

// Same as before

// Calculate Levenshtein distance between two strings
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

std::vector<std::string> RedBlackTree::suggestCorrections(const std::string& word, size_t max_distance) const {
    std::vector<std::string> suggestions;
    findSuggestions(root, word, suggestions, max_distance);
    return suggestions;
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

    // Check if words are correct and provide suggestions for misspelled words
    std::string word;
    std::cout << "Enter a word to check its correctness and find suggestions: ";
    std::cin >> word;

    if (dictionary.isWordCorrect(word))
        std::cout << word << " is spelled correctly." << std::endl;
    else {
        std::vector<std::string> suggestions = dictionary.suggestCorrections(word, 2);
        if (!suggestions.empty()) {
            std::cout << "Suggestions: ";
            for (const std::string& suggestion : suggestions) {
                std::cout << suggestion << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "No suggestions found." << std::endl;
        }
    }

    return 0;
}
