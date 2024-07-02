#include "function.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct HashTable {
    vector<vector<pair<string, string>>> table;
    size_t size;
    int currentLoad = 0;
};
// Function to add word-definition pair to a vector
void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd) {
    da.push_back(wd);
}

// Initialize the hash table
void initHashTable(HashTable& ht, size_t tableSize) {
    ht.table.resize(tableSize);
    ht.size = tableSize;
}

// Hash function
size_t hashFunction(const string& key, size_t tableSize) {
    const size_t FNV_PRIME = 0x00000000811C9DC5;
    size_t hash = 0;
    for (char ch : key) {
        hash = hash * FNV_PRIME ^ ch;
    }
    return hash % tableSize;
}

// Insert word and definition into the hash table
void insertWordDefinition(HashTable& ht, const string& word, const string& definition) {
    size_t index = hashFunction(definition, ht.size);
    if (ht.table[index].size() == 0) ht.currentLoad += 1;
    addWordDefinition(ht.table[index], { word, definition });
}

// Find the word based on its definition
string findWord( HashTable& ht, const string& definition) {
    size_t index = hashFunction(definition, ht.size);
    for (const auto& pair : ht.table[index]) {
        if (pair.second == definition) {
            return pair.first;
        }
    }
    return "Word not found.";
}

// Load the dataset into the hash table
HashTable loadDataset(const string& filename, size_t tableSize) {
    HashTable dictionary;
    initHashTable(dictionary, tableSize);
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return dictionary;
    }

    string line;
    while (getline(file, line)) {
        istringstream stream(line);
        string word, speech, definition;
        if (getline(stream, word, ',') && getline(stream, speech, ',') && getline(stream, definition)) {
            insertWordDefinition(dictionary, word, definition);
        }
    }
    file.close();
    return dictionary;
}

void saveHashtable (HashTable& ht)
{
    fstream o;
    o.open("Dataset//Hashtable.bin", ios::binary | ios::out);

    if (!o.is_open())
    {
        cout << "Cannot open file";
        return;
    }
    o.write((char*)&ht.size, sizeof(ht.size));
    o.write((char*)&ht.currentLoad, sizeof(int));

    for (int i = 0; i < ht.size; i++)
    {
        int row_size = ht.table[i].size();
        if (row_size != 0)
        {
            o.write((char*)&i, sizeof(i));
            o.write((char*)&row_size, sizeof(int));
            for (int j = 0; j < row_size; j++)
            {
                int length = 0;
                length = ht.table[i][j].first.length();
                o.write((char*)&length, sizeof(length));
                o.write((char*)ht.table[i][j].first.c_str(), length);

                length = ht.table[i][j].second.length();
                o.write((char*)&length, sizeof(length));
                o.write((char*)ht.table[i][j].second.c_str(), length);
            }
        }
    }
}
HashTable loadHashTable(const string& filename) {
    HashTable ht;

    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return ht; // Return empty hash table on error
    }

    // Read table size and current load
    in.read((char*)&ht.size, sizeof(ht.size));
    in.read((char*)&ht.currentLoad, sizeof(int));

    // Resize the hash table based on the loaded size
    ht.table.resize(ht.size);

    for (int i = 0; i < ht.currentLoad; i++)
    {
        int index = 0,row_size=0;
        in.read((char*)&index, sizeof(int));
        in.read((char*)&row_size, sizeof(int));
        
        for (int j = 0; j < row_size; j++)
        {
            int length = 0;
            in.read((char*)&length, sizeof(int));
            char* word=new char[length+1];
            in.read(word, length);
            word[length] = '\0';

            in.read((char*)&length, sizeof(int));
            char* def = new char[length + 1];
            in.read(def, length);
            def[length] = '\0';

            ht.table[index].push_back({ word,def });
            delete[]word;
            delete[]def; 
        }
        
    }
    in.close();
    return ht;
}
int main() {
    const size_t tableSize = 101;

    //// Load the dataset
    //auto start = chrono::high_resolution_clock::now();
    //HashTable dictionary = loadDataset("Dataset/englishDictionary2.csv", tableSize);
    //auto end = chrono::high_resolution_clock::now();
    //chrono::duration<double> duration = end - start;
    //cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;
    //
    /*saveHashtable(dictionary);*/

    HashTable newDict;

    // Load the dataset
    auto start = chrono::high_resolution_clock::now();
    newDict = loadHashTable("Dataset//Hashtable.bin");
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;


    // Ask the user for a definition to search
    string definition;
    cout << "Enter the definition to search for: ";
    getline(cin, definition);

    // find and display the corresponding word
    start = chrono::high_resolution_clock::now();
    string word = findWord(newDict, definition);
    end = chrono::high_resolution_clock::now();
    cout << "the word for the given definition is: " << word << endl;
    duration = end - start;
    cout << "time taken to search dataset: " << duration.count() << " seconds" << endl;


    return 0;
}
