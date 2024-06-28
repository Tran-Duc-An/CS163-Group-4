#pragma once
#include <iostream>
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
	int numChildren = 0;
	trie() : children(38, nullptr) {}
};
string toLowerCase(string& str);
string removeSpecialCharacters(string& str);
void insertWord(trie*& root, string& word, string& definition);
bool findWordMeaning(trie* root, string& word, vector<string>& meaning);
bool loadRawData(trie*& root);
void loadTrie(trie*& root, ifstream& fin);
void saveTrie(trie* root, ofstream& fout);
void deleteTrie(trie*& root);