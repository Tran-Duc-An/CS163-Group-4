#pragma once
#include <vector>
#include <string>
#include <list>
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

struct HashTable {
	vector<vector<pair<string, string>>> table;
	size_t size;
	int currentLoad = 0;
};

namespace EV {
	EVTrie* findWord(EVTrie* root, string word);
	void deleteTrie(EVTrie* root);
	void insertWord(EVTrie*& root, string& word, wstring& definition);
	bool loadRawData(EVTrie*& root, string path);
	void saveTrietoFile(EVTrie* root, string path);
	bool findWordMeaning(EVTrie* root, string word, vector<wstring>& meaning, EVTrie*& node);
	bool loadTriefromFile(EVTrie*& root, string path);
	void helperDeleteAWord(EVTrie* root, string& word);
	bool deleteAWord(EVTrie* root, string& word);
	void getWordByIndex(EVTrie* curNode, int& index, string& currentWord, string& resultWord, wstring& resultDefinition);
	void randomAWordAnd4Definitions(EVTrie* root, string& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3);
	void randomADefinitionAnd4Words(EVTrie* root, wstring& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3);
	void unLikeAWord(list<string>& favWords, list<wstring>& favDefs, string word, wstring Def);
	void saveFavWord(list<string>& favWords, list<wstring>& favDefs, string filename);
	void loadFavWord(EVTrie* root, list<string>& favWords, list<wstring>& def, string filename);

}

namespace EE {
	void insertWord(EETrie*& root, string& word, string& definition);
	EETrie* findWord(EETrie* root, string& word);
	bool changeWordDefinition(EETrie* root, string& word, string& newDefinition, int indexOfOldDefinitionToBeReplaced);
	void helperDeleteAWord(EETrie* root, string& word);
	bool deleteAWord(EETrie* root, string& word);
	bool findWordMeaning(EETrie* root, string word, vector<string>& meaning, EETrie*& node);
	bool loadRawData(EETrie*& root,string path);
	bool loadTrieFromFile(EETrie*& root, string path);
	void saveTrietoFile(EETrie* root, string path);
	void deleteTrie(EETrie*& root);
	void getWordByIndex(EETrie* curNode, int& index, string& currentWord, string& resultWord, string& resultDefinition);
	void randomAWordAnd4Definitions(EETrie* root, string& rightWord, string& rightDefinition, string& wrongDefinition1, string& wrongDefinition2, string& wrongDefinition3);
	void randomADefinitionAnd4Words(EETrie* root, string& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3);


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
	void unLikeAWord(list<wstring>& favWords, list<wstring>& favDefs, wstring word, wstring Def);
	void saveFavWord(list<wstring>& favWords, list<wstring>& favDefs, string filename);
	void loadFavWord(VTrie* root, list<wstring>& favWords, list<wstring>& def, string filename);
}

namespace Def {
	void insertWord(HashTable& ht, const string& word, const string& definition);
	string findWordMeaning(HashTable& ht, const string& definition);
	bool loadRawData(HashTable& dictionary, size_t tableSize, const string& filename);
	void saveHashtable(HashTable& ht, string filename);
	bool loadHashTable(HashTable& ht, const string& filename);
}

void handleWString(wstring& s, int row, int maxRows);
void fillMap();
void addToHistory(wstring word, wstring def, string fileName);

