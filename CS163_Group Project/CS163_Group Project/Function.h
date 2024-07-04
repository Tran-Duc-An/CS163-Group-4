#pragma once
#include <vector>
#include <string>

using namespace std;
struct EVTrie {
	vector<EVTrie*>  children;
	bool isEnd = false;
	vector<wstring> definition;
	short int numChildren = 0;
	bool isLiked = 0;
	EVTrie() : children(38, nullptr) {}
};
struct EETrie
{
	vector<EETrie*>  children;
	bool isend = false;
	vector<string> definition;
	short int numChildren = 0;
	bool isLiked = 0;
	EETrie() : children(38, nullptr) {}
};

struct VTrie
{
	vector<VTrie*> children;
	vector<wstring> definition;
	short int numChildren = 0;
	bool isLiked = 0;
	// 89 vietnamese characters and 2 for hyphen and space
	VTrie() : children(91, nullptr) {}
};


namespace EV {
	EVTrie* findWord(EVTrie* root, string& word);
	void deleteTrie(EVTrie* root);
	void insertWord(EVTrie*& root, string& word, wstring& definition);
	bool loadRawData(EVTrie*& root, string path);
	void saveTrietoFile(EVTrie* root, string path);
	bool findWordMeaning(EVTrie* root, string word, vector<wstring>& meaning, EVTrie*& node);
	bool loadTriefromFile(EVTrie*& root, string path);
}

namespace EE {
	void insertWord(EETrie*& root, string& word, string& definition);
	EETrie* findWord(EETrie* root, string& word);
	bool changeWordDefinition(EETrie* root, string& word, string& newDefinition, int indexOfOldDefinitionToBeReplaced);
	void helperDeleteAWord(EETrie* root, string& word);
	bool deleteAWord(EETrie* root, string& word);
	bool findWordMeaning(EETrie* root, string word, vector<string>& meaning);
	bool loadRawData(EETrie*& root,string path);
	bool loadTrieFromFile(EETrie*& root, string path);
	void saveTrietoFile(EETrie* root, string path);
	void deleteTrie(EETrie*& root);
}


namespace VE {
	void insertWord(VTrie*& root, wstring& word, wstring& definition);
	VTrie* findWord(VTrie* root, wstring& word);
	bool findWordMeaning(VTrie* root, wstring& word, vector<wstring>& meaning, VTrie*& node);
	bool changeWordDefinition(VTrie* root, wstring& word, wstring& newDefinition, int indexOfOldDefinitionToBeReplaced);
	void helperDeleteAWord(VTrie* root, wstring& word);
	bool deleteAWord(VTrie* root, wstring& word);
	void deleteTrie(VTrie*& root);
	bool loadRawData(VTrie*& root,string path);
	void saveTrieToFile(VTrie* root, string path);
	bool loadTrieFromFile(VTrie*& root, string path);
}