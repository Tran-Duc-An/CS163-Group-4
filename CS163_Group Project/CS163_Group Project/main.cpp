//#include"function.h"
//#include<iostream>
//#include<vector>
//#include<string>
//#include<chrono>
//#include<sstream>
//#include<fstream>
//using namespace std;
//
//int main() {
//    
//    vector<pair<string, string>> table;
//    auto start = chrono::high_resolution_clock::now();
//    loadDataset(table);
//    auto end = chrono::high_resolution_clock::now();
//    chrono::duration<double> duration = end - start;
//    cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;
//    string def;
//    cout << "Enter Definition: ";
//    getline(cin, def);
//    def = toLowerCase(def);
//    vector<string> word=searchByDef(table,def);
//   
//    for (auto str : word)
//        cout << str << endl;
//}


#include"Emoji.h"
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    const size_t tableSize(101);
    auto start = chrono::high_resolution_clock::now();
    Emo ht = loadDataset("Dataset_emoji\\emojis.csv", tableSize);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;
    // Ask the user for a definition to search
    string name;
    cout << "Enter the definition to search for: ";
    getline(cin, name);
    vector<string> utf8 = findbyNameUntil(ht, name);

}