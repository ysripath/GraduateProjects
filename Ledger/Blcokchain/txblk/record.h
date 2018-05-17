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
	//txblk
    string signature;
    bool inBlock;
    bool validated;

	record(string transId, long int M, vector<pair<string, long int>> vOut, long int N, vector<pair<string, long int>> op);
    record(string transId, long int M, vector<pair<string, long int>> vOut, long int N, vector<pair<string, long int>> op, string sign);
};



#endif // !_RECORD_H_

