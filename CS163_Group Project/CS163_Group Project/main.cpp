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
#include <chrono> // Include the chrono library
// Structure to hold word and definition pairs
struct WordDefinition {
    std::string word;
    std::string definition;
};

// Dynamic array struct for chaining
struct DynamicArray {
    WordDefinition* array;
    size_t capacity;
    size_t count;
};

// Initialize the dynamic array
void initDynamicArray(DynamicArray& da, size_t initialCapacity = 1024) {
    da.array = new WordDefinition[initialCapacity];
    da.capacity = initialCapacity;
    da.count = 0;
}

// Resize the dynamic array when it's full
void resizeDynamicArray(DynamicArray& da) {
    da.capacity *= 2;
    WordDefinition* newArray = new WordDefinition[da.capacity];
    for (size_t i = 0; i < da.count; ++i) {
        newArray[i] = da.array[i];
    }
    delete[] da.array;
    da.array = newArray;
}

// Add an element to the dynamic array
void addWordDefinition(DynamicArray& da, const WordDefinition& wd) {
    if (da.count == da.capacity) {
        resizeDynamicArray(da);
    }
    da.array[da.count++] = wd;
}

// Free the dynamic array
void freeDynamicArray(DynamicArray& da) {
    delete[] da.array;
}

// Hash table struct with chaining using dynamic arrays
struct HashTable {
    DynamicArray* table;
    size_t size;
};

// Initialize the hash table
void initHashTable(HashTable& ht, size_t tableSize) {
    ht.table = new DynamicArray[tableSize];
    ht.size = tableSize;
    for (size_t i = 0; i < tableSize; ++i) {
        initDynamicArray(ht.table[i]);
    }
}

// Free the hash table
void freeHashTable(HashTable& ht) {
    for (size_t i = 0; i < ht.size; ++i) {
        freeDynamicArray(ht.table[i]);
    }
    delete[] ht.table;
}

// Hash function
size_t hashFunction(const std::string& key, size_t tableSize) {
    const size_t FNV_PRIME = 0x00000000811C9DC5;
    size_t hash = 0; // Initialize hash to 0
    for (char ch : key) {
        hash = hash * FNV_PRIME ^ ch; // Corrected line
    }
    return hash % tableSize;
}


// Insert word and definition into the hash table
//This function will be included into function insertWord
void insertWordDefinition(HashTable& ht, const std::string& word, const std::string& definition) {
    size_t index = hashFunction(definition, ht.size);
    addWordDefinition(ht.table[index], { word, definition });
}

// Find the word based on its definition
std::string findWord(const HashTable& ht, const std::string& definition) {
    size_t index = hashFunction(definition, ht.size);
    for (size_t i = 0; i < ht.table[index].count; ++i) {
        if (ht.table[index].array[i].definition == definition) {
            return ht.table[index].array[i].word;
        }
    }
    return "Word not found.";
}

// Load the dataset into the hash table
// Function to read a chunk of data from the file
bool readChunk(std::ifstream& file, std::vector<std::string>& lines, size_t bufferSize) {
    lines.clear();
    char *buffer= new char[bufferSize];  // Create a buffer of characters

    // Read data into the buffer until the buffer is full or the end of file is reached
    while (file.read(buffer, bufferSize)) {
        size_t bytesRead = file.gcount();

        // Process the read data (might involve splitting into lines)
        std::string chunk(buffer, bytesRead);
        std::istringstream stream(chunk);
        std::string line;
        while (std::getline(stream, line, '\n')) {
            lines.push_back(line);
        }

        // Check if the end of file was reached before filling the buffer
        if (bytesRead < bufferSize) {
            break;
        }
    }

    return !file.eof();
}


HashTable loadDataset(const std::string& filename, size_t tableSize) {
    HashTable dictionary;
    initHashTable(dictionary, tableSize);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return dictionary;
    }

    const size_t chunkSize = 100000; // Adjust chunk size as needed
    std::vector<std::string> lines;
    while (readChunk(file, lines, chunkSize)) {
        for (const std::string& line : lines) {
            // Process each line (extract word and definition)
            std::istringstream stream(line);
            std::string word, speech, definition;
            if (std::getline(stream, word, ',') && std::getline(stream, speech, ',') && std::getline(stream, definition)) {
                insertWordDefinition(dictionary, word, definition);
            }
        }
    }
    file.close();
    return dictionary;
}

const size_t tableSize = 100;  // Set the size of the hash table
int main() {
   
    // Load the dataset
        auto start = std::chrono::high_resolution_clock::now(); // Start time
    HashTable dictionary = loadDataset("Dataset/englishDictionary2.csv", tableSize);
    auto end = std::chrono::high_resolution_clock::now();   // End time

     //Calculate the duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken to load dataset: " << duration.count() << " seconds" << std::endl;

    // Ask the user for a definition to search
    std::string definition;
    std::cout << "Enter the definition to search for: ";
    std::getline(std::cin, definition);

    // Find and display the corresponding word
     start = std::chrono::high_resolution_clock::now(); // Start time
    std::string word = findWord(dictionary, definition);
    end = std::chrono::high_resolution_clock::now();   // End time
    std::cout << "The word for the given definition is: " << word << std::endl;
    duration = end - start;
    std::cout << "Time taken to search dataset: " << duration.count() << " seconds" << std::endl;


    // Free the hash table
    freeHashTable(dictionary);

    return 0;
}
