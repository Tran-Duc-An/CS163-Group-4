#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <codecvt>
#include "functions.h"
#include "UI.h";
using namespace std;


//int main() {
//	run();
//	return 0;
//}

int main()
{
	// TEST function to load EE trie and load example from file
	EETrie* root = new EETrie();
	if (!EE::loadTrieFromFile(root, "Dataset/UserTrieEN.bin"))
	{
		cout << "Cannot load trie from file" << endl;
	}
	if (!EE::loadExample(root, "Dataset/words_examples.csv"))
	{
		cout << "Cannot load example from file" << endl;
	}
	string word;
	EETrie* node;
	string example;
	while (1)
	{
		cout << "Enter a word: ";
		getline(cin, word);
		if (word == "exit")
		{
			break;
		}
		example = EE::getExample(root, word);
		cout << example << endl;
	}
	EE::deleteTrie(root);
	return 0;
}