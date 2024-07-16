//#include"function.h"
//int main() {
//    const size_t tableSize = 101;
//
//    //// Load the dataset
//    //auto start = chrono::high_resolution_clock::now();
//    //HashTable dictionary = loadDataset("Dataset/englishDictionary2.csv", tableSize);
//    //auto end = chrono::high_resolution_clock::now();
//    //chrono::duration<double> duration = end - start;
//    //cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;
//    //
//    //saveHashtable(dictionary);
//
//    HashTable newDict;
//
//    // Load the dataset
//    auto start = chrono::high_resolution_clock::now();
//    newDict = loadHashTable("Dataset//Hashtable.bin");
//    auto end = chrono::high_resolution_clock::now();
//    chrono::duration<double> duration = end - start;
//    cout << "Time taken to load dataset: " << duration.count() << " seconds" << endl;
//
//
//    // Ask the user for a definition to search
//    string definition;
//    cout << "Enter the definition to search for: ";
//    getline(cin, definition);
//
//    // find and display the corresponding word
//    start = chrono::high_resolution_clock::now();
//    string word = findWord(newDict, definition);
//    end = chrono::high_resolution_clock::now();
//    History(word, definition, "Dataset\\History.csv");
//    cout << "the word for the given definition is: " << word << endl;
//    duration = end - start;
//    cout << "time taken to search dataset: " << duration.count() << " seconds" << endl;
//
//    vector<Combination> test;
//    test=ReadHistory("Dataset\\History.csv");
//    for (int i = 0; i < test.size(); i++)
//    {
//        cout << test[i].word << "  " << test[i].definition << endl;
//    }
//
//    return 0;
//}
// 
// 
//#include "functions.h"
//#include "fstream"
//#include<iostream>
//#include<Windows.h>
//
//int main()
//{
//	SetConsoleOutputCP(CP_UTF8);
//	// setlocale(LC_ALL, "en_US.UTF-8");
//	std::cout << u8"\U0001F39F\UEFOE";
//	return 0;
//}
//


//
//Complete one but have some error for like "1F9D1 200D 2696", it is divided into a man and a scale not a court

// It means that we need to keep "1F9D1 200D 2696 FE0F" if there is and then delete "1F9D1 200D 2696"

#include "function.h"
#include "fstream"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include <windows.h>

// Function to convert a single Unicode code point to a UTF-8 string
std::string codePointToUtf8(char32_t codePoint) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
	return convert.to_bytes(codePoint);
}

// Function to convert a space-separated sequence of hex code points to a UTF-8 string
std::string hexCodePointSequenceToUtf8(const std::string& hexCodePointSequence) {
	std::istringstream iss(hexCodePointSequence);//F3223
	std::string hexCodePoint;
	std::string utf8str;

	while (iss >> hexCodePoint) {
		char32_t codePoint = std::stoul(hexCodePoint, nullptr, 16);
		utf8str += codePointToUtf8(codePoint);
	}

	return utf8str;
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	std::ifstream file("Dataset_emoji\\Fixed_filtered.csv");
	if (!file.is_open()) {
		std::cerr << "failed to open file" << std::endl;
		return 1;
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

		// output the original and the converted utf-8 string
		std::cout << "original: " << codepoint << " => utf-8: " << utf8str << std::endl;
	}

	file.close();
	return 0;
}
//
//
//For 1 specific code
// 
//#include "function.h"
//#include "fstream"
//#include <iostream>
//#include <sstream>
//#include <vector>
//#include <locale>
//#include <codecvt>
//#include <string>
//#include<Windows.h>
//std::string hexCodePointSequenceToUtf8(const std::string& hexCodePointSequence) {
//	// Split the hex code point sequence into individual hex code points
//	std::istringstream iss(hexCodePointSequence);
//	std::string hexCodePoint;
//	std::vector<char32_t> codePoints;
//
//	while (iss >> hexCodePoint) {
//		// Convert each hex string to integer and add to the codePoints vector
//		char32_t codePoint = std::stoul(hexCodePoint, nullptr, 16);
//		codePoints.push_back(codePoint);
//	}
//
//	// Convert the vector of char32_t to std::u32string
//	std::u32string u32str(codePoints.begin(), codePoints.end());
//
//	// Convert std::u32string to std::string (UTF-8)
//	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
//	return convert.to_bytes(u32str);
//}
//
//int main() {
//	SetConsoleOutputCP(CP_UTF8);
//	/*std::string hexCodePointSequence = "1F9D1 1F3FB FE0F 200D 2764 FE0F 200D 1F9D1 1F3FC FE0F";
//	std::string utf8Str = hexCodePointSequenceToUtf8(hexCodePointSequence);
//	std::cout << utf8Str << std::endl;*/
//	cout << u8"F09FA791F09F8FBBE2808DE29DA4EFB88FE2808DF09F928BE2808DF09FA791F09F8FBC";
//	return 0;
//}
//
//


//////Filter
//#include "function.h"
//#include "fstream"
//#include <fstream>
//#include <iostream>
//#include <string>
//#include<sstream>
//using namespace std;
//
//bool checkFE0F(string a)
//{
//	int length = a.size();
//	if (a[length - 1] == 'F' && a[length - 2] == '0' && a[length - 3] == 'E' && a[length - 4] == 'F')
//		return true;
//	else return false;
//}
//int main() {
//	// Input and output filenames
//	string inputFile = "Dataset_emoji\\emojis.csv"; // Replace with your actual input filename
//	string outputFile = "Dataset_emoji\\Fixed_filtered.csv"; // Replace with your desired output filename
//
//	// Open input and output files
//	ifstream inputStream(inputFile);
//	ofstream outputStream(outputFile);
//
//	// Check if files are open
//	if (!inputStream.is_open()) {
//		cerr << "Error opening input file: " << inputFile << endl;
//		return 1;
//	}
//	if (!outputStream.is_open()) {
//		cerr << "Error opening output file: " << outputFile << endl;
//		return 1;
//	}
//
//	// Write header row to output file (assuming it exists in the input file)
//		string line;
//
//		std::getline(inputStream, line);
//		outputStream << line << endl;
//
//		string group, subgroup, codepoint, status, representation, name, section;
//
//	// Read data line by line
//	while (getline(inputStream, line)) {
//		std::stringstream ss(line);
//		getline(ss, group, ',');
//		getline(ss, subgroup, ',');
//		getline(ss, codepoint, ',');
//		getline(ss, status, ',');
//		getline(ss, representation, ',');
//		getline(ss, name, ',');
//		getline(ss, section, ',');
//
//		// Check if current line's codepoint ends with "FE0F"
//		bool FE0F = checkFE0F(codepoint);
//
//		// Write current line
//		outputStream << group << ',' << subgroup << ',' << codepoint << ',' << status << ',' << representation << ',' << name << ',' << section << endl;
//
//	
//		if (FE0F) {
//			getline(inputStream, line);
//			std::stringstream ss(line);
//			getline(ss, group, ',');
//			getline(ss, subgroup, ',');
//			getline(ss, codepoint, ',');
//			getline(ss, status, ',');
//			getline(ss, representation, ',');
//			getline(ss, name, ',');
//			getline(ss, section, ',');
//
//			bool FE0F2 = checkFE0F(codepoint);
//			if(FE0F2) outputStream << group << ',' << subgroup << ',' << codepoint << ',' << status << ',' << representation << ',' << name << ',' << section << endl;
//		}
//	}
//
//	// Close files
//	inputStream.close();
//	outputStream.close();
//
//	cout << "Filtered data written to: " << outputFile << endl;
//
//	return 0;
//}
//
//

