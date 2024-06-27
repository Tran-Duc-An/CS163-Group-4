#pragma once
#include <vector>
#include <string>

using namespace std;
struct TrieEng {
	vector<TrieEng*>  children;
	bool isEnd = false;
	vector<wstring> definition;
	TrieEng() : children(26, nullptr) {}
};


void createTrieEngFromFile(TrieEng*& root, string path);
bool findWordMeaning(TrieEng* root, string word, vector<wstring>& meaning);