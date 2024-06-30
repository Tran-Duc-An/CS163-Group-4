#pragma once
#include <vector>
#include <string>

using namespace std;
struct TrieEng {
	vector<TrieEng*>  children;
	bool isEnd = false;
	vector<wstring> definition;
	short int numChildren = 0;
	TrieEng() : children(38, nullptr) {}
};

void deleteTrieEng(TrieEng* root);
bool loadRawData(TrieEng*& root, string path);
void saveTrietoFile(TrieEng* root, string path);
bool findWordMeaning(TrieEng* root, string word, vector<wstring>& meaning);
bool loadTriefromFile(TrieEng*& root, string path);