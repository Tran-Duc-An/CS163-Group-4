#include "function.h"

//// Function to add word-definition pair to a vector
//void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd) {
//    da.push_back(wd);
//}
//
//// Initialize the hash table
//void initHashTable(HashTable& ht, size_t tableSize) {
//    ht.table.resize(tableSize);
//    ht.size = tableSize;
//}
//
//// Hash function
//size_t hashFunction(const string& key, size_t tableSize) {
//    const size_t FNV_PRIME = 0x00000000811C9DC5;
//    size_t hash = 0;
//    for (char ch : key) {
//        hash = hash * FNV_PRIME ^ ch;
//    }
//    return hash % tableSize;
//}
//
//// Insert word and definition into the hash table
//void insertWordDefinition(HashTable& ht, const string& word, const string& definition) {
//    size_t index = hashFunction(definition, ht.size);
//    addWordDefinition(ht.table[index], { word, definition });
//}
//
//// Find the word based on its definition
//string findWord(const HashTable& ht, const string& definition) {
//    size_t index = hashFunction(definition, ht.size);
//    for (const auto& pair : ht.table[index]) {
//        if (pair.second == definition) {
//            return pair.first;
//        }
//    }
//    return "Word not found.";
//}
//
//// Load the dataset into the hash table
//HashTable loadDataset(const string& filename, size_t tableSize) {
//    HashTable dictionary;
//    initHashTable(dictionary, tableSize);
//    ifstream file(filename);
//    if (!file.is_open()) {
//        cerr << "Error: Could not open file " << filename << endl;
//        return dictionary;
//    }
//
//    string line;
//    while (getline(file, line)) {
//        istringstream stream(line);
//        string word, speech, definition;
//        if (getline(stream, word, ',') && getline(stream, speech, ',') && getline(stream, definition)) {
//            insertWordDefinition(dictionary, word, definition);
//        }
//    }
//    file.close();
//    return dictionary;
//}