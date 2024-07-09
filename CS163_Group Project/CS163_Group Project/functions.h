#pragma once
#include <vector>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <random>
using namespace std;

// Global functions
bool continueProgram(int type);
string toLowerCase(string& str);
string removeSpecialCharacters(string& str);
void preProcessing(string& word);

// Trie structure
struct EVTrie 
{
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

// Trie functions
namespace EV 
{
	bool loadRawData(EVTrie*& root, string path);
	void inputTrie(EVTrie*& root, wifstream& fin);
	bool loadTrieFromFile(EVTrie*& root, string path);
	void outputTrie(EVTrie* root, wofstream& fou);
	void saveTrieToFile(EVTrie* root, string path);
	void insertWord(EVTrie*& root, string& word, wstring& definition);
	EVTrie* findWord(EVTrie* root, string& word);
	bool findWordMeaning(EVTrie* root, string word, vector<wstring>& meaning, EVTrie*& node);
	void getWordByIndex(EVTrie* curNode, int& index, string& currentWord, string& resultWord, wstring& resultDefinition);
	void randomAWordAnd4Definitions(EVTrie* root, string& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3);
	void randomADefinitionAnd4Words(EVTrie* root, wstring& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3);
	void helperDeleteAWord(EVTrie* root, string& word);
	bool deleteAWord(EVTrie* root, string& word);
	void deleteTrie(EVTrie* root);
}
namespace EE 
{
	bool loadRawData(EETrie*& root, string path);
	void loadTrie(EETrie*& root, ifstream& fin);
	bool loadTrieFromFile(EETrie*& root, string path);
	void saveTrie(EETrie* root, ofstream& fout);
	void saveTrietoFile(EETrie* root, string path);
	void insertWord(EETrie*& root, string& word, string& definition);
	EETrie* findWord(EETrie* root, string& word);
	bool findWordMeaning(EETrie* root, string word, vector<string>& meaning);
	bool changeWordDefinition(EETrie* root, string& word, string& newDefinition, int indexOfOldDefinitionToBeReplaced);
	void getWordByIndex(EETrie* curNode, int& index, string& currentWord, string& resultWord, string& resultDefinition);
	void randomAWordAnd4Definitions(EETrie* root, string& rightWord, string& rightDefinition, string& wrongDefinition1, string& wrongDefinition2, string& wrongDefinition3);
	void randomADefinitionAnd4Words(EETrie* root, string& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3);
	void helperDeleteAWord(EETrie* root, string& word);
	bool deleteAWord(EETrie* root, string& word);
	void deleteTrie(EETrie*& root);
}
namespace VE 
{
	void fillMap();
	wstring VToLower(wstring& str);
	bool loadRawData(VTrie*& root, string path);
	void loadTrie(VTrie*& root, wifstream& fin);
	bool loadTrieFromFile(VTrie*& root, string path);
	void saveTrie(VTrie* root, wofstream& fout);
	void saveTrieToFile(VTrie* root, string path);
	void insertWord(VTrie*& root, wstring& word, wstring& definition);
	VTrie* findWord(VTrie* root, wstring& word);
	bool findWordMeaning(VTrie* root, wstring& word, vector<wstring>& meaning, VTrie*& node);
	bool changeWordDefinition(VTrie* root, wstring& word, wstring& newDefinition, int indexOfOldDefinitionToBeReplaced);
	void getWordByIndex(VTrie* curNode, int& index, wstring& currentWord, wstring& resultWord, wstring& resultDefinition);
	void randomAWordAnd4Definitions(VTrie* root, wstring& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3);
	void randomADefinitionAnd4Words(VTrie* root, wstring& rightDefinition, wstring& rightWord, wstring& wrongWord1, wstring& wrongWord2, wstring& wrongWord3);
	void helperDeleteAWord(VTrie* root, wstring& word);
	bool deleteAWord(VTrie* root, wstring& word);
	void deleteTrie(VTrie*& root);
}
