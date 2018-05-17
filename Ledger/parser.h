#ifndef _PARSER_H_
#define _PARSER_H_
#include <fstream>
#include "header.h"
#include "record.h"
#include <map>
#include <openssl/sha.h>

/* Parser for reading file input and creating a list of all the valid records (in the form of a linked list)*/

// To Do return head of the list
record* parseFile(string fileName, record* head, map<string, vector<pair<string, long int>>> &transMap, bool verboseFlag, map<string, record> &recordMap);

// Utility function to check if the given transaction record is syntactically valid
record* isTransactionStringValid(string line, record* head, map<string, vector<pair<string, long int>>> transMap, bool verboseFlag);
#endif // !_PARSER_H_

