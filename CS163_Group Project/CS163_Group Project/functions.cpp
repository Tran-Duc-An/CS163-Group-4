#include "functions.h"

// Global functions
bool continueProgram(int type)
{
	int choice;
	if (type == 0)
	{
		cout << "\nDo you want to continue the program?\n";
		cout << "Enter '1' for Yes or '0' for No\n";
		cout << "Enter a number: ";
		cin >> choice;
		cin.ignore();
		if (choice != 1 && choice != 0)
		{
			cout << "\nYou missed the instruction, please read it again carefully!\n";
			return continueProgram(0);
		}
	}
	else if (type == 1)
	{
		wcout << L"\nBạn có muốn tiếp tục chương trình không?\n";
		wcout << L"Nhập '1' để Đồng ý hoặc '0' để Từ chối\n";
		wcout << L"Nhập một số: ";
		cin >> choice;
		cin.ignore();
		if (choice != 1 && choice != 0)
		{
			wcout << L"\nBạn đã bỏ qua hướng dẫn, hãy đọc lại hướng dẫn kĩ hơn nhé!\n";
			return continueProgram(1);
		}
	}
	return choice;
}
string toLowerCase(string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}
	return str;
}
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
void preProcessing(string& word) {
	size_t bracket_pos = word.find('[');
	if (bracket_pos != string::npos)
	{
		word = word.substr(0, bracket_pos - 1);
	}
	size_t bracket_pos_open = word.find('(');
	size_t bracket_pos_close = word.find(')');
	if (bracket_pos_open != string::npos)
	{
		word = word.erase(bracket_pos_open - 1, bracket_pos_close - bracket_pos_open + 2);
	}
	while (!word.empty() && word[0] == ' ') word.erase(word.begin(), word.begin() + 1);
	while (!word.empty() && word[word.length() - 1] == ' ') word.erase(word.length() - 2, word.length() - 1);
}

// Eng-Viet trie functions
bool EV::loadRawData(EVTrie*& root, string path)
{

	wifstream fin;
	fin.open(path);

	// Ensure the file is opened with UTF-8 encoding
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));

	if (!fin.is_open())
	{
		return false;
	}

	wstring wword;
	wstring definition;


	while (getline(fin, wword, L',')) {
		if (getline(fin, definition, L'\n'))
		{
			// Convert wstring to string using wstring_convert
			wstring_convert<codecvt_utf8<wchar_t>> converter;
			string word = converter.to_bytes(wword);

			preProcessing(word);
			EV::insertWord(root, word, definition);
		}
	}

	return true;
}
void EV::inputTrie(EVTrie*& root, wifstream& fin) {
	short int numDefinitions = 0;
	fin.read((wchar_t*)&numDefinitions, sizeof(short int));
	if (numDefinitions != 0) {
		root->isEnd = true;
		for (int i = 0; i < numDefinitions; ++i)
		{
			short int len;
			fin.read((wchar_t*)&len, sizeof(short int));
			wchar_t* tmp = new wchar_t[len + 1];
			fin.read(tmp, len);
			tmp[len] = '\0';
			root->definition.push_back(tmp);
			delete[] tmp;
		}
	}
	fin.read((wchar_t*)&root->numChildren, sizeof(short int));
	// load children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < root->numChildren; ++i)
	{
		wchar_t c;
		fin.read(&c, sizeof(wchar_t));
		if (c >= L'a' && c <= L'z')
		{
			root->children[c - L'a'] = new EVTrie();
			inputTrie(root->children[c - L'a'], fin);
		}
		else if (c == L' ')
		{
			root->children[26] = new EVTrie();
			inputTrie(root->children[26], fin);
		}
		else if (c == L'-')
		{
			root->children[27] = new EVTrie();
			inputTrie(root->children[27], fin);
		}
		else if (c >= L'0' && c <= L'9')
		{
			root->children[c - L'0' + 28] = new EVTrie();
			inputTrie(root->children[c - L'0' + 28], fin);
		}
	}
}
bool EV::loadTrieFromFile(EVTrie*& root, string path) {
	wifstream fin;
	fin.open(path, ios::binary);

	// Ensure the file is opened with UTF-8 encoding
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));

	if (!fin.is_open())
	{
		return false;
	}
	inputTrie(root, fin);
	fin.close();
	return true;
}
void EV::outputTrie(EVTrie* root, wofstream& fou) {
	if (root == nullptr) return;

	short int numDefinitions = root->definition.size();
	fou.write((wchar_t*)(&numDefinitions), sizeof(short int));
	if (root->definition.size() != 0) {
		for (wstring& str : root->definition) {
			short int len = str.length();
			fou.write((wchar_t*)(&len), sizeof(short int));
			fou.write(str.c_str(), len);
		}
	}
	short int numChildren = root->numChildren;
	fou.write((wchar_t*)(&numChildren), sizeof(short int));
	// save children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < 38; ++i) {
		if (root->children[i] != nullptr) {
			wchar_t c = i < 26 ? (wchar_t)(i + L'a') : (i == 26 ? L' ' : (i == 27 ? L'-' : (i - 28 + L'0')));

			fou.write(&c, sizeof(wchar_t));
			outputTrie(root->children[i], fou);
		}
	}
}
void EV::saveTrieToFile(EVTrie* root, string path) {
	wofstream fou;
	fou.open(path, ios::binary);

	// Ensure the file is opened with UTF-8 encoding
	fou.imbue(locale(fou.getloc(), new codecvt_utf8<wchar_t>));

	outputTrie(root, fou);
	fou.close();
}
void EV::insertWord(EVTrie*& root, string& word, wstring& definition)
{
	word = toLowerCase(word);
	word = removeSpecialCharacters(word);
	EVTrie* current = root;

	for (char c : word)
	{
		// a to z is 0 to 25
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr)
			{
				current->children[c - 'a'] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[c - 'a'];
		}
		// space is 26
		else if (c == ' ')
		{
			if (current->children[26] == nullptr)
			{
				current->children[26] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[26];
		}
		// hyphen is 27
		else if (c == '-')
		{
			if (current->children[27] == nullptr)
			{
				current->children[27] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[27];
		}
		// 0 to 9 is 28 to 37
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr)
			{
				current->children[c - '0' + 28] = new EVTrie;
				current->numChildren++;
			}
			current = current->children[c - '0' + 28];

		}
	}
	current->isEnd = true;
	current->definition.push_back(definition);

}
EVTrie* EV::findWord(EVTrie* root, string& word)
{
	word = toLowerCase(word);
	EVTrie* current = root;
	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return 0;
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return 0;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return 0;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return 0;
			current = current->children[c - '0' + 28];
		}
	}
	if (!current->isEnd) return 0;
	return current;
}
bool EV::findWordMeaning(EVTrie* root, string word, vector<wstring>& meaning,EVTrie*&node)
{
	node = EV::findWord(root, word);
	if (node == 0) return false;
	meaning = node->definition;
	return true;
}
void EV::getWordByIndex(EVTrie* curNode, int& index, string& currentWord, string& resultWord, wstring& resultDefinition)
{
	if (curNode == nullptr)
		return;
	if (curNode->definition.size() != 0)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultDefinition = curNode->definition[0];
			return;
		}
		index--;
	}
	for (int i = 0; i < 38; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			char tempChar = 'a' + i;
			currentWord.push_back(tempChar);
			getWordByIndex(curNode->children[i], index, currentWord, resultWord, resultDefinition);
			currentWord.pop_back();
			if (!resultWord.empty())
				return;
		}
	}
}
void EV::randomAWordAnd4Definitions(EVTrie* root, string& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 113477);
	int randomIndex;
	string currentWord;

	// get right word
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong definition 1
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong definition 2
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EV::randomADefinitionAnd4Words(EVTrie* root, wstring& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 113477);
	int randomIndex;
	string currentWord;

	// get right definition
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong word 1
	randomIndex = dis(gen);
	currentWord = "";
	wstring wrongDefinition1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong word 2
	randomIndex = dis(gen);
	currentWord = "";
	wstring wrongDefinition2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	wstring wrongDefinition3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EV::helperDeleteAWord(EVTrie* root, string& word) {

	EVTrie* current = root;
	EVTrie* parent = nullptr;
	int childIndex = -1;

	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return;
			parent = current;
			childIndex = c - 'a';
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return;
			parent = current;
			childIndex = 26;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return;
			parent = current;
			childIndex = 27;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return;
			parent = current;
			childIndex = c - '0' + 28;
			current = current->children[c - '0' + 28];
		}
	}
	if (parent != nullptr)
	{
		delete current;
		parent->children[childIndex] = nullptr;
		parent->numChildren--;
	}
}
bool EV::deleteAWord(EVTrie* root, string& word) {
	EVTrie* node = EV::findWord(root, word);
	if (node == 0) return false;
	// if the node is leaf node, we have to call a helper function to delete it from its parent's children array
	if (node->numChildren == 0)
	{
		void EV::helperDeleteAWord(EVTrie * root, string & word);
	}
	else
	{
		node->definition.clear();
		node->isEnd = false;
	}
	return true;
}
void EV::deleteTrie(EVTrie* root)
{
	if (root == nullptr) return;
	for (EVTrie* child : root->children)
	{
		EV::deleteTrie(child);
	}
	delete root;
}

// Eng-Eng trie functions
bool EE::loadRawData(EETrie*& root, string path)
{
	ifstream fin;
	fin.open(path);
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
		EE::insertWord(root, word, definition);
	}
	return true;
}
void EE::loadTrie(EETrie*& root, ifstream& fin)
{
	short int numDefinitions;
	fin.read((char*)&numDefinitions, sizeof(short int));
	if (numDefinitions != 0) root->isend = true;
	for (int i = 0; i < numDefinitions; ++i)
	{
		short int len;
		fin.read((char*)&len, sizeof(short int));
		char* tmp = new char[len + 1];
		fin.read(tmp, len);
		tmp[len] = '\0';
		root->definition.push_back(tmp);
		delete[] tmp;
	}
	fin.read((char*)&root->numChildren, sizeof(short int));
	// load children which from 'a' to 'z', space, hyphen, '0' to '9'
	for (int i = 0; i < root->numChildren; ++i)
	{
		char c;
		fin.read(&c, sizeof(char));
		if (c >= 'a' && c <= 'z')
		{
			root->children[c - 'a'] = new EETrie();
			loadTrie(root->children[c - 'a'], fin);
		}
		else if (c == ' ')
		{
			root->children[26] = new EETrie();
			loadTrie(root->children[26], fin);
		}
		else if (c == '-')
		{
			root->children[27] = new EETrie();
			loadTrie(root->children[27], fin);
		}
		else if (c >= '0' && c <= '9')
		{
			root->children[c - '0' + 28] = new EETrie();
			loadTrie(root->children[c - '0' + 28], fin);
		}
	}
}
bool EE::loadTrieFromFile(EETrie*& root, string path) {
	ifstream fin;
	fin.open(path, ios::binary);

	if (!fin.is_open())
	{
		return false;
	}
	loadTrie(root, fin);
	fin.close();
	return true;
}
void EE::saveTrie(EETrie* root, ofstream& fout)
{
	short int numDefinitions = root->definition.size();
	fout.write((char*)&numDefinitions, sizeof(short int));
	for (string& str : root->definition)
	{
		short int len = str.length();
		fout.write((char*)&len, sizeof(short int));
		fout.write(str.c_str(), len);
	}
	short int numChildren = root->numChildren;
	fout.write((char*)&numChildren, sizeof(short int));
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
void EE::saveTrietoFile(EETrie* root, string path) {
	ofstream fou;
	fou.open(path, ios::binary);
	saveTrie(root, fou);
	fou.close();
}
void EE::insertWord(EETrie*& root, string& word, string& definition)
{
	word = toLowerCase(word);
	word = removeSpecialCharacters(word);
	EETrie* current = root;
	for (char c : word)
	{
		// a to z is 0 to 25
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr)
			{
				current->children[c - 'a'] = new EETrie;
				current->numChildren++;
			}
			current = current->children[c - 'a'];
		}
		// space is 26
		else if (c == ' ')
		{
			if (current->children[26] == nullptr)
			{
				current->children[26] = new EETrie;
				current->numChildren++;
			}
			current = current->children[26];
		}
		// hyphen is 27
		else if (c == '-')
		{
			if (current->children[27] == nullptr)
			{
				current->children[27] = new EETrie;
				current->numChildren++;
			}
			current = current->children[27];
		}
		// 0 to 9 is 28 to 37
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr)
			{
				current->children[c - '0' + 28] = new EETrie;
				current->numChildren++;
			}
			current = current->children[c - '0' + 28];
		}
	}
	current->isend = true;
	current->definition.push_back(definition);
}
EETrie* EE::findWord(EETrie* root, string& word)
{
	word = toLowerCase(word);
	EETrie* current = root;
	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return 0;
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return 0;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return 0;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return 0;
			current = current->children[c - '0' + 28];
		}
	}
	if (!current->isend) return 0;
	return current;
}
bool EE::findWordMeaning(EETrie* root, string word, vector<string>& meaning)
{
	EETrie* node = findWord(root, word);
	if (node == 0) return false;
	meaning = node->definition;
	return true;
}
bool EE::changeWordDefinition(EETrie* root, string& word, string& newDefinition, int indexOfOldDefinitionToBeReplaced)
{
	EETrie* node = EE::findWord(root, word);
	if (node == 0) return false;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return false;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;
	return true;

}
void EE::getWordByIndex(EETrie* curNode, int& index, string& currentWord, string& resultWord, string& resultDefinition)
{
	if (curNode == nullptr)
		return;
	if (curNode->isend)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultDefinition = curNode->definition[0];
			return;
		}
		index--;
	}
	for (int i = 0; i < 38; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			char tempChar = 'a' + i;
			currentWord.push_back(tempChar);
			getWordByIndex(curNode->children[i], index, currentWord, resultWord, resultDefinition);
			currentWord.pop_back();
			if (!resultWord.empty())
				return;
		}
	}
}
void EE::randomAWordAnd4Definitions(EETrie* root, string& rightWord, string& rightDefinition, string& wrongDefinition1, string& wrongDefinition2, string& wrongDefinition3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 113477);
	int randomIndex;
	string currentWord;

	// get right word
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong definition 1
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong definition 2
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	string wrongWord3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EE::randomADefinitionAnd4Words(EETrie* root, string& rightDefinition, string& rightWord, string& wrongWord1, string& wrongWord2, string& wrongWord3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 113477);
	int randomIndex;
	string currentWord;

	// get right definition
	randomIndex = dis(gen);
	currentWord = "";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong word 1
	randomIndex = dis(gen);
	currentWord = "";
	string wrongDefinition1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong word 2
	randomIndex = dis(gen);
	currentWord = "";
	string wrongDefinition2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = "";
	string wrongDefinition3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void EE::helperDeleteAWord(EETrie* root, string& word)
{
	EETrie* current = root;
	EETrie* parent = nullptr;
	int childIndex = -1;

	for (char c : word)
	{
		if (c >= 'a' && c <= 'z')
		{
			if (current->children[c - 'a'] == nullptr) return;
			parent = current;
			childIndex = c - 'a';
			current = current->children[c - 'a'];
		}
		else if (c == ' ')
		{
			if (current->children[26] == nullptr) return;
			parent = current;
			childIndex = 26;
			current = current->children[26];
		}
		else if (c == '-')
		{
			if (current->children[27] == nullptr) return;
			parent = current;
			childIndex = 27;
			current = current->children[27];
		}
		else if (c >= '0' && c <= '9')
		{
			if (current->children[c - '0' + 28] == nullptr) return;
			parent = current;
			childIndex = c - '0' + 28;
			current = current->children[c - '0' + 28];
		}
	}
	if (parent != nullptr)
	{
		delete current;
		parent->children[childIndex] = nullptr;
		parent->numChildren--;
	}
}
bool EE::deleteAWord(EETrie* root, string& word)
{
	EETrie* node = EE::findWord(root, word);
	if (node == 0) return false;
	// if the node is leaf node, we have to call a helper function to delete it from its parent's children array
	if (node->numChildren == 0)
	{
		void helperDeleteAWord(EETrie * root, string & word);
	}
	else
	{
		node->definition.clear();
		node->isend = false;
	}
	return true;
}
void EE::deleteTrie(EETrie*& root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 38; ++i)
	{
		deleteTrie(root->children[i]);
	}
	delete root;
	root = nullptr;
}

// Viet-Eng trie functions
short int map[7930];
short int reverseMap[91];
void VE::fillMap()
{
	memset(map, -1, 7929 * sizeof(short int));
	wstring word = L"a á à ả ã ạ ă ắ ằ ẳ ẵ ặ â ấ ầ ẩ ẫ ậ b c d đ e é è ẻ ẽ ẹ ê ế ề ể ễ ệ g h i í ì ỉ ĩ ị k l m n o ó ò ỏ õ ọ ô ố ồ ổ ỗ ộ ơ ớ ờ ở ỡ ợ p q r s t u ú ù ủ ũ ụ ư ứ ừ ử ữ ự v x y ý ỳ ỷ ỹ ỵ";
	wstring temp = L"";
	for (wchar_t& c : word) if (c != L' ') temp.push_back(c);
	int i = 0;
	for (wchar_t& c : temp)
	{
		map[c] = i;
		reverseMap[i] = c;
		i++;
	}
	map[L'-'] = 89;
	map[L' '] = 90;
	reverseMap[89] = L'-';
	reverseMap[90] = L' ';
}
wstring VE::VToLower(wstring& str)
{
	for (wchar_t& c : str)
	{
		if (c == L'-' || c == L' ' || c > 7929) continue;
		else if (c >= 65 && c <= 89) c += 32;
		else if (map[c] == -1)
		{
			wchar_t temp1 = c + 1;
			wchar_t temp2 = c + 32;
			if (map[temp1] != -1) c = temp1;
			else if (map[temp2] != -1) c = temp2;
		}

	}
	return str;
}
bool VE::loadRawData(VTrie*& root, string path)
{
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin(path);
	fin.imbue(loc);
	if (!fin.is_open())
	{
		wcout << L"Không tìm thấy dữ liệu\n";
		return false;
	}
	else
	{
		wstring word, definition, wordType;
		getline(fin, word);
		while (getline(fin, word, L','))
		{
			getline(fin, wordType, L',');
			getline(fin, definition, L'\n');
			insertWord(root, word, definition);
		}
		return true;
	}
}
void VE::loadTrie(VTrie*& root, wifstream& fin)
{
	short int numDefinitions;
	fin.read((wchar_t*)&numDefinitions, sizeof(short int));
	for (int i = 0; i < numDefinitions; ++i)
	{
		short int len;
		fin.read((wchar_t*)&len, sizeof(short int));
		wchar_t* tmp = new wchar_t[len + 1];
		fin.read((wchar_t*)tmp, len);
		tmp[len] = L'\0';
		root->definition.push_back(tmp);
		delete[] tmp;
	}
	fin.read((wchar_t*)&root->numChildren, sizeof(short int));
	for (int i = 0; i < root->numChildren; ++i)
	{
		wchar_t c;
		fin.read((wchar_t*)&c, sizeof(wchar_t));
		if (map[c] != -1)
		{
			root->children[map[c]] = new VTrie();
			loadTrie(root->children[map[c]], fin);
		}
	}
}
bool VE::loadTrieFromFile(VTrie*& root, string path) {
	wifstream fin;
	fin.open(path, ios::binary);
	// Ensure the file is opened with UTF-8 encoding
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
	if (!fin.is_open())
	{
		return false;
	}
	loadTrie(root, fin);
	fin.close();
	return true;
}
void VE::saveTrie(VTrie* root, wofstream& fout)
{
	short int numDefinitions = root->definition.size();
	fout.write((wchar_t*)&numDefinitions, sizeof(short int));
	for (wstring& str : root->definition)
	{
		short int len = str.length();
		// write length of the string
		fout.write((wchar_t*)&len, sizeof(short int));
		// write the string itself
		fout.write((wchar_t*)str.c_str(), len);
	}
	short int numChildren = root->numChildren;
	fout.write((wchar_t*)&numChildren, sizeof(short int));
	for (int i = 0; i < 91; ++i)
	{
		if (root->children[i] != nullptr)
		{
			wchar_t c = reverseMap[i];
			fout.write((wchar_t*)&c, sizeof(wchar_t));
			saveTrie(root->children[i], fout);
		}
	}
}
void VE::saveTrieToFile(VTrie* root, string path) {
	wofstream fou;
	fou.open(path, ios::binary);
	// Ensure the file is opened with UTF-8 encoding
	fou.imbue(locale(fou.getloc(), new codecvt_utf8<wchar_t>));
	saveTrie(root, fou);
	fou.close();
}
void VE::insertWord(VTrie*& root, wstring& word, wstring& definition)
{
	word = VToLower(word);
	VTrie* current = root;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) continue;
		if (current->children[map[c]] == nullptr)
		{
			current->children[map[c]] = new VTrie();
			current->numChildren++;
		}
		current = current->children[map[c]];
	}
	current->definition.push_back(definition);
}
VTrie* VE::findWord(VTrie* root, wstring& word)
{
	word = VToLower(word);
	VTrie* current = root;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) return nullptr;
		if (current->children[map[c]] == nullptr) return nullptr;
		current = current->children[map[c]];
	}
	if (current->definition.empty()) return nullptr;
	return current;
}
bool VE::findWordMeaning(VTrie* root, wstring& word, vector<wstring>& meaning, VTrie*& node)
{

	node = VE::findWord(root, word);
	if (node == nullptr) return false;
	meaning = node->definition;
	return true;
}
bool VE::changeWordDefinition(VTrie* root, wstring& word, wstring& newDefinition, int indexOfOldDefinitionToBeReplaced)
{
	VTrie* node = VE::findWord(root, word);
	if (node == nullptr) return false;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return false;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;
	return true;
}
void VE::getWordByIndex(VTrie* curNode, int& index, wstring& currentWord, wstring& resultWord, wstring& resultDefinition)
{
	if (curNode == nullptr)
		return;
	if (curNode->definition.size() != 0)
	{
		if (index == 0)
		{
			resultWord = currentWord;
			resultDefinition = curNode->definition[0];
			return;
		}
		index--;
	}
	for (int i = 0; i < 91; ++i)
	{
		if (curNode->children[i] != nullptr)
		{
			wchar_t tempChar = reverseMap[i];
			currentWord.push_back(tempChar);
			getWordByIndex(curNode->children[i], index, currentWord, resultWord, resultDefinition);
			currentWord.pop_back();
			if (!resultWord.empty())
				return;
		}
	}
}
void VE::randomAWordAnd4Definitions(VTrie* root, wstring& rightWord, wstring& rightDefinition, wstring& wrongDefinition1, wstring& wrongDefinition2, wstring& wrongDefinition3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 145921);
	int randomIndex;
	wstring currentWord;

	// get right word
	randomIndex = dis(gen);
	currentWord = L"";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong definition 1
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongWord1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong definition 2
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongWord2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongWord3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void VE::randomADefinitionAnd4Words(VTrie* root, wstring& rightDefinition, wstring& rightWord, wstring& wrongWord1, wstring& wrongWord2, wstring& wrongWord3)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 145921);
	int randomIndex;
	wstring currentWord;

	// get right definition
	randomIndex = dis(gen);
	currentWord = L"";
	getWordByIndex(root, randomIndex, currentWord, rightWord, rightDefinition);
	// get wrong word 1
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongDefinition1;
	getWordByIndex(root, randomIndex, currentWord, wrongWord1, wrongDefinition1);
	// get wrong word 2
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongDefinition2;
	getWordByIndex(root, randomIndex, currentWord, wrongWord2, wrongDefinition2);
	// get wrong definition 3
	randomIndex = dis(gen);
	currentWord = L"";
	wstring wrongDefinition3;
	getWordByIndex(root, randomIndex, currentWord, wrongWord3, wrongDefinition3);
}
void VE::helperDeleteAWord(VTrie* root, wstring& word)
{
	VTrie* current = root;
	VTrie* parent = nullptr;
	int childIndex = -1;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) return;
		if (current->children[map[c]] == nullptr) return;
		parent = current;
		childIndex = map[c];
		current = current->children[map[c]];
	}
	if (parent != nullptr)
	{
		delete current;
		parent->children[childIndex] = nullptr;
		parent->numChildren--;
	}
}
bool VE::deleteAWord(VTrie * root, wstring & word)
{
	VTrie* node = VE::findWord(root, word);
	if (node == nullptr) return false;
	if (node->numChildren == 0)
	{
		VE::helperDeleteAWord(root, word);
	}
	else
	{
		node->definition.clear();
	}
	return true;
}
void VE::deleteTrie(VTrie*& root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 91; ++i)
	{
		VE::deleteTrie(root->children[i]);
	}
	delete root;
	root = nullptr;
}
