#include <fstream>

// ... (previous code)

class RedBlackTree {
private:
    // ... (previous code)

    // Helper function to read words from a file and insert into the Red-Black Tree
    void loadDictionaryFromFile(const std::string& filename);

    // ... (previous code)
};

// Load words from a file and insert them into the Red-Black Tree
void RedBlackTree::loadDictionaryFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string word;
    while (std::getline(file, word)) {
        // Remove leading/trailing whitespaces
        word.erase(word.find_last_not_of(" \t\r\n") + 1);

        // Ignore empty lines
        if (!word.empty())
            insertWord(word);
    }

    file.close();
}

int main() {
    RedBlackTree dictionary;

    // Load dictionary from file (assuming "dictionary.txt" exists in the current directory)
    dictionary.loadDictionaryFromFile("dictionary.txt");

    dictionary.printDictionary();

    // ... (rest of the main function remains the same)

    return 0;
}
