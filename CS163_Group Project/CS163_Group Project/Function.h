#pragma once
#include <vector>
#include <string>

using namespace std;
struct TrieEng {
	vector<TrieEng*>  children;
	bool isEnd = false;
	vector<wstring> definition;
	int numChildren = 0;
	TrieEng() : children(38, nullptr) {}
};


bool loadRawData(TrieEng*& root, string path);
bool findWordMeaning(TrieEng* root, string word, vector<wstring>& meaning);