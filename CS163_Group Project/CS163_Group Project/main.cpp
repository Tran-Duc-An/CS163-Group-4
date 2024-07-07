#include "functions.h"

// testing Eng-Eng Dictionary
int main()
{
	cout << "Here's the Dictionary\n";
	EETrie* root = new EETrie();
	if (!EE::loadRawData(root, ".\\Dataset\\english Dictionary.csv"))
	{
		cout << "Failed to load data\n";
		return 0;
	}
	string word;
	vector<string> definition;
	while (continueProgram(0))
	{
		// Searching word
		cout << "\nLet's search for a word!\n";
		cout << "Enter a word: ";
		getline(cin, word);
		if (EE::findWordMeaning(root, word, definition))
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

		// Random a word and 4 definitions
		cout << "\nLet's do a Vocabulary test!\n";
		string rightWord;
		vector<string> rightDefinition, wrongDefinition1, wrongDefinition2, wrongDefinition3;
		EE::randomAWordAnd4Definitions(root, rightWord, rightDefinition, wrongDefinition1, wrongDefinition2, wrongDefinition3);
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
		EE::randomADefinitionAnd4Words(root, rightDefinitionN, rightWordN, wrongWord1N, wrongWord2N, wrongWord3N);
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
	}
	cout << "\nThanks for using the dictionary!\n";
	ofstream fout;
	fout.open("Dataset\\userData.bin", ios::binary);
	EE::saveTrie(root, fout);
	fout.close();

	EE::deleteTrie(root);
	return 0;
}
// testing Viet-Eng Dictionary
//int main()
//{
//	_setmode(_fileno(stdout), _O_WTEXT);
//	_setmode(_fileno(stdin), _O_WTEXT);
//	locale loc = locale(locale(), new codecvt_utf8<wchar_t>);
//
//	wcout << L"Dưới đây là từ điển\n";
//	Vtrie* root = new Vtrie();
//	fillMap();
//	if (!VloadRawData(root))
//	{
//		wcout << L"Không tải được dữ liệu!\n";
//		return 0;
//	}
//	wstring vWord = L"";
//	vector<wstring> vDefinition;
//	while (continueProgram(1))
//	{
//		// Tìm từ
//		wcout << L"\nHãy tìm từ vựng!\n";
//		wcout << L"Nhập một từ: ";
//		getline(wcin, vWord);
//		if (VFindWordMeaning(root, vWord, vDefinition))
//		{
//			int i = 0;
//			for (wstring& str : vDefinition)
//			{
//				wcout << L"Nghĩa " << ++i << ":" << " ";
//				wcout << str << endl;
//			}
//		}
//		else
//		{
//			wcout << L"Không tìm được từ!\n";
//		}
//
//		// Ngẫu nhiên một từ và 4 định nghĩa
//		wcout << L"\nHãy cùng làm một bài tập từ vựng nhé!\n";
//		wstring rightWord;
//		vector<wstring> rightDefinition, wrongDefinition1, wrongDefinition2, wrongDefinition3;
//		randomAVWordAnd4Definitions(root, rightWord, rightDefinition, wrongDefinition1, wrongDefinition2, wrongDefinition3);
//		wcout << L"Đây là từ: " << rightWord << "\n";
//		wcout << L"Sau đây là 4 định nghĩa, hãy chọn định nghĩa đúng nhất với từ '" << rightWord << "'\n";
//		wcout << L"A. " << wrongDefinition1[0] << "\n";
//		wcout << L"B. " << wrongDefinition2[0] << "\n";
//		wcout << L"C. " << rightDefinition[0] << "\n";
//		wcout << L"D. " << wrongDefinition3[0] << "\n";
//		wcout << L"Đáp án của bạn (A/B/C/D): ";
//		wstring answer;
//		wcin >> answer;
//		if (answer == L"C")
//			wcout << L"\nĐúng!\n";
//		else wcout << L"\nSai!\n";
//
//		// Ngẫu nhiên một định nghĩa và 4 từ
//		wcout << L"\nHãy cùng làm một bài tập khác!\n";
//		vector<wstring> rightDefinitionN;
//		wstring rightWordN, wrongWord1N, wrongWord2N, wrongWord3N;
//		randomAVDefinitionAnd4Words(root, rightDefinitionN, rightWordN, wrongWord1N, wrongWord2N, wrongWord3N);
//		wcout << L"Đây là định nghĩa: " << rightDefinitionN[0] << "\n";
//		wcout << L"Sau đây là 4 từ, hãy chọn từ đúng nhất ứng với định nghĩa trên\n";
//		wcout << L"A. " << wrongWord1N << "\n";
//		wcout << L"B. " << wrongWord2N << "\n";
//		wcout << L"C. " << rightWordN << "\n";
//		wcout << L"D. " << wrongWord3N << "\n";
//		wcout << L"Đáp án của bạn (A/B/C/D): ";
//		wstring answerN;
//		wcin >> answerN;
//		if (answerN == L"C")
//			wcout << L"\nĐúng!\n";
//		else wcout << L"\nSai!\n";
//	}
//	wcout << L"\nCảm ơn đã dùng từ điển!\n";
//	wofstream fout;
//	fout.open("Dataset\\userVData.bin", ios::binary);
//	saveVtrie(root, fout);
//	fout.close();
//
//	VDeleteTrie(root);
//	return 0;
//}
