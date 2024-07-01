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

string toLowerCase(string&str);
string removeSpecialCharacters(string&str);
void insertWord(trie*& root, string& word, string& definition);
bool findWordMeaning(trie* root, string& word, vector<string>& meaning);
bool loadRawData(trie*& root);
void loadTrie(trie*& root, ifstream& fin);
void saveTrie(trie* root, ofstream& fout);
void deleteTrie(trie*& root);
wstring VToLower(wstring&str);
void fillMap();
void VInsertWord(Vtrie*& root, wstring& word,wstring& definition);
bool VFindWordMeaning(Vtrie* root, wstring& word, vector<wstring>& meaning);
void VDeleteTrie(Vtrie*& root);
bool VloadRawData(Vtrie*& root);
void saveVtrie(Vtrie* root, wofstream& fout);
void loadVtrie(Vtrie*& root, wifstream& fin);
