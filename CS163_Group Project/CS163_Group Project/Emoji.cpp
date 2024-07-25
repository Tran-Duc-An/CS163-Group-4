#include"Emoji.h"

std::string hexCodePointSequenceToUtf8(const std::string& hexCodePointSequence) {
    std::istringstream iss(hexCodePointSequence);
    std::string hexCodePoint;
    std::vector<char32_t> codePoints;

    while (iss >> hexCodePoint) {
        char32_t codePoint = std::stoul(hexCodePoint, nullptr, 16);
        codePoints.push_back(codePoint);
    }

    std::u32string u32str(codePoints.begin(), codePoints.end());
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(u32str);
}
#include <vector>
#include <string>

bool checkSubstring(const std::string& s, const std::string& x) {//KMP x in s
    int m = x.length();
    int n = s.length();

    if (n < m) {
        return false;
    }

    // PREPROCESSING
    // longest_border array
    std::vector<int> longest_border(m, 0);
    // Length of Longest Border for prefix before it.
    int len = 0;
    // Iterating from index-1. longest_border[0] will always be 0
    int i = 1;

    while (i < m) {
        if (x[i] == x[len]) {
            // Length of Longest Border Increased
            len += 1;
            longest_border[i] = len;
            i += 1;
        }
        else {
            // Only empty border exists
            if (len == 0) {
                longest_border[i] = 0;
                i += 1;
            }
            else {
                // Try finding longest border for this i with reduced len
                len = longest_border[len - 1];
            }
        }
    }

    int s_Pointer = 0;
    int x_Pointer = 0;

    while (s_Pointer < n) {
        if (s[s_Pointer] == x[x_Pointer]) {
            // Matched, Increment Both
            x_Pointer += 1;
            s_Pointer += 1;
            // All characters matched
            if (x_Pointer == m) {
                return true;
            }
        }
        else {
            if (x_Pointer == 0) {
                // Zero Matched
                s_Pointer += 1;
            }
            else {
                x_Pointer = longest_border[x_Pointer - 1];
            }
        }
    }

    return false;
}

void addWordDefinition(vector<pair<string, string>>& da, const pair<string, string>& wd) {
    da.push_back(wd);
}

// Initialize the hash table
void initHashTable(Emo& ht, size_t tableSize) {
    ht.Emo_table.resize(tableSize);
    ht.size = tableSize;
}

// Hash function
size_t hashFunction(const string& key, size_t tableSize) {
    const size_t FNV_PRIME = 0x00000000811C9DC5;
    size_t hash = 0;
    for (char ch : key) {
        hash = hash * FNV_PRIME ^ ch;
    }
    return hash % tableSize;
}

// Insert word and definition into the hash table
void insertEmo(Emo& ht, const string& name, const string& utf8) {
    size_t index = hashFunction(utf8, ht.size);
    if (ht.Emo_table[index].size() == 0) ht.currentLoad += 1;
    addWordDefinition(ht.Emo_table[index], { name, utf8 });
}

// Find the word based on its definition
pair<string, string> findBycode(Emo& ht, string utf8) {
    utf8 = hexCodePointSequenceToUtf8(utf8);
    size_t index = hashFunction(utf8, ht.size);
    for (const auto& pair : ht.Emo_table[index]) {
        if (pair.second == utf8) {
            return pair;
        }
    }

}
string findByname(Emo& ht, const string& name, int& index, int& inside)
{
    int size = ht.Emo_table.size();
    for (; index < size; index++)
    {
        for (; inside < ht.Emo_table[index].size(); inside++) {
            pair<string, string> pair = ht.Emo_table[index][inside];
            if (checkSubstring(pair.first, name))
            {
                inside++;
                return pair.second;
            }
        }
        inside = 0;
    }
    return "";
}
vector<string> findbyNameUntil(Emo& ht, const string& name)
{
    int index = 0;
    int inside = 0;
    int size = ht.Emo_table.size();
    int count = 10000;
    vector<string> find;

    while (true)
    {
        if (index >= size)
        {
            cout << "End of search!";
            return find;
        }

        while (count--)
        {
            if (index >= size)
            {
                cout << "End of search!";
                return find;
            }
            string result = findByname(ht, name, index, inside);
            if (!result.empty()) {
                find.push_back(result);
            }
            else {
                break;
            }
        }
        for (auto str : find)
        {
            cout << str << endl;
        }
        cout << "If you want to find more, enter a number. If not, enter 0:";
        cin >> count;
        if (count == 0)
            return find;
    }

}
// Load the dataset into the hash table
Emo loadDataset(const string& filename, size_t tableSize) {
    Emo dictionary;
    initHashTable(dictionary, tableSize);
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return dictionary;
    }

    std::string line;
    // skip the header line
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string group, subgroup, codepoint, status, representation, name, section;

        std::getline(ss, group, ',');
        std::getline(ss, subgroup, ',');
        std::getline(ss, codepoint, ',');
        std::getline(ss, status, ',');
        std::getline(ss, representation, ',');
        std::getline(ss, name, ',');
        std::getline(ss, section, ',');

        // convert the code point to utf-8
        std::string utf8str = hexCodePointSequenceToUtf8(codepoint);

        insertEmo(dictionary, name, utf8str);
    }
    file.close();
    return dictionary;
}