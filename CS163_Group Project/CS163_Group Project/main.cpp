#include <iostream>
#include <vector>
using namespace std;

struct Trie {
	vector<char> letters;
	vector<Trie*>  children;
	bool isEnd = 0;
	vector<string> definition;
};

void insert(Trie*& root, string s, int k, int pos, string definition) {
	if (k == pos) {
		if (root == nullptr) {
			root = new Trie;
			root->letters.push_back(s[pos]);
			root->children.push_back(nullptr);
		}
		else {
			bool flag = 0;
			for (int i = 0; i < root->letters.size(); i++) {
				if (s[pos] == root->letters[i]) flag = 1;
			}
			if (flag == 0) {
				root->letters.push_back(s[pos]);
				root->children.push_back(nullptr);
			}
		}
		if (pos == s.length() - 1) {
			root->isEnd = 1;
			root->definition.push_back(definition);
		}
	}
	else {
		for (int i = 0; i < root->letters.size(); i++) {
			if (s[k] == root->letters[i]) insert(root->children[i], s, k + 1, pos, definition);
		}
	}
}

void createTrie(Trie*& root, vector<string> s, vector<string> def) {
	for (int i = 0; i < s.size(); i++) {
		for (int j = 0; j < s[i].length(); j++) insert(root, s[i], 0, j, def[i]);
	}
}

bool find(Trie* root, string s, int k) {
	if (root == nullptr) return 0;
	if (k == s.length() - 1 && root->isEnd == 1) {
		return 1;
	}
	else if (k == s.length() - 1 && root->isEnd == 0) return 0;
	for (int i = 0; i < root->letters.size(); i++) {
		if (s[k] == root->letters[i]) return find(root->children[i], s, k + 1);
	}
	return 0;
}

void deleteTrie(Trie*& root) {
	if (root == nullptr) return;
	for (int i = 0; i < root->children.size(); i++)
	{
		deleteTrie(root->children[i]);
		delete root;
	}
	root = nullptr;
}

int main() {
	vector<string> s = { "apple", "an", "atomic" };
	vector<string> def = { "a round fruit with shiny red or green skin that is fairly hard and white inside",
		"used instead of a when the following word begins with a vowel sound",
		"using the energy that is created when an atom is divided" };
	Trie* root = nullptr;
	createTrie(root, s, def);
	string ss = "apple";
	cout << find(root, ss, 0);
	deleteTrie(root);
	return 0;
}