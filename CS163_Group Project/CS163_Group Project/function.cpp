#include"function.h"
// Lower case for Definition, and 
string toLowerCase(string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
    }
    return str;
}
//void History(string word, string definition,string filename)
//{
//    fstream out;
//    out.open(filename, ios::out | ios::app);
//    if (!out.is_open() || word.empty() || definition.empty()) {
//        return;
//    }
//    out << word << "," << definition << endl;
//}
//vector<Combination> ReadHistory(string filename)
//{
//    fstream in;
//    in.open(filename, ios::in);
//    vector<Combination> res;
//    if (!in.is_open()) {
//        cerr << "Error: Could not open file 'Dataset\\History.csv'." << endl;
//        return res; // Return empty vector on error
//    }
//    string line;
//    while (getline(in, line)) {
//        stringstream ss(line);
//        string word, definition;
//        getline(ss, word, ',');
//        getline(ss, definition, ',');
//        Combination temp;
//        temp.definition = definition;
//        temp.word = word;
//        res.push_back(temp);
//    }
//    return res;
//}
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
void loadDataset(vector<pair<string, string>> &table)
{
    ifstream file("Dataset/englishDictionary2.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << "Dataset/englishDictionary2.csv" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        istringstream stream(line);
        string word, speech, definition;
        if (getline(stream, word, ',') && getline(stream, speech, ',') && getline(stream, definition)) {
            definition = toLowerCase(definition);
            pair<string, string>temp{ word,definition };
            table.push_back(temp);
        }
    }
    file.close();
}
vector<string> searchByDef(vector<pair<string, string>>& table, string def)
{
    
    vector<string> word;
    for (int i = 0; i < table.size(); i++)
    {
        if (checkSubstring(table[i].second, def))
            word.push_back(table[i].first);
    }
    return word;
}