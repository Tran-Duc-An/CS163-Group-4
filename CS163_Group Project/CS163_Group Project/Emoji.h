#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <Windows.h>
#include<chrono>
using namespace std;


struct Emo
{
    vector<vector<pair<string, string>>> Emo_table; //name,emoji
    size_t size;
    int currentLoad = 0;
};
namespace Emoji {
	void insertEmo(Emo& ht, const string& name, const string& utf8);

	// Find the word based on its definition
	pair<string, string> findBycode(Emo& ht, string utf8);
	pair<string, string> findByname(Emo& ht, const string& name, int& index, int& inside);
	vector<pair<string, string>> findbyNameUntil(Emo& ht, const string& name);
	// Load the dataset into the hash table
	Emo loadDataset(const string& filename, size_t tableSize);
}
bool checkSubstring(const std::string& s, const std::string& x);