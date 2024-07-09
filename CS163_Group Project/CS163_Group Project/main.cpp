#include "functions.h"
#include <fstream>
/* 
int main()
{
	trie* root = new trie();
	 
	auto start = chrono::high_resolution_clock::now();
	if (!loadRawData(root))
	{
		cout << "Failed to load data\n";
		return 0;
	}
	auto end = chrono::high_resolution_clock::now();
	cout << "Time taken to load raw data: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
	string word;
	vector<string> definition;
	while (1)
	{
		cout << "Enter a word: ";
		getline(cin, word);
		if (word == "exit") break;
		
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
	// test function to change a word meaning
	string newDefinition;
	cout << "Enter a word to change its meaning: ";
	getline(cin, word);
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
	int indexBase1;
	cout << "Enter the index of the meaning you want to change: ";
	cin >> indexBase1;
	cin.ignore();
	cout << "Enter the new meaning: ";
	getline(cin, newDefinition);
	start = chrono::high_resolution_clock::now();
	if (changeWordDefinition(root, word, newDefinition, indexBase1 - 1))
	{
		end = chrono::high_resolution_clock::now();
		cout << "Time taken to change the meaning: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
		cout << "Meaning changed successfully\n";
	}
	else
	{
		cout << "Word not found\n";
	}

	cout << "Enter a word to find: ";
	getline(cin, word);
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

	ofstream fout;
	//fout.open("Dataset\\userData.bin", ios::binary);
	fout.open("Dataset\\test.bin", ios::binary);
	saveTrie(root, fout);
	fout.close();

	deleteTrie(root);
	return 0;
}
*/

/* 
int main()
{
	trie* root = new trie();
	ifstream fin;

	// check how long it takes to load the data
	auto start = chrono::high_resolution_clock::now();
	//fin.open("Dataset\\userData.bin", ios::binary);
	fin.open("Dataset\\test.bin", ios::binary);
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
			if (addToHistory(word, root))
			{
				cout << "Word added to history\n";
			}
			else
			{
				cout << "Word not found\n";
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
	trie* root = new trie();
	ifstream fin;

	// check how long it takes to load the data
	auto start = chrono::high_resolution_clock::now();
	//fin.open("Dataset\\userData.bin", ios::binary);
	fin.open("Dataset\\test.bin", ios::binary);
	if (!fin.is_open())
	{
		cout << "File not found\n";
		return 0;
	}
	loadTrie(root, fin);
	auto end = chrono::high_resolution_clock::now();
	cout << "Time taken to load data: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
	list<string> favWords;
	// load favorite words
	loadFavWord(favWords);
	// test function to save favorite words
	string word;
	vector<string> definition;
	while (1)
	{
		cout << "Enter a word: ";
		getline(cin, word);
		if (word == "exit") break;

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
		bool option;
		cout << "Enter 1 to mark this word as favorite, 0 to skip: ";
		cin >> option;
		cin.ignore();
		if (option)
		{
			likeAWord(favWords, word, root);
		}
	}

	// test function to unlike a word
	cout << "These are your favorite words\n";
	for (string& str : favWords)
	{
		cout << str << endl;
	}
	cout << "Enter a word to unlike: ";
	getline(cin, word);
	unlikeAWord(favWords, word, root);
	// save favorite words
	saveFavWord(favWords, root);
	deleteTrie(root);
	return 0;
}


/* 
int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);
	
	Vtrie* root = new Vtrie();
	auto start = chrono::high_resolution_clock::now();
	fillMap();
	if (VloadRawData(root))
	{
		auto end = chrono::high_resolution_clock::now();
		wcout << L"Thời gian tải dữ liệu: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L"ms" << endl;
		wstring a = L"";
		while (VToLower(a) != L"exit")
		{
			vector<wstring> means;
			wcout << L"Nhập từ để tìm kiếm: "; getline(wcin, a);
			VFindWordMeaning(root, a, means);
			if (means.size() == 0)
				wcout << L"Không tìm được từ!!!" << endl;
			for (int i = 0; i < means.size(); ++i)
				wcout << L"Nghĩa " << i + 1 << L": " << means[i] << endl;
		}
	}
	else
		wcout << L"Tải dữ liệu thất bại!!!" << endl;

	// test function to change a word meaning
	wstring newDefinition;
	wstring word = L"";
	vector<wstring> definition;
	wcout << L"Nhập từ cần thay đổi nghĩa: "; getline(wcin, word);
	if (VFindWordMeaning(root, word, definition))
	{
		for (int i = 0; i < definition.size(); ++i)
			wcout << L"Nghĩa " << i + 1 << L": " << definition[i] << endl;
	}
	else
		wcout << L"Không tìm thấy từ cần thay đổi nghĩa!!!" << endl;
	int indexBase1;
	wcout << L"Nhập chỉ số của nghĩa cần thay đổi: "; wcin >> indexBase1;
	wcin.ignore();
	wcout << L"Nhập nghĩa mới: "; getline(wcin, newDefinition);
	
	// check how long it takes to change the meaning
	
	start = chrono::high_resolution_clock::now();
	if (VChangeWordDefinition(root, word, newDefinition, indexBase1 - 1))
	{
		auto end = chrono::high_resolution_clock::now();
		wcout << L"Thời gian thay đổi nghĩa: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L"ms" << endl;
		wcout << L"Thay đổi nghĩa thành công!!!" << endl;
	}
	else
		wcout << L"Không tìm thấy từ cần thay đổi nghĩa!!!" << endl;

	// check if the meaning has been changed
	wcout << L"Nhập từ cần tìm: "; getline(wcin, word);
	start = chrono::high_resolution_clock::now();
	if (VFindWordMeaning(root, word, definition))
	{
		auto end = chrono::high_resolution_clock::now();
		wcout << L"Thời gian tìm từ: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L"ms" << endl;
		for (int i = 0; i < definition.size(); ++i)
			wcout << L"Nghĩa " << i + 1 << L": " << definition[i] << endl;
	}
	else
		wcout << L"Không tìm thấy từ cần tìm!!!" << endl;

	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
	wofstream fout;
	start = chrono::high_resolution_clock::now();
	//fout.open("Dataset\\UserVData.bin", ios::binary);
	fout.open("Dataset\\testV.bin", ios::binary);
	if (fout.is_open())
	{
		fout.imbue(loc);
		
		saveVtrie(root, fout);
		fout.close();
		auto end = chrono::high_resolution_clock::now();
		wcout << L"Thời gian lưu dữ liệu: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L"ms" << endl;
	}
	else
		wcout << L"Không thể mở file để lưu dữ liệu!!!" << endl;
	VDeleteTrie(root);
	return 0;
}
*/


/* 
int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);
	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
	Vtrie* root = new Vtrie();
	fillMap();
	wifstream fin;
	auto start = chrono::high_resolution_clock::now();
	//fin.open("Dataset\\UserVData.bin", ios::binary);
	fin.open("Dataset\\testV.bin", ios::binary);
	if (!fin.is_open())
	{
		wcout << L"Không tìm thấy file dữ liệu!!!" << endl;
		return 0;
	}
	fin.imbue(loc);
	loadVtrie(root, fin);
	auto end = chrono::high_resolution_clock::now();
	wcout << L"Thời gian tải dữ liệu: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L"ms" << endl;
	wstring a = L"";
	while (VToLower(a) != L"exit")
	{
		vector<wstring> means;
		wcout << L"Nhập từ để tìm kiếm: "; getline(wcin, a);
		VFindWordMeaning(root, a, means);
		if (means.size() == 0)
			wcout << L"Không tìm được từ!!!" << endl;
		for (int i = 0; i < means.size(); ++i)
			wcout << L"Nghĩa " << i + 1 << L": " << means[i] << endl;
	}
	VDeleteTrie(root);
	return 0;
}
*/ 
/* 
int main()
{
	trie* root = new trie();
	auto start = chrono::high_resolution_clock::now();
	if (!loadRawData(root))
	{
		cout << "Failed to load data\n";
		return 0;
	}
	auto end = chrono::high_resolution_clock::now();
	cout << "Time taken to load raw data: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
	string word;
	vector<string> definition;
	while (true)
	{
		// Searching word
		cout << "\nLet's search for a word!\n";
		cout << "Enter a word: ";
		getline(cin, word);
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
			cout << "Word not found\n";

		// Random a word and 4 definitions
		cout << "\nLet's do a Vocabulary test!\n";
		string rightWord;
		vector<string> rightDefinition, wrongDefinition1, wrongDefinition2, wrongDefinition3;
		randomAWordAnd4Definitions(root, rightWord, rightDefinition, wrongDefinition1, wrongDefinition2, wrongDefinition3);
		cout << "Here is the word: " << rightWord << "\n";
		cout << "Here are 4 definitions, choose the most correct definition for the word '" << rightWord << "'\n";
		cout << "A. " << wrongDefinition1[0] << "\n";
		cout << "B. " << wrongDefinition2[0] << "\n";
		cout << "C. " << rightDefinition[0] << "\n";
		cout << "D. " << wrongDefinition3[0] << "\n";
		cout << "Your answer (A/B/C/D): ";
		string answer;
		cin >> answer;
		if (answer == "C")
			cout << "\nCorrect!\n";
		else cout << "\nWrong!\n";

		// Random a definition and 4 words
		cout << "\nLet's do another Vocabulary test!\n";
		vector<string> rightDefinitionN;
		string rightWordN, wrongWord1N, wrongWord2N, wrongWord3N;
		randomADefinitionAnd4Words(root, rightDefinitionN, rightWordN, wrongWord1N, wrongWord2N, wrongWord3N);
		cout << "Here is the definition: " << rightDefinitionN[0] << "\n";
		cout << "Here are 4 words, choose the most correct word for the definition above\n";
		cout << "A. " << wrongWord1N << "\n";
		cout << "B. " << wrongWord2N << "\n";
		cout << "C. " << rightWordN << "\n";
		cout << "D. " << wrongWord3N << "\n";
		cout << "Your answer (A/B/C/D): ";
		string answerN;
		cin >> answerN;
		if (answerN == "C")
			cout << "\nCorrect!\n";
		else cout << "\nWrong!\n";

		// Exit the program
		cout << "\nDo you want to exit the dictionary? (Y/N): ";
		bool wantToExit = 0;
		cout << "Enter 1 to exit, 0 to continue: ";
		cin >> wantToExit;
		if (wantToExit)
			break;
	}
	cout << "\nThanks for using the dictionary!\n";
	deleteTrie(root);
	return 0;
}
*/