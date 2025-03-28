﻿#include "functions.h"
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <sstream>
#include <random>
#include <list>
#include <chrono>
#include <thread>
using namespace std;
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter2;


string toLowerCase(string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}
	return str;
}
string removeSpecialCharacters(string& str) {
	string res;
	for (char c : str) {
		if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ' || c == '-')
		{
			res.push_back(c);
		}
	}
	return res;
}
void EV::insertWord(EVTrie*& root, string& word, wstring& definition)
{
	word = toLowerCase(word);
	word = removeSpecialCharacters(word);
	EVTrie* current = root;

	for (char c : word)
	{
		// a to z is 0 to 25
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr)
			{
				current->children[c - 'a'] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[c - 'a'];
		}
		// space is 26
		else if (c == ' ')
		{
			if (current->children[26] == nullptr)
			{
				current->children[26] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[26];
		}
		// hyphen is 27
		else if (c == '-')
		{
			if (current->children[27] == nullptr)
			{
				current->children[27] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[27];
		}
		// 0 to 9 is 28 to 37
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr)
			{
				current->children[c - '0' + 28] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[c - '0' + 28];

		}
	}
	current->isEnd = true;
	current->definition.push_back(definition);

}

EVTrie* EV::findWord(EVTrie* root, string word)
{
	word = toLowerCase(word);
	EVTrie* current = root;
	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return 0;
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return 0;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return 0;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return 0;
			current = current->children[c - '0' + 28];
		}
	}
	if (!current->isEnd) return 0;
	return current;
}

bool EV::findWordMeaning(EVTrie* root, string word, vector<wstring>& meaning, EVTrie*& node)
{
	node = EV::findWord(root, word);
	if (node == 0) return false;
	meaning = node->definition;
	return true;
}
void EV::changeWordDefinition(EVTrie*& node, wstring newDefinition, int indexOfOldDefinitionToBeReplaced) {
	if (node == nullptr) return;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;
}

void EV::deleteTrie(EVTrie* root)
{
	if (root == nullptr) return;
	for (EVTrie* child : root->children)
	{
		EV::deleteTrie(child);
	}
	delete root;
}

void preProcessing(string& word) {
	size_t bracket_pos = word.find('[');
	if (bracket_pos != string::npos)
	{
		word = word.substr(0, bracket_pos - 1);
	}
	size_t bracket_pos_open = word.find('(');
	size_t bracket_pos_close = word.find(')');
	if (bracket_pos_open != string::npos)
	{
		word = word.erase(bracket_pos_open - 1, bracket_pos_close - bracket_pos_open + 2);
	}
	while (!word.empty() && word[0] == ' ') word.erase(word.begin(), word.begin() + 1);
	while (!word.empty() && word[word.length() - 1] == ' ') word.erase(word.length() - 2, word.length() - 1);
}

bool EV::loadRawData(EVTrie*& root, string path)
{

	wifstream fin;
	fin.open(path);

	// Ensure the file is opened with UTF-8 encoding
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));

	if (!fin.is_open())
	{
		return false;
	}

	wstring wword;
	wstring definition;


	while (getline(fin, wword, L',')) {
		if (getline(fin, definition, L'\n'))
		{
			// Convert wstring to string using wstring_convert
			wstring_convert<codecvt_utf8<wchar_t>> converter;
			string word = converter.to_bytes(wword);

			preProcessing(word);
			EV::insertWord(root, word, definition);
		}
	}

	return true;
}

void outputTrie(EVTrie* root, wofstream& fou) {
	if (root == nullptr) return;

	short int numDefinitions = root->definition.size();
	fou.write((wchar_t*)(&numDefinitions), sizeof(short int));
	if (root->definition.size() != 0) {
		for (wstring& str : root->definition) {
			short int len = str.length();
			fou.write((wchar_t*)(&len), sizeof(short int));
			fou.write(str.c_str(), len);
		}
	}
	short int numChildren = root->numChildren;
	fou.write((wchar_t*)(&numChildren), sizeof(short int));
	// save children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < 38; ++i) {
		if (root->children[i] != nullptr) {
			wchar_t c = i < 26 ? (wchar_t)(i + L'a') : (i == 26 ? L' ' : (i == 27 ? L'-' : (i - 28 + L'0')));

			fou.write(&c, sizeof(wchar_t));
			outputTrie(root->children[i], fou);
		}
	}
}
void EV::saveTrietoFile(EVTrie* root, string path) {
	wofstream fou;
	fou.open(path, ios::binary);

	// Ensure the file is opened with UTF-8 encoding
	fou.imbue(locale(fou.getloc(), new codecvt_utf8<wchar_t>));

	outputTrie(root, fou);
	fou.close();
}

void inputTrie(EVTrie*& root, wifstream& fin) {
	short int numDefinitions = 0;
	fin.read((wchar_t*)&numDefinitions, sizeof(short int));
	if (numDefinitions != 0) {
		root->isEnd = true;
		for (int i = 0; i < numDefinitions; ++i)
		{
			short int len;
			fin.read((wchar_t*)&len, sizeof(short int));
			wchar_t* tmp = new wchar_t[len + 1];
			fin.read(tmp, len);
			tmp[len] = '\0';
			root->definition.push_back(tmp);
			delete[] tmp;
		}
	}
	fin.read((wchar_t*)&root->numChildren, sizeof(short int));
	// load children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < root->numChildren; ++i)
	{
		wchar_t c;
		fin.read(&c, sizeof(wchar_t));
		if (c >= L'a' && c <= L'z')
		{
			root->children[c - L'a'] = new EVTrie();
			inputTrie(root->children[c - L'a'], fin);
		}
		else if (c == L' ')
		{
			root->children[26] = new EVTrie();
			inputTrie(root->children[26], fin);
		}
		else if (c == L'-')
		{
			root->children[27] = new EVTrie();
			inputTrie(root->children[27], fin);
		}
		else if (c >= L'0' && c <= L'9')
		{
			root->children[c - L'0' + 28] = new EVTrie();
			inputTrie(root->children[c - L'0' + 28], fin);
		}
	}
}

bool EV::loadTriefromFile(EVTrie*& root, string path) {

	wifstream fin;
	fin.open(path, ios::binary);

	// Ensure the file is opened with UTF-8 encoding
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));

	if (!fin.is_open())
	{
		return false;
	}
	inputTrie(root, fin);
	fin.close();
	return true;
}

void EV::helperDeleteAWord(EVTrie* root, string& word) {

	EVTrie* current = root;
	EVTrie* parent = nullptr;
	int childIndex = -1;

	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return;
			parent = current;
			childIndex = c - 'a';
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return;
			parent = current;
			childIndex = 26;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return;
			parent = current;
			childIndex = 27;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return;
			parent = current;
			childIndex = c - '0' + 28;
			current = current->children[c - '0' + 28];
		}
	}
	if (parent != nullptr)
	{
		delete current;
		parent->children[childIndex] = nullptr;
		parent->numChildren--;
	}
}

bool EV::deleteAWord(EVTrie* root, string& word) {
	EVTrie* node = EV::findWord(root, word);
	if (node == 0) return false;
	// if the node is leaf node, we have to call a helper function to delete it from its parent's children array
	if (node->numChildren == 0)
	{
		void EV::helperDeleteAWord(EVTrie * root, string & word);
	}
	else
	{
		node->definition.clear();
		node->isEnd = false;
	}
	return true;
}

void EV::getWordByIndex(EVTrie* curNode, int& index, string& currentWord, string& resultWord, wstring& resultDefinition)
{
	if (curNode == nullptr)
		return;
	if (curNode->definition.size() != 0)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultDefinition = curNode->definition[0];
			return;
		}
		index--;
	}
	for (int i = 0; i < 38; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			char tempChar = 'a' + i;
			currentWord.push_back(tempChar);
			EV::getWordByIndex(curNode->children[i], index, currentWord, resultWord, resultDefinition);
			currentWord.pop_back();

			if (!resultWord.empty())
				return;
		}
	}
}
void EV::randomAWordAnd4Definitions(EVTrie* root, string& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 100000);
	int randomIndex;
	string currentWord;

	// get right word
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong definition 1
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong definition 2
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EV::randomADefinitionAnd4Words(EVTrie* root, wstring& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 100000);
	int randomIndex;
	string currentWord;

	// get right definition
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong word 1
	randomIndex = dis(gen);
	currentWord = "";
	wstring wrongDefinition1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong word 2
	randomIndex = dis(gen);
	currentWord = "";
	wstring wrongDefinition2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	wstring wrongDefinition3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}

void EV::loadFavWord(EVTrie* root, vector<string>& favWords, vector<wstring>& def, string filename)
{
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin;
	fin.imbue(loc);
	fin.open(filename);
	if (!fin.is_open())
	{
		return;
	}
	while (!fin.eof())
	{
		wstring word = L"";
		getline(fin, word, L',');
		if (word.empty()) break;
		EVTrie* node = EV::findWord(root, converter2.to_bytes(word));
		if (node != nullptr) node->isLiked = 1;
		favWords.push_back(converter2.to_bytes(word));
		getline(fin, word, L'\n');
		def.push_back(word);

	}
	fin.close();
	return;
}

void EV::unLikeAWord(vector<string>& favWords, vector<wstring>& favDefs, string word, wstring Def)
{
	// remove the word from the list of favWords its definition from the list of favDefs
	auto it = find(favWords.begin(), favWords.end(), word);
	if (it != favWords.end()) favWords.erase(it);
	auto it2 = find(favDefs.begin(), favDefs.end(), Def);
	if (it2 != favDefs.end()) favDefs.erase(it2);
}

void EV::saveFavWord(vector<string>& favWords, vector<wstring>& favDefs, string filename)
{
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wofstream fout;
	fout.imbue(loc);
	fout.open(filename);
	if (!fout.is_open())
	{
		return;
	}
	int n = favWords.size();
	for (int i = 0; i < n; ++i)
	{
		fout << converter2.from_bytes(favWords[i]) << L"," << favDefs[i] << L"\n";
	}
	fout.close();
	return;
}

void EV::getWordNodeByIndex(EVTrie* curNode, int& index, string& currentWord, string& resultWord, EVTrie*& resultNode)
{
	if (curNode == nullptr)
		return;
	if (curNode->definition.size() != 0)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultNode = curNode;
			return;
		}
		index--;
	}
	for (int i = 0; i < 38; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			char tempChar = 'a' + i;
			currentWord.push_back(tempChar);
			EV::getWordNodeByIndex(curNode->children[i], index, currentWord, resultWord, resultNode);
			currentWord.pop_back();

			if (resultNode != nullptr)
				return;
		}
	}
}
void EV::randomAWordNode(EVTrie* root, string& resultWord, EVTrie*& resultNode)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 100000);

	string currentWord = "";
	resultWord = "";
	resultNode = nullptr;
	int randomIndex = dis(gen);
	EV::getWordNodeByIndex(root, randomIndex, currentWord, resultWord, resultNode);
}


//English to English Dictionary

void::EE::addExample(EETrie*& root, string& word, string& example)
{
	EETrie* node = findWord(root, word);
	if (node == 0) return;
	node->example = example;
}

string EE::getExample(EETrie* root, string& word)
{
	EETrie* node = findWord(root, word);
	if (node == 0) return "";
	return node->example;

}
bool EE::loadExample(EETrie*& root, const string& fileName)
{
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open())
	{
		cout << "File" << fileName << "not found\n";
		return false;
	}
	string word, example;
	while (getline(fin, word, ','))
	{
		getline(fin, example, '\n');
		addExample(root, word, example);
	}
	return true;
}

void EE::insertWord(EETrie*& root, string& word, string& definition)
{
	word = toLowerCase(word);
	word = removeSpecialCharacters(word);
	EETrie* current = root;
	for (char c : word)
	{
		// a to z is 0 to 25
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr)
			{
				current->children[c - 'a'] = new EETrie;
				current->numChildren++;
			}
			current = current->children[c - 'a'];
		}
		// space is 26
		else if (c == ' ')
		{
			if (current->children[26] == nullptr)
			{
				current->children[26] = new EETrie;
				current->numChildren++;
			}
			current = current->children[26];
		}
		// hyphen is 27
		else if (c == '-')
		{
			if (current->children[27] == nullptr)
			{
				current->children[27] = new EETrie;
				current->numChildren++;
			}
			current = current->children[27];
		}
		// 0 to 9 is 28 to 37
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr)
			{
				current->children[c - '0' + 28] = new EETrie;
				current->numChildren++;
			}
			current = current->children[c - '0' + 28];
		}
	}
	current->isend = true;
	current->definition.push_back(definition);
}

EETrie* EE::findWord(EETrie* root, string word)
{
	word = toLowerCase(word);
	EETrie* current = root;
	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return 0;
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return 0;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return 0;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return 0;
			current = current->children[c - '0' + 28];
		}
	}
	if (!current->isend) return 0;
	return current;
}

bool EE::findWordMeaning(EETrie* root, string word, vector<string>& meaning, EETrie*& node)
{
	node = findWord(root, word);
	if (node == 0) return false;
	meaning = node->definition;
	return true;
}

void EE::changeWordDefinition(EETrie*& node, string newDefinition, int indexOfOldDefinitionToBeReplaced)
{
	if (node == 0) return;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;

}

void EE::helperDeleteAWord(EETrie* root, string& word)
{
	EETrie* current = root;
	EETrie* parent = nullptr;
	int childIndex = -1;

	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return;
			parent = current;
			childIndex = c - 'a';
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return;
			parent = current;
			childIndex = 26;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return;
			parent = current;
			childIndex = 27;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return;
			parent = current;
			childIndex = c - '0' + 28;
			current = current->children[c - '0' + 28];
		}
	}
	if (parent != nullptr)
	{
		delete current;
		parent->children[childIndex] = nullptr;
		parent->numChildren--;
	}
}
bool EE::deleteAWord(EETrie* root, string& word)
{
	EETrie* node = EE::findWord(root, word);
	if (node == 0) return false;
	// if the node is leaf node, we have to call a helper function to delete it from its parent's children array
	if (node->numChildren == 0)
	{
		void helperDeleteAWord(EETrie * root, string & word);
	}
	else
	{
		node->definition.clear();
		node->isend = false;
	}
	return true;
}


bool EE::loadRawData(EETrie*& root, string path)
{
	ifstream fin;
	fin.open(path);
	if (!fin.is_open())
	{
		cout << "File not found\n";
		return false;
	}
	string word, definition, wordType;
	getline(fin, word);
	while (getline(fin, word, ','))
	{
		getline(fin, wordType, ',');
		getline(fin, definition, '\n');
		EE::insertWord(root, word, definition);
	}
	return true;
}

void loadTrie(EETrie*& root, ifstream& fin)
{
	short int numDefinitions;
	fin.read((char*)&numDefinitions, sizeof(short int));
	if (numDefinitions != 0) root->isend = true;
	for (int i = 0; i < numDefinitions; ++i)
	{
		short int len;
		fin.read((char*)&len, sizeof(short int));
		char* tmp = new char[len + 1];
		fin.read(tmp, len);
		tmp[len] = '\0';
		root->definition.push_back(tmp);
		delete[] tmp;
	}
	// load example
	bool hasExample;
	fin.read((char*)&hasExample, sizeof(bool));
	if (hasExample)
	{
		short int len;
		fin.read((char*)&len, sizeof(short int));
		char* tmp = new char[len + 1];
		fin.read(tmp, len);
		tmp[len] = '\0';
		root->example = tmp;
		delete[] tmp;
	}
	fin.read((char*)&root->numChildren, sizeof(short int));
	// load children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < root->numChildren; ++i)
	{
		char c;
		fin.read(&c, sizeof(char));
		if (c >= 'a' && c <= 'z')
		{
			root->children[c - 'a'] = new EETrie();
			loadTrie(root->children[c - 'a'], fin);
		}
		else if (c == ' ')
		{
			root->children[26] = new EETrie();
			loadTrie(root->children[26], fin);
		}
		else if (c == '-')
		{
			root->children[27] = new EETrie();
			loadTrie(root->children[27], fin);
		}
		else if (c >= '0' && c <= '9')
		{
			root->children[c - '0' + 28] = new EETrie();
			loadTrie(root->children[c - '0' + 28], fin);
		}
	}
}
bool EE::loadTrieFromFile(EETrie*& root, string path) {
	ifstream fin;
	fin.open(path, ios::binary);

	if (!fin.is_open())
	{
		return false;
	}
	loadTrie(root, fin);
	fin.close();
	return true;
}

void saveTrie(EETrie* root, ofstream& fout)
{
	short int numDefinitions = root->definition.size();
	fout.write((char*)&numDefinitions, sizeof(short int));
	for (string& str : root->definition)
	{
		short int len = str.length();
		fout.write((char*)&len, sizeof(short int));
		fout.write(str.c_str(), len);
	}
	bool hasExample = root->example.empty() ? false : true;
	fout.write((char*)&hasExample, sizeof(bool));
	if (hasExample)
	{
		short int len = root->example.length();
		fout.write((char*)&len, sizeof(short int));
		fout.write(root->example.c_str(), len);
	}
	short int numChildren = root->numChildren;
	fout.write((char*)&numChildren, sizeof(short int));
	// save children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < 38; ++i)
	{
		if (root->children[i] != nullptr)
		{
			char c = i < 26 ? (char)(i + 'a') : (i == 26 ? ' ' : (i == 27 ? '-' : (i - 28 + '0')));
			fout.write(&c, sizeof(char));
			saveTrie(root->children[i], fout);
		}
	}
}
void EE::saveTrietoFile(EETrie* root, string path) {
	ofstream fou;
	fou.open(path, ios::binary);
	saveTrie(root, fou);
	fou.close();
}

void EE::deleteTrie(EETrie*& root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 38; ++i)
	{
		deleteTrie(root->children[i]);
	}
	delete root;
	root = nullptr;
}

void EE::getWordByIndex(EETrie* curNode, int& index, string& currentWord, string& resultWord, string& resultDefinition)
{
	if (curNode == nullptr)
		return;
	if (curNode->isend)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultDefinition = curNode->definition[0];
			return;
		}
		index--;
	}
	for (int i = 0; i < 38; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			char tempChar = 'a' + i;
			currentWord.push_back(tempChar);
			EE::getWordByIndex(curNode->children[i], index, currentWord, resultWord, resultDefinition);
			currentWord.pop_back();

			if (!resultWord.empty())
				return;
		}
	}
}
void EE::randomAWordAnd4Definitions(EETrie* root, string& rightWord, string& rightDefinition, string& wrongDefinition1, string& wrongDefinition2, string& wrongDefinition3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 110000);
	int randomIndex;
	string currentWord;

	// get right word
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong definition 1
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong definition 2
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EE::randomADefinitionAnd4Words(EETrie* root, string& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 110000);
	int randomIndex;
	string currentWord;

	// get right definition
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong word 1
	randomIndex = dis(gen);
	currentWord = "";
	string wrongDefinition1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong word 2
	randomIndex = dis(gen);
	currentWord = "";
	string wrongDefinition2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	string wrongDefinition3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EE::loadFavWord(EETrie* root, vector<string>& favWords, vector<string>& def, string filename)
{
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open())
	{
		return;
	}
	while (!fin.eof())
	{
		string word;
		getline(fin, word, ',');
		if (word.empty()) break;
		EETrie* node = EE::findWord(root, word);
		if (node != nullptr) node->isLiked = 1;
		favWords.push_back(word);
		getline(fin, word, '\n');
		def.push_back(word);

	}
	fin.close();
	return;
}

void EE::unLikeAWord(vector<string>& favWords, vector<string>& favDefs, string word, string Def)
{
	// remove the word from the list of favWords its definition from the list of favDefs
	auto it = find(favWords.begin(), favWords.end(), word);
	if (it != favWords.end()) favWords.erase(it);
	auto it2 = find(favDefs.begin(), favDefs.end(), Def);
	if (it2 != favDefs.end()) favDefs.erase(it2);
}

void EE::saveFavWord(vector<string>& favWords, vector<string>& favDefs, string filename)
{
	ofstream fout;
	fout.open(filename);
	if (!fout.is_open())
	{
		return;
	}
	int n = favWords.size();
	for (int i = 0; i < n; ++i)
	{
		fout << favWords[i] << "," << favDefs[i] << "\n";
	}
	fout.close();
	return;
}

void EE::getWordNodeByIndex(EETrie* curNode, int& index, string& currentWord, string& resultWord, EETrie*& resultNode)
{
	if (curNode == nullptr)
		return;
	if (curNode->isend)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultNode = curNode;
			return;
		}
		index--;
	}
	for (int i = 0; i < 38; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			char tempChar = 'a' + i;
			currentWord.push_back(tempChar);
			EE::getWordNodeByIndex(curNode->children[i], index, currentWord, resultWord, resultNode);
			currentWord.pop_back();

			if (resultNode != nullptr)
				return;
		}
	}
}
void EE::randomAWordNode(EETrie* root, string& resultWord, EETrie*& resultNode)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 112000);

	string currentWord = "";
	resultWord = "";
	resultNode = nullptr;
	int randomIndex = dis(gen);
	EE::getWordNodeByIndex(root, randomIndex, currentWord, resultWord, resultNode);
}


// Vietnamese to English dictionary
short int map[7930];
short int reverseMap[91];
void fillMap()
{
	memset(map, -1, 7929 * sizeof(short int));
	wstring word = L"a á à ả ã ạ ă ắ ằ ẳ ẵ ặ â ấ ầ ẩ ẫ ậ b c d đ e é è ẻ ẽ ẹ ê ế ề ể ễ ệ g h i í ì ỉ ĩ ị k l m n o ó ò ỏ õ ọ ô ố ồ ổ ỗ ộ ơ ớ ờ ở ỡ ợ p q r s t u ú ù ủ ũ ụ ư ứ ừ ử ữ ự v x y ý ỳ ỷ ỹ ỵ";
	wstring temp = L"";
	for (wchar_t& c : word) if (c != L' ') temp.push_back(c);
	int i = 0;
	for (wchar_t& c : temp)
	{
		map[c] = i;
		reverseMap[i] = c;
		i++;
	}
	map[L'-'] = 89;
	map[L' '] = 90;
	reverseMap[89] = L'-';
	reverseMap[90] = L' ';
}
wstring VToLower(wstring& str)
{
	for (wchar_t& c : str)
	{
		if (c == L'-' || c == L' ' || c > 7929) continue;
		else if (c >= 65 && c <= 89) c += 32;
		else if (map[c] == -1)
		{
			wchar_t temp1 = c + 1;
			wchar_t temp2 = c + 32;
			if (map[temp1] != -1) c = temp1;
			else if (map[temp2] != -1) c = temp2;
		}

	}
	return str;
}

void VE::insertWord(VTrie*& root, wstring& word, wstring& definition)
{
	word = VToLower(word);
	VTrie* current = root;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) continue;
		if (current->children[map[c]] == nullptr)
		{
			current->children[map[c]] = new VTrie();
			current->numChildren++;
		}
		current = current->children[map[c]];
	}
	current->definition.push_back(definition);
}

VTrie* VE::findWord(VTrie* root, wstring& word)
{
	word = VToLower(word);
	VTrie* current = root;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) return nullptr;
		if (current->children[map[c]] == nullptr) return nullptr;
		current = current->children[map[c]];
	}
	if (current->definition.empty()) return nullptr;
	return current;
}

bool VE::findWordMeaning(VTrie* root, wstring& word, vector<wstring>& meaning, VTrie*& node)
{

	node = VE::findWord(root, word);
	if (node == nullptr) return false;
	meaning = node->definition;
	return true;
}

void VE::changeWordDefinition(VTrie*& node, wstring newDefinition, int indexOfOldDefinitionToBeReplaced)
{
	if (node == nullptr) return;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;
}

void VE::helperDeleteAWord(VTrie* root, wstring& word)
{
	VTrie* current = root;
	VTrie* parent = nullptr;
	int childIndex = -1;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) return;
		if (current->children[map[c]] == nullptr) return;
		parent = current;
		childIndex = map[c];
		current = current->children[map[c]];
	}
	if (parent != nullptr)
	{
		delete current;
		parent->children[childIndex] = nullptr;
		parent->numChildren--;
	}
}

bool VE::deleteAWord(VTrie* root, wstring& word)
{
	VTrie* node = VE::findWord(root, word);
	if (node == nullptr) return false;
	if (node->numChildren == 0)
	{
		VE::helperDeleteAWord(root, word);
	}
	else
	{
		node->definition.clear();
	}
	return true;
}

void VE::deleteTrie(VTrie*& root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 91; ++i)
	{
		VE::deleteTrie(root->children[i]);
	}
	delete root;
	root = nullptr;
}

bool VE::loadRawData(VTrie*& root, string path)
{
	fillMap();
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin(path);
	fin.imbue(loc);
	if (!fin.is_open())
	{
		wcout << L"File not found\n";
		return false;
	}
	else
	{
		wstring word, definition, wordType;
		getline(fin, word);
		while (getline(fin, word, L','))
		{
			getline(fin, wordType, L',');
			getline(fin, definition, L'\n');
			VE::insertWord(root, word, definition);
		}
		return true;
	}
}

void saveVtrie(VTrie* root, wofstream& fout)
{
	short int numDefinitions = root->definition.size();
	fout.write((wchar_t*)&numDefinitions, sizeof(short int));
	for (wstring& str : root->definition)
	{
		short int len = str.length();
		// write length of the string
		fout.write((wchar_t*)&len, sizeof(short int));
		// write the string itself
		fout.write((wchar_t*)str.c_str(), len);
	}
	short int numChildren = root->numChildren;
	fout.write((wchar_t*)&numChildren, sizeof(short int));
	for (int i = 0; i < 91; ++i)
	{
		if (root->children[i] != nullptr)
		{
			wchar_t c = reverseMap[i];
			fout.write((wchar_t*)&c, sizeof(wchar_t));
			saveVtrie(root->children[i], fout);
		}
	}
}

void VE::saveTrieToFile(VTrie* root, string path) {
	wofstream fou;
	fou.open(path, ios::binary);
	// Ensure the file is opened with UTF-8 encoding
	fou.imbue(locale(fou.getloc(), new codecvt_utf8<wchar_t>));
	saveVtrie(root, fou);
	fou.close();
}

void loadVtrie(VTrie*& root, wifstream& fin)
{
	short int numDefinitions;
	fin.read((wchar_t*)&numDefinitions, sizeof(short int));
	for (int i = 0; i < numDefinitions; ++i)
	{
		short int len;
		fin.read((wchar_t*)&len, sizeof(short int));
		wchar_t* tmp = new wchar_t[len + 1];
		fin.read((wchar_t*)tmp, len);
		tmp[len] = L'\0';
		root->definition.push_back(tmp);
		delete[] tmp;
	}
	fin.read((wchar_t*)&root->numChildren, sizeof(short int));
	for (int i = 0; i < root->numChildren; ++i)
	{
		wchar_t c;
		fin.read((wchar_t*)&c, sizeof(wchar_t));
		if (map[c] != -1)
		{
			root->children[map[c]] = new VTrie();
			loadVtrie(root->children[map[c]], fin);
		}
	}
}

bool VE::loadTrieFromFile(VTrie*& root, string path) {
	fillMap();
	wifstream fin;
	fin.open(path, ios::binary);
	// Ensure the file is opened with UTF-8 encoding
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
	if (!fin.is_open())
	{
		return false;
	}
	loadVtrie(root, fin);
	fin.close();
	return true;
}

void VE::loadFavWord(VTrie* root, vector<wstring>& favWords, vector<wstring>& def, string filename)
{
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin;
	fin.imbue(loc);
	fin.open(filename);
	if (!fin.is_open())
	{
		return;
	}
	while (!fin.eof())
	{
		wstring word;
		getline(fin, word, L',');
		if (word.empty()) break;

		VTrie* node = VE::findWord(root, word);
		if (node != nullptr) node->isLiked = 1;

		favWords.push_back(word);
		getline(fin, word, L'\n');
		def.push_back(word);

	}
	fin.close();
	return;
}

void VE::unLikeAWord(vector<wstring>& favWords, vector<wstring>& favDefs, wstring word, wstring Def)
{
	// remove the word from the list of favWords its definition from the list of favDefs
	auto it = find(favWords.begin(), favWords.end(), word);
	if (it != favWords.end()) favWords.erase(it);
	it = find(favDefs.begin(), favDefs.end(), Def);
	if (it != favDefs.end()) favDefs.erase(it);
}

void VE::saveFavWord(vector<wstring>& favWords, vector<wstring>& favDefs, string filename)
{
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wofstream fout;
	fout.imbue(loc);
	fout.open(filename);
	if (!fout.is_open())
	{
		return;
	}
	int n = favWords.size();
	for (int i = 0; i < n; ++i)
	{
		fout << favWords[i] << L"," << favDefs[i] << L"\n";
	}
	fout.close();
	return;
}

void VE::getWordByIndex(VTrie* curNode, int& index, wstring& currentWord, wstring& resultWord, wstring& resultDefinition)
{
	if (curNode == nullptr)
		return;
	if (curNode->definition.size() != 0)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultDefinition = curNode->definition[0];
			return;
		}
		index--;
	}
	for (int i = 0; i < 91; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			wchar_t tempChar = reverseMap[i];
			currentWord.push_back(tempChar);
			VE::getWordByIndex(curNode->children[i], index, currentWord, resultWord, resultDefinition);
			currentWord.pop_back();
			if (!resultWord.empty())
				return;
		}
	}
}
void VE::randomAWordAnd4Definitions(VTrie* root, wstring& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 145921);
	int randomIndex;
	wstring currentWord;

	// get right word
	randomIndex = dis(gen);
	currentWord = L"";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong definition 1
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongWord1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong definition 2
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongWord2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongWord3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void VE::randomADefinitionAnd4Words(VTrie* root, wstring& rightDefinition, wstring& rightWord, wstring& wrongWord1, wstring& wrongWord2, wstring& wrongWord3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 140000);
	int randomIndex;
	wstring currentWord;

	// get right definition
	randomIndex = dis(gen);
	currentWord = L"";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong word 1
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongDefinition1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong word 2
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongDefinition2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongDefinition3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}

bool checkSubstring(const std::string& s, const std::string& x) {//KMP x in s
	int m = x.length();
	int n = s.length();

	if (n < m) {
		return false;
	}

	// PREPROCESSING
	// longest_border array
	std::vector<int> longest_border(m, 0);
	// Length of Longest Border for prefix before it.
	int len = 0;
	// Iterating from index-1. longest_border[0] will always be 0
	int i = 1;

	while (i < m) {
		if (x[i] == x[len]) {
			// Length of Longest Border Increased
			len += 1;
			longest_border[i] = len;
			i += 1;
		}
		else {
			// Only empty border exists
			if (len == 0) {
				longest_border[i] = 0;
				i += 1;
			}
			else {
				// Try finding longest border for this i with reduced len
				len = longest_border[len - 1];
			}
		}
	}

	int s_Pointer = 0;
	int x_Pointer = 0;

	while (s_Pointer < n) {
		if (s[s_Pointer] == x[x_Pointer]) {
			// Matched, Increment Both
			x_Pointer += 1;
			s_Pointer += 1;
			// All characters matched
			if (x_Pointer == m) {
				return true;
			}
		}
		else {
			if (x_Pointer == 0) {
				// Zero Matched
				s_Pointer += 1;
			}
			else {
				x_Pointer = longest_border[x_Pointer - 1];
			}
		}
	}

	return false;
}
void Def::loadDataset(vector<pair<string, string>>& table, string filename)
{
	ifstream file(filename);
	if (!file.is_open()) {

		return;
	}
	string line;
	while (getline(file, line)) {
		istringstream stream(line);
		string word, speech, definition;
		if (getline(stream, word, ',') && getline(stream, speech, ',') && getline(stream, definition)) {
			definition = toLowerCase(definition);
			pair<string, string>temp{ word,definition };
			table.push_back(temp);
		}
	}
	file.close();
}
vector<string> Def::searchByDef(vector<pair<string, string>>& table, string def)
{
	vector<string> word;
	def = toLowerCase(def);
	for (int i = 0; i < table.size(); i++)
	{
		if (checkSubstring(table[i].second, def))
			word.push_back(table[i].first);
	}
	return word;
}


std::string hexCodePointSequenceToUtf8(const std::string& hexCodePointSequence) {
	std::istringstream iss(hexCodePointSequence);
	std::string hexCodePoint;
	std::vector<char32_t> codePoints;

	while (iss >> hexCodePoint) {
		char32_t codePoint = std::stoul(hexCodePoint, nullptr, 16);
		codePoints.push_back(codePoint);
	}

	std::u32string u32str(codePoints.begin(), codePoints.end());
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
	return convert.to_bytes(u32str);
}


void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd) {
	da.push_back(wd);
}

// Initialize the hash table
void initHashTable(Emo& ht, size_t tableSize) {
	ht.Emo_table.resize(tableSize);
	ht.size = tableSize;
}

// Hash function
size_t hashFunction(const string& key, size_t tableSize) {
	const size_t FNV_PRIME = 0x00000000811C9DC5;
	size_t hash = 0;
	for (char ch : key) {
		hash = hash * FNV_PRIME ^ ch;
	}
	return hash % tableSize;
}

// Insert word and definition into the hash table
void Emoji::insertEmo(Emo& ht, const string& name, const string& utf8) {
	size_t index = hashFunction(utf8, ht.size);
	if (ht.Emo_table[index].size() == 0) ht.currentLoad += 1;
	addWordDefinition(ht.Emo_table[index], { name, utf8 });
}

// Find the word based on its definition
pair<string, string> Emoji::findBycode(Emo& ht, string utf8) {
	size_t index = hashFunction(utf8, ht.size);
	for (const auto& pair : ht.Emo_table[index]) {
		if (pair.second == utf8) {
			return pair;
		}
	}
	return make_pair("", "");
}
pair<string, string> Emoji::findByname(Emo& ht, const string& name, int& index, int& inside)
{
	int size = ht.Emo_table.size();
	for (; index < size; index++)
	{
		for (; inside < ht.Emo_table[index].size(); inside++) {
			pair<string, string> pair = ht.Emo_table[index][inside];
			if (checkSubstring(pair.first, name))
			{
				inside++;
				return pair;
			}
		}
		inside = 0;
	}
	return { "","" };
}
vector<pair<string, string>> Emoji::findbyNameUntil(Emo& ht, const string& name)
{
	int index = 0;
	int inside = 0;
	int size = ht.Emo_table.size();
	int count = 5;
	vector<pair<string, string>> find;

	while (true)
	{
		if (index >= size)
		{
			return find;
		}

		if (index >= size)
		{
			return find;
		}
		pair<string, string> result = Emoji::findByname(ht, name, index, inside);
		if (!result.first.empty() && !result.second.empty()) {
			find.push_back(result);
		}
		else {
			break;
		}

	}
	return find;
}
// Load the dataset into the hash table
Emo Emoji::loadDataset(const string& filename, size_t tableSize) {
	Emo dictionary;
	initHashTable(dictionary, tableSize);
	locale loc(locale(), new codecvt_utf8<wchar_t>);

	wifstream file(filename);
	file.imbue(loc);
	if (!file.is_open()) {
		cerr << "Error: Could not open file " << filename << endl;
		return dictionary;
	}

	std::wstring line;
	// skip the header line
	std::getline(file, line);
	while (std::getline(file, line)) {
		std::wstringstream ss(line);
		std::wstring No, emoji, codepoint, name;

		std::getline(ss, No, L',');
		std::getline(ss, emoji, L',');
		std::getline(ss, codepoint, L',');
		std::getline(ss, name, L',');

		string name_str(name.begin(), name.end());
		string codepoint_str(codepoint.begin(), codepoint.end());

		// convert the code point to utf-8
		insertEmo(dictionary, name_str, codepoint_str);
	}
	file.close();
	return dictionary;
}
void addToHistory(wstring word, wstring def, string fileName,vector<wstring>& his,vector<wstring>& realTime	)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wofstream fout(fileName, ios::app);
	fout.imbue(loc);
	if (!fout.is_open())
	{
		return;
	}
	else
	{
		time_t now;
		time(&now);
		tm* local = localtime(&now);
		//Print out by this format: word, d/m/yyyy h:m:sAM/PM, definition
		wstring timeStr =
			to_wstring(local->tm_mday) + L"/" +
			to_wstring(local->tm_mon + 1) + L"/" +
			to_wstring(local->tm_year + 1900) + L" " +
			to_wstring((local->tm_hour > 12 ? local->tm_hour - 12 : local->tm_hour)) + L":" +
			(local->tm_min < 10 ? L"0" : L"") + to_wstring(local->tm_min) + L":" +
			(local->tm_sec < 10 ? L"0" : L"") + to_wstring(local->tm_sec) +
			(local->tm_hour >= 12 ? L"PM" : L"AM") + L", ";

		// Add the formatted time string to the history vector
		his.push_back(word + L":" + def);
		realTime.push_back(timeStr);
		fout << word << ',' << local->tm_mday << '/' << local->tm_mon + 1 << '/' << local->tm_year + 1900
			<< ' ' << (local->tm_hour >= 12 ? local->tm_hour - 12 : local->tm_hour) << ':' << local->tm_min << ':'
			<< local->tm_sec << (local->tm_hour >= 12 ? "PM" : "AM") << ',';
		fout << def << endl;
	}
	fout.close();
}


void loadSearchHistory(vector<wstring>& info, vector<wstring>& time, string filename)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin(filename);
	fin.imbue(loc);
	if (!fin.is_open())
	{
		wcout << L"Cannot open search history!!!" << endl;
		return;
	}
	else
	{
		wstring word, t, def;
		while (getline(fin, word, L','))
		{
			getline(fin, t, L',');
			getline(fin, def);
			info.push_back(word + L": " + def);
			time.push_back(t);
		}
	}

	fin.close();
}

bool resetToOriginal(bool EE, bool EV, bool VE, EETrie*& rootEtoE, EVTrie*& rootEtoV, VTrie*& rootVtoE)
{
	auto start = chrono::high_resolution_clock::now();
	// Use threads to delete old trie faster
	std::thread t1, t2, t3, t4, t5, t6;
	if (EE)
	{
		t1 = std::thread(EE::deleteTrie, std::ref(rootEtoE));
		remove("Dataset\\favWordsEE.txt");
	}
	if (EV)
	{
		t2 = std::thread(EV::deleteTrie, std::ref(rootEtoV));
		remove("Dataset\\favWordsEV.txt");
	}
	if (VE)
	{
		t3 = std::thread(VE::deleteTrie, std::ref(rootVtoE));
		remove("Dataset\\favWordsVE.txt");
	}
	remove("Dataset\\History.txt");
	// Wait for all delete threads to finish
	if (EE && t1.joinable())
	{
		t1.join();
	}
	if (EV && t2.joinable())
	{
		t2.join();
	}
	if (VE && t3.joinable())
	{
		t3.join();
	}

	// Use threads to reload new trie faster by function copyAndReload
	if (EV)
	{
		t4 = std::thread(EV::copyAndReload, std::ref(rootEtoV));
	}
	if (VE)
	{
		t5 = std::thread(VE::copyAndReload, std::ref(rootVtoE));
	}
	if (EE)
	{
		t6 = std::thread(EE::copyAndReload, std::ref(rootEtoE));
	}

	// Wait for all reload threads to finish
	if (EV && t4.joinable())
	{
		t4.join();
	}
	if (VE && t5.joinable())
	{
		t5.join();
	}
	if (EE && t6.joinable())
	{
		t6.join();
	}

	auto end = chrono::high_resolution_clock::now();
	wcout << L"Time taken to reset to original: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L" milliseconds" << endl;
	return true;
}

bool EE::copyAndReload(EETrie*& rootEtoE)
{

	rootEtoE = new EETrie();
	// copy data from file "TrieEN.bin" to file "UserTrieEN.bin" by using rdbuf() function
	ifstream fin("Dataset/TrieEN.bin", ios::binary);
	ofstream fout("Dataset/UserTrieEN.bin", ios::binary);
	if (!fin.is_open() || !fout.is_open())
	{
		return false;
	}
	fout << fin.rdbuf();
	fin.close();
	fout.close();
	EE::loadTrieFromFile(rootEtoE, "Dataset/UserTrieEN.bin");
}
bool EV::copyAndReload(EVTrie*& rootEtoV)
{

	rootEtoV = new EVTrie();
	// copy data from file "TrieEN.bin" to file "UserTrieEN.bin" by using rdbuf() function
	ifstream fin("Dataset/TrieENVN.bin", ios::binary);
	ofstream fout("Dataset/UserTrieENVN.bin", ios::binary);
	if (!fin.is_open() || !fout.is_open())
	{
		return false;
	}
	fout << fin.rdbuf();
	fin.close();
	fout.close();
	EV::loadTriefromFile(rootEtoV, "Dataset/UserTrieENVN.bin");
}
bool VE::copyAndReload(VTrie*& rootVtoE)
{

	rootVtoE = new VTrie();
	// copy data from file "TrieVN.bin" to file "UserTrieVN.bin" by using rdbuf() function
	ifstream fin("Dataset/TrieVNEN.bin", ios::binary);
	ofstream fout("Dataset/UserTrieVNEN.bin", ios::binary);
	if (!fin.is_open() || !fout.is_open())
	{
		return false;
	}
	fout << fin.rdbuf();
	fin.close();
	fout.close();
	VE::loadTrieFromFile(rootVtoE, "Dataset/UserTrieVNEN.bin");
}

void VE::getWordNodeByIndex(VTrie* curNode, int& index, wstring& currentWord, wstring& resultWord, VTrie*& resultNode)
{
	if (curNode == nullptr)
		return;
	if (curNode->definition.size() != 0)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultNode = curNode;
			return;
		}
		index--;
	}
	for (int i = 0; i < 91; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			wchar_t tempChar = reverseMap[i];
			currentWord.push_back(tempChar);
			VE::getWordNodeByIndex(curNode->children[i], index, currentWord, resultWord, resultNode);
			currentWord.pop_back();

			if (resultNode != nullptr)
				return;
		}
	}
}

void VE::randomAWordNode(VTrie* root, wstring& resultWord, VTrie*& resultNode)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 140000);

	wstring currentWord = L"";
	resultWord = L"";
	resultNode = nullptr;
	int randomIndex = dis(gen);
	VE::getWordNodeByIndex(root, randomIndex, currentWord, resultWord, resultNode);
}