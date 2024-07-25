#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <Windows.h>
#include<chrono>0
using namespace std;


struct Emo
{
    vector<vector<pair<string, string>>> Emo_table; //name,emoji
    size_t size;
    int currentLoad = 0;
};
std::string hexCodePointSequenceToUtf8(const std::string& hexCodePointSequence);
bool checkSubstring(const std::string& s, const std::string& x);
void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd);

// Initialize the hash table
void initHashTable(Emo& ht, size_t tableSize);

// Hash function
size_t hashFunction(const string& key, size_t tableSize);

// Insert word and definition into the hash table
void insertEmo(Emo& ht, const string& name, const string& utf8);

// Find the word based on its definition
pair<string, string> findBycode(Emo& ht, string utf8);
string findByname(Emo& ht, const string& name, int& index, int& inside);
vector<string> findbyNameUntil(Emo& ht, const string& name);
// Load the dataset into the hash table
Emo loadDataset(const string& filename, size_t tableSize);