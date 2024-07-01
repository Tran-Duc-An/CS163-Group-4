#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd);

struct HashTable {
    vector<vector<pair<string, string>>> table;
    size_t size;
};

void initHashTable(HashTable& ht, size_t tableSize);

size_t hashFunction(const string& key, size_t tableSize);

void insertWordDefinition(HashTable& ht, const string& word, const string& definition);

string findWord(const HashTable& ht, const string& definition);

HashTable loadDataset(const string& filename, size_t tableSize);
