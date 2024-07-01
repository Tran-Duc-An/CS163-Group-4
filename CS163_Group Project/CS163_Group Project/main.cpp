#include "function.h"
#include <chrono>

int main() {
    const size_t tableSize = 101;

    // Load the dataset
    auto start = chrono::high_resolution_clock::now();
    HashTable dictionary = loadDataset("Dataset/englishDictionary2.csv", tableSize);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;

    // Ask the user for a definition to search
    string definition;
    cout << "Enter the definition to search for: ";
    getline(cin, definition);

    // Find and display the corresponding word
    start = chrono::high_resolution_clock::now();
    string word = findWord(dictionary, definition);
    end = chrono::high_resolution_clock::now();
    cout << "The word for the given definition is: " << word << endl;
    duration = end - start;
    cout << "Time taken to search dataset: " << duration.count() << " seconds" << endl;

    return 0;
}
