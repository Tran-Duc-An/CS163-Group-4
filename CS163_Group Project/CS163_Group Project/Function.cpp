#include "Function.h"
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <locale>
#include <codecvt>
using namespace std;
string toLowerCase(string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}
	return str;
}
void insertWord(TrieEng*& root, string word, wstring definition)
{
	word = toLowerCase(word);
	TrieEng* current = root;
	for (char c : word)
	{
		if (current->children[c - 'a'] == nullptr)
		{
			current->children[c - 'a'] = new TrieEng();
		}
		current = current->children[c - 'a'];
	}
	current->isEnd = true;
	current->definition.push_back(definition);
}

void createTrieEng(TrieEng*& root, vector<string> words, vector<wstring> definition) {
	for (int i = 0; i < words.size(); i++) {
		insertWord(root, words[i], definition[i]);
	}
}

bool findWordMeaning(TrieEng* root, string word, vector<wstring>& meaning)
{
	word = toLowerCase(word);
	TrieEng* current = root;
	for (char c : word)
	{
		if (current->children[c - 'a'] == nullptr)
		{
			return false;
		}
		current = current->children[c - 'a'];
	}
	if (current->isEnd)
	{
		meaning = current->definition;
		return true;
	}
	return false;
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

void createTrieEngFromFile(TrieEng*& root,string path) {
	wifstream fin;
	fin.imbue(locale(fin.getloc(), new codecvt_utf8_utf16<wchar_t>));
	fin.open(path);
	if (!fin.is_open()) return;
	vector<string> words;
	vector<wstring> definition;
	std::wstring line;

	while (getline(fin, line)) {
		size_t pos = line.find(L';');
		if (pos != wstring::npos) {
			wstring wword = line.substr(0, pos);
			wstring def = line.substr(pos + 1);

			// Convert wword from wstring to string
			string word(wword.begin(), wword.end());

			words.push_back(word);
			definition.push_back(def);
		}
	}

	createTrieEng(root, words, definition);
	fin.close();
}

