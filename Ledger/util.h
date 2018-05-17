#ifndef _UTIL_H_
#define _UTIL_H_
#include "header.h"
#include "record.h"
#include <map>

#include <vector>

// Check and convert choice to corresponding option
char choiceUtil(string str);


// Utility function to iterate through a vector
void iterateVecUtil(vector<pair<string, long int>> &ptr);

void printNodeToFileUtil(fstream &file, record* node);

// Utility function to print the record node
void printNodeUtil(record* node);

// Utility function to print LL
void printListUtil(record* head);


// Utility function to print the balances of all the account holders
void printBalance(map<string, long int> balanceMap);
#endif // !_UTIL_H_
