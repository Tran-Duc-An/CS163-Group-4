#pragma once
#include <vector>
#include <string>

using namespace std;
struct EVTrie {
	vector<EVTrie*>  children;
	bool isEnd = false;
	vector<wstring> definition;
	short int numChildren = 0;
	EVTrie() : children(38, nullptr) {}
};
struct EETrie
{
	vector<EETrie*>  children;
	bool isend = false;
	vector<string> definition;
	short int numChildren = 0;
	EETrie() : children(38, nullptr) {}
};

struct VTrie
{
	vector<VTrie*> children;
	vector<wstring> definition;
	short int numChildren = 0;
	// 89 vietnamese characters and 2 for hyphen and space
	VTrie() : children(91, nullptr) {}
};


namespace EV {
	void deleteTrie(EVTrie* root);
	void insertWord(EVTrie*& root, string& word, wstring& definition);
	bool loadRawData(EVTrie*& root, string path);
	void saveTrietoFile(EVTrie* root, string path);
	bool findWordMeaning(EVTrie* root, string word, vector<wstring>& meaning);
	bool loadTriefromFile(EVTrie*& root, string path);
}

namespace EE {
	void insertWord(EETrie*& root, string& word, string& definition);
	bool findWordMeaning(EETrie* root, string word, vector<string>& meaning);
	bool loadRawData(EETrie*& root,string path);
	bool loadTrieFromFile(EETrie*& root, string path);
	void saveTrietoFile(EETrie* root, string path);
	void deleteTrie(EETrie*& root);
}


namespace VE {
	void insertWord(VTrie*& root, wstring& word, wstring& definition);
	bool findWordMeaning(VTrie* root, wstring& word, vector<wstring>& meaning);
	void deleteTrie(VTrie*& root);
	bool loadRawData(VTrie*& root,string path);
	void saveTrieToFile(VTrie* root, string path);
	bool loadTrieFromFile(VTrie*& root, string path);
}