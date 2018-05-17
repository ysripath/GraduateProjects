#include "util.h"

char choiceUtil(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
#ifdef DEBUG_LOG
	debugUtil("choiceUtil", "Option is " + str);
#endif // DEBUG_LOG
	if (str.length() > 1) {
		if (str.compare("FILE") == 0
			|| str.compare("TRANSACTION") == 0
			|| str.compare("PRINT") == 0
			|| str.compare("HELP") == 0
			|| str.compare("DUMP") == 0
			|| str.compare("WIPE") == 0
			|| str.compare("INTERACTIVE") == 0
			|| str.compare("VERBOSE") == 0
			|| str.compare("BALANCE") == 0
			|| str.compare("EXIT") == 0) {
			return str[0];
		}
		else {
			return '0'; // Invalid option
		}
	}
	else {
		return str[0];
	}
	return str[0];
}


// Utility function to iterate through a vector
void iterateVecUtil(vector<pair<string, long int>> &ptr)
{
	
	if (ptr.size() == 0)
		cout << "Empty vector" << endl;
	else {
		cout << "Vector contents" << endl;
		auto itr = ptr.begin();
		while (itr != ptr.end())
		{
			cout << itr->first << ", " << itr->second << endl;
			itr++;
		}
	}
	
}
// Utility function to print the record in file
void printNodeToFileUtil(fstream &fileHandle, record* node)
{
	fileHandle << node->transId << "; " << node->M << "; ";
	auto itr = node->vOut.begin();
	while (itr != node->vOut.end()) {
		fileHandle << "(" << itr->first << ", " << itr->second << ")";
		itr++;
	}
	fileHandle << "; ";
	fileHandle << node->N << "; ";
	auto itr2 = node->op.begin();
	while (itr2 != node->op.end()) {
		fileHandle << "(" << itr2->first << ", " << itr2->second << ")";
		itr2++;
	}
	fileHandle << "\n";
}


// Utility function to print the record node
void printNodeUtil(record* node)
{
	cout << node->transId << "; " << node->M << "; ";
	auto itr = node->vOut.begin();
	while (itr != node->vOut.end()) {
		cout << "(" << itr->first << ", " << itr->second << ")";
		itr++;
	}
	cout << "; ";
	cout << node->N << "; ";
	auto itr2 = node->op.begin();
	while (itr2 != node->op.end()) {
		cout << "(" << itr2->first << ", " << itr2->second << ")";
		itr2++;
	}
	cout << "\n";
}

// Utility function to print the Linked list
void printListUtil(record* head)
{
	if (head == NULL)
	{
#ifdef DEBUG_LOG
		debugUtil("printListUtil", "Record Linked List is empty");
#endif // DEBUG_LOG
		return;
	}
	while (head != NULL)
	{
		if (head->validRecord) {
			printNodeUtil(head);
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("printListUtil", "Invalid record, " + head->transId);
#endif // DEBUG_LOG
		}
		head = head->next;
	}
}


void printBalance(map<string, long int> balanceMap)
{
	auto itr = balanceMap.begin();
	while (itr != balanceMap.end())
	{
		cout << itr->first << ", " << itr->second << endl;
		itr++;
	}
}
