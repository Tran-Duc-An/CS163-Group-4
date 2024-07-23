#include"function.h"
#include<iostream>
#include<vector>
#include<string>
#include<chrono>
#include<sstream>
#include<fstream>
using namespace std;

int main() {
    
    vector<pair<string, string>> table;
    auto start = chrono::high_resolution_clock::now();
    loadDataset(table);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;
    string def;
    cout << "Enter Definition: ";
    getline(cin, def);
    def = toLowerCase(def);
    vector<string> word=searchByDef(table,def);
   
    for (auto str : word)
        cout << str << endl;
}
