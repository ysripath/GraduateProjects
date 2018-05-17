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
	next(NULL)
{
	recordCount = recordNumber++;
	for (int i = 0; i < N; i++)
	{
		spentMap.insert(pair<int, bool>(i, false));
	}
}
