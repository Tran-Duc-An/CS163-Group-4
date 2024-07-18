#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <chrono>
#include <io.h>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <list>

using namespace std;

struct trie
{

	vector<trie*>  children;
	bool isend = false;
	vector<string> definition;
	int numChildren = 0;
	trie() : children(38, nullptr) {}
};

struct Vtrie
{
	vector<Vtrie*> children;
	vector<wstring> definition;
	int numChildren = 0;
	bool isLiked = false;
	// 89 vietnamese characters and 2 for hyphen and space
	Vtrie() : children(91, nullptr) {}
};
string toLowerCase(string& str);
string removeSpecialCharacters(string& str);
void insertWord(trie*& root, string& word, string& definition);
bool findWordMeaning(trie* root, string& word, vector<string>& meaning);
bool loadRawData(trie*& root);
void loadTrie(trie*& root, ifstream& fin);
void saveTrie(trie* root, ofstream& fout);
void deleteTrie(trie*& root);
wstring VToLower(wstring& str);
void fillMap();
void VInsertWord(Vtrie*& root, wstring& word, wstring& definition);
Vtrie* VFindWord(Vtrie* root, wstring& word);
bool VloadRawData(Vtrie*& root);
bool VFindWordMeaning(Vtrie* root, wstring& word, vector<wstring>& meaning);
void VDeleteTrie(Vtrie*& root);
void VEupdatesearchhistory(wstring word, vector<wstring> definition);
void VEloadsearchhistory(vector<wstring>& info, vector<wstring>& time);
bool VloadFavWord(list<wstring>& favWords, list<wstring>& def);
void VlikeAWord(list<wstring>& favWords, list<wstring>& favDefs, wstring word, Vtrie* root);
void VunlikeAWord(list<wstring>& favWords, list<wstring>& favDefs, wstring word, Vtrie* root);
bool VsaveFavWord(list<wstring>& favWords, list<wstring>& favDefs);