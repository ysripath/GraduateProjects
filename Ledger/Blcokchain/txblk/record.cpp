#include "record.h"

int record::recordNumber = 1;

record::record(string lTransId, long int lM, vector<pair<string, long int>> lvOut, long int lN, vector<pair<string, long int>> lop):
	transId(lTransId),
	M(lM),
	vOut(lvOut),
	N(lN),
	op(lop),
	validRecord(false),
	includedRecord(false),
	neglect(false),
    inBlock(false),
    validated(false),
	next(NULL),
	signature(NULL)
{
	recordCount = recordNumber++;
	for (int i = 0; i < N; i++)
	{
		spentMap.insert(pair<int, bool>(i, false));
	}
}

record::record(string lTransId, long int lM, vector<pair<string, long int>> lvOut, long int lN, vector<pair<string, long int>> lop, string sign) :
	transId(lTransId),
	M(lM),
	vOut(lvOut),
	N(lN),
	op(lop),
	validRecord(false),
	includedRecord(false),
	neglect(false),
    inBlock(false),
    validated(false),
    next(NULL),
    signature(sign)
{
	recordCount = recordNumber++;
	for (int i = 0; i < N; i++)
	{
		spentMap.insert(pair<int, bool>(i, false));
	}

    // Fix new lines
    size_t start_pos = 0;
    while((start_pos = signature.find("\\n", start_pos)) != std::string::npos) {
         signature.replace(start_pos, 2, "\n");
         start_pos += 1;
    }
}
