#pragma once
#include <vector>
#include <string>
using namespace std;

struct Trie_Node
{
	char data;
	Trie_Node* children[26] = { nullptr };
	bool is_word = false;
	vector <string> definition;
	Trie_Node(char data) : data(data) {}
};

bool insertword(Trie_Node** root, string data);