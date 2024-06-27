#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

struct trie {

	vector<trie*>  children;
	bool isend = false;
	vector<string> definition;
	int numChildren = 0;
	trie() : children(26, nullptr) {}
};


string toLowerCase(string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}
	return str;
}
void insertWord(trie*& root, string& word, string& definition)
{
	word = toLowerCase(word);
	trie* current = root;
	for (char c : word)
	{
		if (current->children[c - 'a'] == nullptr)
		{
			current->children[c - 'a'] = new trie;
			current->numChildren++;
		}
		current = current->children[c - 'a'];
	}
	current->isend = true;
	current->definition.push_back(definition);
}
bool findWordMeaning(trie* root, string& word, vector<string>& meaning)
{
	word = toLowerCase(word);
	trie* current = root;
	for (char c : word)
	{
		if (current->children[c - 'a'] == nullptr)
		{
			return false;
		}
		current = current->children[c - 'a'];
	}
	if (current->isend)
	{
		meaning = current->definition;
		return true;
	}
	return false;
}
/* 
void saveTrie(trie* root, ofstream& fout)
{
	if (root == nullptr) return;
	fout << root->definition.size();
	for (string& str : root->definition)
	{
		fout << str.length();
		fout << str;
	}
	fout << root->numChildren;
	for (int i = 0; i < 26; ++i)
	{
		if (root->children[i] != nullptr)
		{
			fout << (char)(i + 'a');
			saveTrie(root->children[i], fout);
		}
	}
	
}

void loadTrie(trie*& root, ifstream& fin)
{
	root = new trie();
	int numDefinitions;
	fin >> numDefinitions;
	for (int i = 0; i < numDefinitions; ++i)
	{
		int len;
		fin >> len;
		char* tmp = new char[len + 1];
		fin.read(tmp, len);
		root->definition.push_back(tmp);
	}
	fin >> root->numChildren;
	for (int i = 0; i < root->numChildren; ++i)
	{
		char c;
		fin >> c;
		root->children[c - 'a'] = new trie();
		loadTrie(root->children[c - 'a'], fin);
	}
}
*/ 


// save trie in binary file
void saveTrie(trie* root, ofstream& fout)
{
	if (root == nullptr) return;
	int numDefinitions = root->definition.size();
	fout.write((char*)&numDefinitions, sizeof(int));
	for (string& str : root->definition)
	{
		int len = str.length();
		fout.write((char*)&len, sizeof(int));
		fout.write(str.c_str(), len);
	}
	int numChildren = root->numChildren;
	fout.write((char*)&numChildren, sizeof(int));
	for (int i = 0; i < 26; ++i)
	{
		if (root->children[i] != nullptr)
		{
			char c = i + 'a';
			fout.write(&c, sizeof(char));
			saveTrie(root->children[i], fout);
		}
	}
}	


// load trie from binary file
void loadTrie(trie*& root, ifstream& fin)
{
	int numDefinitions;
	fin.read((char*)&numDefinitions, sizeof(int));
	if (numDefinitions != 0) root->isend = true;
	for (int i = 0; i < numDefinitions; ++i)
	{
		int len;
		fin.read((char*)&len, sizeof(int));
		char* tmp = new char[len + 1];
		fin.read(tmp, len);
		tmp[len] = '\0';
		root->definition.push_back(tmp);
		delete[] tmp;
	}
	fin.read((char*)&root->numChildren, sizeof(int));
	for (int i = 0; i < root->numChildren; ++i)
	{
		char c;
		fin.read(&c, sizeof(char));
		root->children[c - 'a'] = new trie();
		loadTrie(root->children[c - 'a'], fin);
	}
}

void deleteTrie(trie* root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 26; ++i)
	{
		deleteTrie(root->children[i]);
	}
	delete root;
}
int main()
{
	trie* root = new trie();
	string word;
	string def;
	vector<string> definition;
	ifstream fin;
	fin.open("Dataset\\userDat.bin", ios::binary);
	if (fin.is_open())
	{
		loadTrie(root, fin);
	}
	fin.close();
	while (1)
	{
		cout << "Enter a word: ";
		cin >> word;
		if (word == "exit") break;
		if (findWordMeaning(root, word, definition))
		{
			for (string& str : definition)
			{
				cout << str << endl;
			}
		}
		else
		{
			cout << "Word not found\n";
		}
	}
	deleteTrie(root);
	return 0;
}