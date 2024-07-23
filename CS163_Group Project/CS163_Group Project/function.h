#pragma once
#include "function.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include<iomanip>
using namespace std;

//struct HashTable {
//    vector<vector<pair<string, string>>> table;
//    size_t size;
//    int currentLoad = 0;
//};
//struct Combination {
//    string word;
//    string definition;
//};
//void History(string word, string definition, string filename);
//vector<Combination> ReadHistory(string filename);
string toLowerCase(string& str);
bool checkSubstring(const std::string& s, const std::string& x);
void loadDataset(vector<pair<string, string>>& table);
vector<string> searchByDef(vector<pair<string, string>>& table, string def);