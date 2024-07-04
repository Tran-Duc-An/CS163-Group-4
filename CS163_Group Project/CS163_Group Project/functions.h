#pragma once
#include <iostream>
#include <cstdlib>
#include <io.h>
#include <fcntl.h>
#include <clocale>
#include <codecvt>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <chrono>

using namespace std;

struct trie 
{

	vector<trie*>  children;
	bool isend = false;
	vector<string> definition;
	short int numChildren = 0;
	trie() : children(38, nullptr) {}
};

// struct for vietnamese trie
struct Vtrie
{
	vector<Vtrie*> children;
	vector<wstring> definition;
	short int numChildren = 0;
	// 89 vietnamese characters and 2 for hyphen and space
	Vtrie() : children(91, nullptr) {}
};
// EE functions
string toLowerCase(string&str);
string removeSpecialCharacters(string&str);
void insertWord(trie*& root, string& word, string& definition);
trie* findWord(trie* root, string& word);
bool findWordMeaning(trie* root, string& word, vector<string>& meaning);
bool loadRawData(trie*& root);
void loadTrie(trie*& root, ifstream& fin);
void saveTrie(trie* root, ofstream& fout);
void deleteTrie(trie*& root);
void helperDeleteAWord(trie* root, string& word);
bool deleteAWord(trie* root, string& word);
bool changeWordDefinition(trie* root, string& word, string& newDefinition, int indexOfOldDefinitionToBeReplaced);

// VE functions
wstring VToLower(wstring&str);
void fillMap();
void VInsertWord(Vtrie*& root, wstring& word,wstring& definition);
Vtrie* VFindWord(Vtrie* root, wstring& word);
bool VFindWordMeaning(Vtrie* root, wstring& word, vector<wstring>& meaning);
void VDeleteTrie(Vtrie*& root);
bool VloadRawData(Vtrie*& root);
void saveVtrie(Vtrie* root, wofstream& fout);
void loadVtrie(Vtrie*& root, wifstream& fin);
void helperDeleteAWord(Vtrie* root, wstring& word);
bool VDeleteAWord(Vtrie* root, wstring& word);
bool VChangeWordDefinition(Vtrie* root, wstring& word, wstring& newDefinition, int indexOfOldDefinitionToBeReplaced);


