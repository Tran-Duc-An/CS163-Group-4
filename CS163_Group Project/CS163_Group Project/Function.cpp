#include "Function.h"
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
using namespace std;
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
void insertWord(TrieEng*& root, string& word, wstring& definition)
{
	word = toLowerCase(word);
	word = removeSpecialCharacters(word);
	TrieEng* current = root;
	int cnt = 0;
	for (char c : word)
	{
		// a to z is 0 to 25
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr)
			{
				current->children[c - 'a'] = new TrieEng;
				current->numChildren++;
			}
			current = current->children[c - 'a'];
			cnt++;
		}
		// space is 26
		else if (c == ' ')
		{
			if (current->children[26] == nullptr)
			{
				current->children[26] = new TrieEng;
				current->numChildren++;
			}
			current = current->children[26];
			cnt++;
		}
		// hyphen is 27
		else if (c == '-')
		{
			if (current->children[27] == nullptr)
			{
				current->children[27] = new TrieEng;
				current->numChildren++;
			}
			current = current->children[27];
			cnt++;
		}
		// 0 to 9 is 28 to 37
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr)
			{
				current->children[c - '0' + 28] = new TrieEng;
				current->numChildren++;
			}
			current = current->children[c - '0' + 28];
			cnt++;
		}
	}
	current->isEnd = true;
	current->definition.push_back(definition);
	current->numChildren = cnt;
}

bool findWordMeaning(TrieEng* root, string word, vector<wstring>& meaning)
{
	word = toLowerCase(word);
	TrieEng* current = root;
	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return false;
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return false;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return false;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return false;
			current = current->children[c - '0' + 28];
		}
	}
	if (!current->isEnd) return false;
	meaning = current->definition;
	return true;
}
void deleteTrieEng(TrieEng* root)
{
	if (root == nullptr) return;
	for (TrieEng* child : root->children)
	{
		deleteTrieEng(child);
	}
	delete root;
}

bool loadRawData(TrieEng*& root,string path)
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


	while (getline(fin, wword, L',')){
		if (getline(fin, definition, L'\n'))
		{
			// Convert wstring to string using wstring_convert
			wstring_convert<codecvt_utf8<wchar_t>> converter;
			string word = converter.to_bytes(wword);

			size_t bracket_pos = word.find('[');
			if (bracket_pos != string::npos)
			{
				word = word.substr(0, bracket_pos - 1);
			}

			insertWord(root, word, definition);
		}
	}

	return true;
}

void outputTrie(TrieEng* root, wofstream& fou) {
	if (root == nullptr) return;

	short int numDefinitions = root->definition.size();
	fou.write(reinterpret_cast<const wchar_t*>(&numDefinitions), sizeof(short int));

	for (wstring& str : root->definition) {
		short int len = str.length();
		fou.write(reinterpret_cast<const wchar_t*>(&len), sizeof(short int));
		fou.write(str.c_str(), len);
	}
	short int numChildren = root->numChildren;
	fou.write(reinterpret_cast<const wchar_t*>(&numChildren), sizeof(short int));
	// save children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < 38; ++i) {
		if (root->children[i] != nullptr) {
			wchar_t c = i < 26 ? (wchar_t)(i + L'a') : (i == 26 ? L' ' : (i == 27 ? L'-' : (i - 28 + L'0')));

			fou.write(&c, sizeof(wchar_t));
			outputTrie(root->children[i], fou);
		}
	}
}
void saveTrietoFile(TrieEng* root, string path) {
	wofstream fou;
	fou.open(path,ios::binary);

	// Ensure the file is opened with UTF-8 encoding
	fou.imbue(locale(fou.getloc(), new codecvt_utf8<wchar_t>));

	outputTrie(root, fou);
	fou.close();
}

void inputTrie(TrieEng*& root, wifstream& fin) {

	short int numDefinitions;
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
		return;
	}
	fin.read((wchar_t*)&root->numChildren, sizeof(short int));
	// load children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < root->numChildren; ++i)
	{
		wchar_t c;
		fin.read(&c, sizeof(wchar_t));
		wcout << c << endl;
		if (c >= 'a' && c <= 'z')
		{
			root->children[c - 'a'] = new TrieEng();
			inputTrie(root->children[c - 'a'], fin);
		}
		else if (c == ' ')
		{
			root->children[26] = new TrieEng();
			inputTrie(root->children[26], fin);
		}
		else if (c == '-')
		{
			root->children[27] = new TrieEng();
			inputTrie(root->children[27], fin);
		}
		else if (c >= '0' && c <= '9')
		{
			root->children[c - '0' + 28] = new TrieEng();
			inputTrie(root->children[c - '0' + 28], fin);
		}
	}
}

bool loadTriefromFile(TrieEng*& root, string path) {
	wifstream fin;
	fin.open(path,ios::binary);

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

