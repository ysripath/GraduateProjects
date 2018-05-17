#ifndef _RECORD_H_
#define _RECORD_H_
#include <vector>
#include <string>
#include <map>

using namespace std;

class record
{
public:
	static int recordNumber;

	// Convert to hex
	int recordCount;
	string transId;
	long int M;
	vector<pair<string, long int>> vOut;
	long int N;
	vector<pair<string, long int>> op;
	bool validRecord;
	bool includedRecord;
	record* next;
	map<int, bool> spentMap;
	bool neglect;
	record(string transId, long int M, vector<pair<string, long int>> vOut, long int N, vector<pair<string, long int>> op);
};



#endif // !_RECORD_H_

