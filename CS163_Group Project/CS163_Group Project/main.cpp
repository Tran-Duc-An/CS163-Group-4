#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct trie {

	vector<trie*>  children;
	bool isend = false;
	vector<string> definition;
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
void deleteTrie(trie* root)
{
	if (root == nullptr) return;
	for (trie* child : root->children)
	{
		deleteTrie(child);
	}
	delete root;
}