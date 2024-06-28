#include "functions.h"
#include "fstream"

//int main()
//{
//	trie* root = new trie();
//	 check how long to load the raw data
//	auto start = chrono::high_resolution_clock::now();
//	if (!loadRawData(root))
//	{
//		cout << "Failed to load data\n";
//		return 0;
//	}
//	auto end = chrono::high_resolution_clock::now();
//	cout << "Time taken to load raw data: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
//	string word;
//	vector<string> definition;
//	while (1)
//	{
//		cout << "Enter a word: ";
//		getline(cin, word);
//		if (word == "exit") break;
//		 check how long it takes to find the word
//		start = chrono::high_resolution_clock::now();
//		if (findWordMeaning(root, word, definition))
//	{
//			end = chrono::high_resolution_clock::now();
//			cout << "Time taken to find the word: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
//			int i = 0;
//			for (string& str : definition)
//			{
//				cout << "Meaning " << ++i << ":" << " ";
//				cout << str << endl;
//			}
//		}
//		else
//		{
//			cout << "Word not found\n";
//		}
//	}
//	ofstream fout;
//	fout.open("Dataset\\userData.bin", ios::binary);
//	saveTrie(root, fout);
//	fout.close();
//
//	deleteTrie(root);
//	return 0;
//}


/* 
int main()
{
	trie* root = new trie();
	ifstream fin;

	// check how long it takes to load the data
	auto start = chrono::high_resolution_clock::now();
	fin.open("Dataset\\userData.bin", ios::binary);
	if (!fin.is_open())
	{
		cout << "File not found\n";
		return 0;
	}
	loadTrie(root, fin);
	auto end = chrono::high_resolution_clock::now();
	cout << "Time taken to load data: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
	string word;
	vector<string> definition;
	while (1)
	{
		cout << "Enter a word: ";
		getline(cin, word);
		if (word == "exit") break;
		// check how long it takes to find the word
		start = chrono::high_resolution_clock::now();
		if (findWordMeaning(root, word, definition))
		{
			end = chrono::high_resolution_clock::now();
			cout << "Time taken to find the word: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
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
*/ 




int main() 
{
	fillMap();
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	Vtrie* root = new Vtrie();
	wstring word;
	wstring definition;
	while (1)
	{
		wcout << L"Enter a word: ";
		getline(wcin, word);
		if (word == L"exit") break;
		wcout << L"Enter the definition: ";
		getline(wcin, definition);
		VInsertWord(root, word, definition);
	}
	vector<wstring> meaning;
	while (1)
	{
		wcout << L"Enter a word: ";
		getline(wcin, word);
		if (word == L"exit") break;
		if (VFindWordMeaning(root, word, meaning))
		{
			int i = 0;
			for (wstring& str : meaning)
			{
				wcout << L"Meaning " << ++i << L":" << L" ";
			wcout << str << endl;
			}
		}
		else
		{
			wcout << L"Word not found\n";
		}
	}
	VDeleteTrie(root);
	return 0;
}



