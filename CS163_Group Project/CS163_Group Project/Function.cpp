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
		}
	}
	current->isEnd = true;
	current->definition.push_back(definition);
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
