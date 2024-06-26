#include "functions.h"
struct trie 
{
	vector<char> letters;
	vector<trie*>  children;
	bool isend = 0;
	string definition;
};
