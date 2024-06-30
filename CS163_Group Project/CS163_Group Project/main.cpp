//#include <iostream>
//#include <vector>
//#include <string>
//#include <fstream>
//using namespace std;
//
//struct trie{
//
//	vector<trie*>  children;
//	bool isend = false;
//	vector<string> definition;
//	trie() : children(26, nullptr) {}
//};
//
//
//string toLowerCase(string& str) {
//	for (int i = 0; i < str.length(); i++) {
//		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
//	}
//	return str;
//}
//void insertWord(trie*& root, string&word, string&definition)
//{
//	word = toLowerCase(word);
//	trie* current = root;
//	for (char c : word)
//	{
//		if (current->children[c - 'a'] == nullptr)
//		{
//			current->children[c - 'a'] = new trie;
//		}
//		current = current->children[c - 'a'];
//	}
//	current->isend = true;
//	current->definition.push_back(definition);
//}
//bool findWordMeaning(trie* root, string& word, vector<string>& meaning)
//{
//	word = toLowerCase(word);
//	trie* current = root;
//	for (char c : word)
//	{
//		if (current->children[c - 'a'] == nullptr)
//		{
//			return false;
//		}
//		current = current->children[c - 'a'];
//	}
//	if (current->isend)
//	{
//		meaning = current->definition;
//		return true;
//	}
//	return false;
//}
//void deleteTrie(trie* root)
//{
//	if (root == nullptr) return;
//	for (trie* child : root->children)
//	{
//		deleteTrie(child);
//	}
//	delete root;
//}
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Structure to hold word and definition pairs
struct WordDefinition {
    std::string word;
    std::string definition;
};

// Dynamic array class for chaining
class DynamicArray {
private:
    WordDefinition* array;
    size_t capacity;
    size_t count;

    // Resize the array when it's full
    void resize() {
        capacity *= 2;
        WordDefinition* newArray = new WordDefinition[capacity];
        for (size_t i = 0; i < count; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }

public:
    // Constructor
    DynamicArray() : capacity(4), count(0) {
        array = new WordDefinition[capacity];
    }

    // Destructor
    ~DynamicArray() {
        delete[] array;
    }

    // Add an element to the array
    void add(const WordDefinition& wd) {
        if (count == capacity) {
            resize();
        }
        array[count++] = wd;
    }

    // Get the number of elements in the array
    size_t size() const {
        return count;
    }

    // Access elements by index
    WordDefinition& operator[](size_t index) {
        return array[index];
    }

    const WordDefinition& operator[](size_t index) const {
        return array[index];
    }
};

// Hash table class with chaining using dynamic arrays
class HashTable {
private:
    DynamicArray* table;
    size_t size;

    // Hash function
    size_t hashFunction(const std::string& key) const {
        size_t hash = 0;
        for (char ch : key) {
            hash = (hash * 31) + ch;
        }
        return hash % (97);
    }

public:
    // Constructor
    HashTable(size_t tableSize) : size(tableSize) {
        table = new DynamicArray[tableSize];
    }

    // Destructor
    ~HashTable() {
        delete[] table;
    }

    // Insert word and definition into the hash table
    void insert(const std::string& word, const std::string& definition) {
        size_t index = hashFunction(definition);
        table[index].add({ word, definition });
    }

    // Find the word based on its definition
    std::string find(const std::string& definition) const {
        size_t index = hashFunction(definition);
        for (size_t i = 0; i < table[index].size(); ++i) {
            if (table[index][i].definition == definition) {
                return table[index][i].word;
            }
        }
        return "Word not found.";
    }
};

// Function to load the dataset into the hash table
HashTable loadDataset(const std::string& filename, size_t tableSize) {
    HashTable dictionary(tableSize);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return dictionary;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string word, speech, definition;
        if (std::getline(stream, word, ',') &&std::getline(stream,speech,',')&& std::getline(stream, definition)) {
            dictionary.insert(word, definition);
        }
    }
    file.close();
    return dictionary;
}

int main() {
    const size_t tableSize = 100;  // Set the size of the hash table
    // Load the dataset
    HashTable dictionary = loadDataset("englishDictionary2.csv", tableSize);

    // Ask the user for a definition to search
    std::string definition;
    std::cout << "Enter the definition to search for: ";
    std::getline(std::cin, definition);

    // Find and display the corresponding word
    std::string word = dictionary.find(definition);
    std::cout << "The word for the given definition is: " << word << std::endl;

    return 0;
}
