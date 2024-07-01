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

	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
	wofstream fout;
	start = chrono::high_resolution_clock::now();
	fout.open("Dataset\\UserVData.bin", ios::binary);
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


int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);
	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
	Vtrie* root = new Vtrie();
	fillMap();
	wifstream fin;
	auto start = chrono::high_resolution_clock::now();
	fin.open("Dataset\\UserVData.bin", ios::binary);
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



/* 
int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);
	wstring word = L"Nguyễn Đức Thịnh";
	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
	wofstream fout;
	fout.open("Dataset\\test.txt", ios::binary);

	if (fout.is_open())
	{
		fout.imbue(loc);
		short int len = word.length();
		fout.write((wchar_t*)&len, sizeof(short int));
		fout.write((wchar_t*)word.c_str(), len);
		fout.close();
	}
	else
		wcout << L"Không thể mở file để lưu dữ liệu!!!" << endl;
	return 0;

}
*/
/* 
int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);
	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin;
	fin.open("Dataset\\test.txt", ios::binary);
	if (!fin.is_open())
	{
		wcout << L"Không tìm thấy file dữ liệu!!!" << endl;
		return 0;
	}
	fin.imbue(loc);
	short int len;
	fin.read((wchar_t*)&len, sizeof(short int));
	wchar_t* buffer = new wchar_t[len + 1];
	fin.read(buffer, len);
	buffer[len] = L'\0';
	wstring word = buffer;
	wcout << word << endl;
	delete[] buffer;
	fin.close();
	return 0;

}
*/