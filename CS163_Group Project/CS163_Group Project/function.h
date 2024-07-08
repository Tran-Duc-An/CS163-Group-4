#pragma once
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

void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd);

// Initialize the hash table
void initHashTable(HashTable& ht, size_t tableSize);

// Hash function
size_t hashFunction(const string& key, size_t tableSize);

// Insert word and definition into the hash table
void insertWordDefinition(HashTable& ht, const string& word, const string& definition);

// Find the word based on its definition
string findWord(HashTable& ht, const string& definition);

// Load the dataset into the hash table
HashTable loadDataset(const string& filename, size_t tableSize);

void saveHashtable(HashTable& ht);
HashTable loadHashTable(const string& filename);
void History(string word, string definition);