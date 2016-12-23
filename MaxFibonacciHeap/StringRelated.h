#ifndef __STRINGRELATED_HEADER__
#define __STRINGRELATED_HEADER__

#include "Node.h"
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;


// Structure that is used for accumulating the required information from a parsed input string
struct utilStruct
{
	bool hashValue; // To denote presence of #
	int frequencyValue; // Frequency value associated with the #tag string
	int queryNumber; // If no #, then the number in the input string represents query number
	string data; // #tag string without #
};

// Utility function for parsing the input string and initialize the InputData structure and return the same
extern utilStruct checkHashTagPresence(string dataValue);

#endif // !__STRINGRELATED_HEADER__