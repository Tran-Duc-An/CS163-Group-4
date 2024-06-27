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
	trie() : children(38, nullptr) {}
};


string toLowerCase(string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}
	return str;
}
// remove all letters other than 26 english alphabets, 0 to 9, space, hyphen
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
void insertWord(trie*& root, string& word, string& definition)
{
	word = toLowerCase(word);
	word = removeSpecialCharacters(word);
	trie* current = root;
	for (char c : word)
	{
		// a to z is 0 to 25
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr)
			{
				current->children[c - 'a'] = new trie;
				current->numChildren++;
			}
			current = current->children[c - 'a'];
		}
		// space is 26
		else if (c == ' ')
		{
			if (current->children[26] == nullptr)
			{
				current->children[26] = new trie;
				current->numChildren++;
			}
			current = current->children[26];
		}
		// hyphen is 27
		else if (c == '-')
		{
			if (current->children[27] == nullptr)
			{
				current->children[27] = new trie;
				current->numChildren++;
			}
			current = current->children[27];
		}
		// 0 to 9 is 28 to 37
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr)
			{
				current->children[c - '0' + 28] = new trie;
				current->numChildren++;
			}
			current = current->children[c - '0' + 28];
		}
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
	if (!current->isend) return false;
	meaning = current->definition;
	return true;
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
	// load children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < root->numChildren; ++i)
	{
		char c;
		fin.read(&c, sizeof(char));
		if (c >= 'a' && c <= 'z')
		{
			root->children[c - 'a'] = new trie();
			loadTrie(root->children[c - 'a'], fin);
		}
		else if (c == ' ')
		{
			root->children[26] = new trie();
			loadTrie(root->children[26], fin);
		}
		else if (c == '-')
		{
			root->children[27] = new trie();
			loadTrie(root->children[27], fin);
		}
		else if (c >= '0' && c <= '9')
		{
			root->children[c - '0' + 28] = new trie();
			loadTrie(root->children[c - '0' + 28], fin);
		}
	}
}

void deleteTrie(trie* root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 38; ++i)
	{
		deleteTrie(root->children[i]);
	}
	delete root;
}

bool loadRawData(trie*& root)
{
	ifstream fin;
	fin.open("Dataset\\englishDictionary.csv");
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
		insertWord(root, word, definition);
	}
	return true;
}

/* 
int main()
{
	trie* root = new trie();
	if (!loadRawData(root))
	{
		cout << "Failed to load data\n";
		return 0;
	}
	string word;
	vector<string> definition;
	while (1)
	{
		cout << "Enter a word: ";
		getline(cin, word);
		if (word == "exit") break;
		if (findWordMeaning(root, word, definition))
		{
			int i = 0;
			for (string& str : definition)
			{
				cout << "Meaning " << ++i << ":" << " ";
				cout << str << endl;
			}
		}
		else
		{
			cout << "Word not found\n";
		}
	}
	ofstream fout;
	fout.open("Dataset\\userDat.bin", ios::binary);
	saveTrie(root, fout);
	fout.close();

	deleteTrie(root);
	return 0;
}
*/ 


int main()
{
	trie* root = new trie();
	ifstream fin;
	fin.open("Dataset\\userDat.bin", ios::binary);
	if (!fin.is_open())
	{
		cout << "File not found\n";
		return 0;
	}
	loadTrie(root, fin);
	string word;
	vector<string> definition;
	while (1)
	{
		cout << "Enter a word: ";
		getline(cin, word);
		if (word == "exit") break;
		if (findWordMeaning(root, word, definition))
		{
			int i = 0;
			for (string& str : definition)
			{
				cout << "Meaning " << ++i << ":" << " ";
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

