#include "functions.h"



string toLowerCase(string& str) 
{
	for (int i = 0; i < str.length(); i++) 
	{
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}
	return str;
}
// remove all letters other than 26 english alphabets, 0 to 9, space, hyphen
string removeSpecialCharacters(string& str) 
{
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
trie* findWord(trie* root, string& word)
{
	word = toLowerCase(word);
	trie* current = root;
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

bool findWordMeaning(trie* root, string& word, vector<string>& meaning)
{
	trie* node = findWord(root, word);
	if (node == 0) return false;
	meaning = node->definition;
	return true;
}


// save trie but use short int instead of int
void saveTrie(trie* root, ofstream& fout)
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

// load trie but use short int instead of int
void loadTrie(trie*& root, ifstream& fin)
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

void deleteTrie(trie*& root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 38; ++i)
	{
		deleteTrie(root->children[i]);
	}
	delete root;
	root = nullptr;
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

bool changeWordDefinition(trie* root, string& word, string& newDefinition, int indexOfOldDefinitionToBeReplaced)
{
		trie* node = findWord(root, word);
	if (node == 0) return false;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return false;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;
	return true;

}
// helper function to delete a word from its parent's children array
// For example, if we want to delete the word "an", the node "n" is the leaf node, we have to delete it from its parent's children array
void helperDeleteAWord(trie* root, string& word)
{
	trie* current = root;
	trie* parent = nullptr;
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

bool deleteAWord(trie* root, string& word)
{
	trie* node = findWord(root, word);
	if (node == 0) return false;
	// if the node is leaf node, we have to call a helper function to delete it from its parent's children array
	if (node->numChildren == 0)
	{
		void helperDeleteAWord(trie * root, string& word);
	}
	else
	{
		node->definition.clear();
		node->isend = false;
	}
	return true;
}



short int map[7930];
short int reverseMap[91];
void fillMap()
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
wstring VToLower(wstring& str)
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
void VInsertWord(Vtrie*&root, wstring& word,wstring& definition)
{
	word = VToLower(word);
	Vtrie* current = root;
	for (wchar_t&c : word)
	{
		if ((int)c > 7929 || map[c] == -1) continue;
		if (current->children[map[c]] == nullptr)
		{
			current->children[map[c]] = new Vtrie();
			current->numChildren++;
		}
		current = current->children[map[c]];
	}
	current->definition.push_back(definition);
}

Vtrie* VFindWord(Vtrie* root, wstring& word)
{
	word = VToLower(word);
	Vtrie* current = root;
	for (wchar_t& c : word)
	{
		if ((int)c > 7929 || map[c] == -1) return nullptr;
		if (current->children[map[c]] == nullptr) return nullptr;
		current = current->children[map[c]];
	}
	if (current->definition.empty()) return nullptr;
	return current;
}
bool VFindWordMeaning(Vtrie* root, wstring& word, vector<wstring>& meaning)
{
	Vtrie* node = VFindWord(root, word);
	if (node == nullptr) return false;
	meaning = node->definition;
	return true;
}

void VDeleteTrie(Vtrie*& root)
{
	if (root == nullptr) return;
	for (int i = 0; i < 91; ++i)
	{
		VDeleteTrie(root->children[i]);
	}
	delete root;
	root = nullptr;
}

bool VloadRawData(Vtrie*& root)
{
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin("Dataset\\VE.csv");
	fin.imbue(loc);
	if (!fin.is_open())
	{
		wcout << L"File not found\n";
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
			VInsertWord(root, word, definition);
		}
		return true;
	}
}

void saveVtrie(Vtrie* root, wofstream& fout)
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
			saveVtrie(root->children[i], fout);
		}
	}
}

void loadVtrie(Vtrie*& root, wifstream& fin)
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
			root->children[map[c]] = new Vtrie();
			loadVtrie(root->children[map[c]], fin);
		}
	}
}

void VHelperDeleteAWord(Vtrie* root, wstring& word)
{
	Vtrie* current = root;
	Vtrie* parent = nullptr;
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

bool VDeleteAWord(Vtrie* root, wstring& word)
{
	Vtrie* node = VFindWord(root, word);
	if (node == nullptr) return false;
	if (node->numChildren == 0)
	{
		VHelperDeleteAWord(root, word);
	}
	else
	{
		node->definition.clear();
	}
	return true;
}

bool VChangeWordDefinition(Vtrie* root, wstring& word, wstring& newDefinition, int indexOfOldDefinitionToBeReplaced)
{
	Vtrie* node = VFindWord(root, word);
	if (node == nullptr) return false;
	if (indexOfOldDefinitionToBeReplaced >= node->definition.size()) return false;
	node->definition[indexOfOldDefinitionToBeReplaced] = newDefinition;
	return true;
}