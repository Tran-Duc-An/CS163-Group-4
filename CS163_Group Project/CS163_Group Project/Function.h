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

struct Emo
{
	vector<vector<pair<string, string>>> Emo_table; //name,utf8
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
	void unLikeAWord(vector<string>& favWords, vector<wstring>& favDefs, string word, wstring Def);
	void saveFavWord(vector<string>& favWords, vector<wstring>& favDefs, string filename);
	void loadFavWord(EVTrie* root, vector<string>& favWords, vector<wstring>& def, string filename);

}

namespace EE {
	void insertWord(EETrie*& root, string& word, string& definition);
	EETrie* findWord(EETrie* root, string word);
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
	void unLikeAWord(vector<string>& favWords, vector<string>& favDefs, string word, string Def);
	void saveFavWord(vector<string>& favWords, vector<string>& favDefs, string filename);
	void loadFavWord(EETrie* root, vector<string>& favWords, vector<string>& def, string filename);

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
	void unLikeAWord(vector<wstring>& favWords, vector<wstring>& favDefs, wstring word, wstring Def);
	void saveFavWord(vector<wstring>& favWords, vector<wstring>& favDefs, string filename);
	void loadFavWord(VTrie* root, vector<wstring>& favWords, vector<wstring>& def, string filename);
	void getWordByIndex(VTrie* curNode, int& index, wstring& currentWord, wstring& resultWord, wstring& resultDefinition);
	void randomAWordAnd4Definitions(VTrie* root, wstring& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3);
	void randomADefinitionAnd4Words(VTrie* root, wstring& rightDefinition, wstring& rightWord, wstring& wrongWord1, wstring& wrongWord2, wstring& wrongWord3);

}

namespace Def {
	void loadDataset(vector<pair<string, string>>& table, string filename);
	vector<string> searchByDef(vector<pair<string, string>>& table, string def);
}

namespace Emoji {
	void insertEmo(Emo& ht, const string& name, const string& utf8);

	// Find the word based on its definition
	pair<string, string> findBycode(Emo& ht, string utf8);
	string findByname(Emo& ht, const string& name, int& index, int& inside);
	vector<string> findbyNameUntil(Emo& ht, const string& name);
	// Load the dataset into the hash table
	Emo loadDataset(const string& filename, size_t tableSize);
}

void handleWString(wstring& s, int row, int maxRows);
void fillMap();
void addToHistory(wstring word, wstring def, string fileName);
void loadSearchHistory(vector<wstring>& info, vector<wstring>& time, string filename);
bool resetToOriginal(bool EE, bool EV, bool VE, EETrie*& rootEtoE, EVTrie*& rootEtoV, VTrie*& rootVtoE);

