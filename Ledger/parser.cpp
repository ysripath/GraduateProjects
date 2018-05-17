#include "parser.h"
#include <vector>
#include "util.h"

bool lVerboseFlag;

bool checkTokens_vout(int count, string str, bool opFlag, vector<pair<string, long int>> &ptr)
{
	int n = 0;
	int l = str.length();
	bool openP = false;
	char buf[1000] = { '/0' };
	//sprintf_s(buf, "%s", str.c_str());
	sprintf(buf, "%s", str.c_str());
	if (buf[0] != '(')
		return false;
	int itr = 0;
	bool closeP = false;
	bool commaFlag = false;
	bool commaPresent = false;
	int identL = 0;

	// For vector
	char bufV[1000] = { '/0' };
	char bufV2[1000] = { '/0' };
	int itrV = 0;
	while (itr != l)
	{
		
		if (commaFlag) {
			commaPresent = true;
			int lItr = itr;
			while (buf[lItr] != ')' && lItr <= l) {
				if (!isdigit(buf[lItr])) {
#ifdef DEBUG_LOG
					debugUtil("checkTokens_vout", "vout index is not a number, " + str);
#endif // DEBUG_LOG
					return false;

				}
				bufV2[itrV++] = buf[lItr];
				lItr++;
			}
			if (lItr > l) {
#ifdef DEBUG_LOG
				debugUtil("checkTokens_vout", "no ) found " + str);
#endif // DEBUG_LOG
				return false;
			}else if (buf[lItr] == ')') {
				// TO check if there is a , between the identifier/name and the numerical
				if (commaPresent == false) {
#ifdef DEBUG_LOG
					debugUtil("checkTokens_vout", "Malformed transaction record, no \",\" found in the pair"+str);
#endif // DEBUG_LOG
					cout << "Malformed transaction record, no \",\" found in the pair" << str << endl;
					return false;
				}
				commaFlag = false;
				itr = lItr;
				itrV = 0;
				continue;
			}
		}
		else {
			if (buf[itr] == '(' && openP) {
				cout<<"Malformed record - unbalanced paranthesis, "<<str<<endl;
				return false;
			} else if (buf[itr] == '(') {
				openP = true;
				itr++;
				continue;
			} else if (buf[itr] != ',' && buf[itr] != ')') {
				identL++;
				bufV[itrV++] = buf[itr];
				itr++;
				continue;
			}
			else if (buf[itr] == ',') {
				commaFlag = true;
				if (identL != 8 && !opFlag) { // !opflag for checking number and not the identifier
#ifdef DEBUG_LOG
					debugUtil("checkTokens_vout", " identifier not 8 bit" + str);
#endif // DEBUG_LOG
					cout << "Identifier size should be 8 bits: " << str << endl;
					return false;
				}
				else {
					//bufV[itrV++] = ',';
					itrV = 0;
					itr++;
					identL = 0;
					continue;
				}
			}
			else if (buf[itr] == ')' && openP) {
				// TO check if there is a , between the identifier/name and the numerical
				if (commaPresent == false) {
#ifdef DEBUG_LOG
					debugUtil("checkTokens_vout", "Malformed transaction record, no \",\" found in the pair" + str);
#endif // DEBUG_LOG
					cout << "Malformed transaction record, no \",\" found in the pair" << str << endl;
					return false;
				}
				closeP = true;
				if (openP && closeP) {
					n++;
					openP = false;
					closeP = false;
					identL = 0;
					string temp(bufV);

					long int tempN = stoi(bufV2);
					ptr.push_back(make_pair(temp, tempN));
					itrV = 0;
					memset(bufV, 0, sizeof(bufV));
					memset(bufV2, 0, sizeof(bufV2));
					itr++;
					//commaFlag = false;
					commaPresent = false;
				}
				else {
#ifdef DEBUG_LOG
					debugUtil("checkTokens_vout", ") in wrong place" + str);
#endif // DEBUG_LOG
					cout<<"malformed record with uneven parenthesis, "<<str<<endl;
					return false;
				}
			}
		}
	}
	if (openP && !closeP) {
#ifdef DEBUG_LOG
		debugUtil("checkTokens_vout", "not a valid vout" + str);
#endif // DEBUG_LOG
		return false;
	}

	if (n == count)
		return true;
	else {
#ifdef DEBUG_LOG
		debugUtil("checkTokens_vout", "number of vout doesn't match with M, " + str);
#endif // DEBUG_LOG
		return false;
	}
}

bool isInt(string str)
{
	return (str.find_first_not_of("0123456789") == string::npos); // Not checking for -ve integers
}

bool isTypeSafe(int col, string str, int lM, int lN, vector<pair<string, long int>> &lvOut, vector<pair<string, long int>> &lop, string lTransId)
{
	switch (col)
	{
	case 1:
	{
		if (str.length() == 8) {
			return true;
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("isTypeSafe", "Transaction identifier is not 8 characters > " + str);
#endif // DEBUG_LOG
			cout << "Transaction Id should be 8 characters, " << str << endl;
			return false;
		}
	}
	break;
	case 2:
	{
		if (isInt(str)) {
			int tempI = stoi(str);
			if (tempI < 0)
			{
				if (lVerboseFlag)
					cout<<"TransId: "<<lTransId<<"M - number of vto needs to be positive int, "<<str<<endl;
				cout<<"M value needs to be >= 0, "<<str<<endl;
				return false;
			}
			return true;
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("isTypeSafe", "UTXO is not an integer, " + str);
#endif // DEBUG_LOG
			cout << "Malformed record, utxo should be integer, transID: " << lTransId << ", " << str << endl;
			return false;
		}
	}
	break;
	case 3:
	{
		int count = lM;
		if (count == 0) {
			// To Do check if its the first record then its valid for M to be 0
		} else if (checkTokens_vout(count, str, false, lvOut)) {
#ifdef DEBUG_LOG
			iterateVecUtil(lvOut);
#endif // DEBUG_LOG
			return true;
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("isTypeSafe", "vout format not valid, " + str);
#endif // DEBUG_LOG
			cout << "Malformed record, vout format is worng. transId: " << lTransId << ", " << str << endl;
			return false;
		}
	}
	break;
	case 4:
	{
		if (isInt(str)) {
			int tempI = stoi(str);
			if (tempI <= 0)
			{
				if (lVerboseFlag)
					cout<<"TransId: "<<lTransId<<"N - number of outputs needs to be greater than 0, "<<str<<endl;
				cout<<"N value needs to be > 0, "<<str<<endl;
				return false;
			}
			return true;
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("isTypeSafe", "N is not valid format, " + str);
#endif // DEBUG_LOG
			cout << "Malformed record, N should be of integer type. transId: " << lTransId << ", " << str << endl;
			return false;
		}
	}
	break;
	case 5:
	{
		int count = lN;
		if (count == 0) {
			// To Do invalidate for 0?
			// Taken care in the previous case itself
		}
		else if (checkTokens_vout(count, str, true, lop)) {
#ifdef DEBUG_LOG
			iterateVecUtil(lop);
#endif // DEBUG_LOG
			return true;
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("isTypeSafe", "op format not valid, " + str);
#endif // DEBUG_LOG
			return false;
			cout << "Malformed record, output format is wrong. transId: " << lTransId << ", " << str << endl;
		}
	}
	break;
	default:
#ifdef DEBUG_LOG
		debugUtil("isTypeSafe", "column number wrong with " + col);
#endif // DEBUG_LOG
		return false;
		break;

	}
	return false;
}




string getRecord(long int M, vector<pair<string, long int>> vOut, long int N, vector<pair<string, long int>> op)
{
	string str = "";
	str = str + to_string(M) + "; ";
	auto vec = vOut;

	auto itr1 = vec.begin();
	while (itr1 != vec.end())
	{
		str = str + "(" + itr1->first + ", " + to_string(itr1->second) + ")";
		itr1++;
	}

	str = str + "; " + to_string(N) + "; ";
	auto vecO = op;
	auto itr2 = vecO.begin();
	while (itr2 != vecO.end())
	{
		str = str + "(" + itr2->first + ", " + to_string(itr2->second) + ")";
		itr2++;
	}
	str = str + "\n";
	//cout << str << endl;
	return str;
}

bool validateTransId(string ltransId, long int M, vector<pair<string, long int>> vOut, long int N, vector<pair<string, long int>> op, string &hashValue)
{
	string inputStr = getRecord(M, vOut, N, op);
	unsigned char buf[SHA_DIGEST_LENGTH] = { '\0' };
	char inputBuf[1000] = { '\0' };
	sprintf(inputBuf, "%s", inputStr.c_str());

	SHA1((unsigned char*)inputBuf, strlen(inputBuf), (unsigned char*)&buf);
	char buf2[SHA_DIGEST_LENGTH * 2 + 1];
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
		sprintf(&buf2[i * 2], "%02x", (unsigned int)buf[i]);
	//cout << mdstr << endl;
	string transId ="";
	for (int i = 0; i < 8; i++)
		transId += buf2[i];

	//cout<<"TransID : "<<transId<<endl;
	if (!transId.compare(ltransId))
		return true;
	else
		{
			cout<<"TransactioId Hash is wrong. TransId: "<<ltransId<<", should be :"<<transId<<endl;
			hashValue = transId;
			return false;
		}
}



	// To Do return head of the list
record* parseFile(string fileName, record* head, map<string, vector<pair<string, long int>>> &transMap, bool verboseFlag, map<string, record> &recordMap)
{
	lVerboseFlag = verboseFlag;
	bool firstRecordFlag = true;
	record* temp = NULL;
	if (head != NULL)
	{
		temp = head;
		while (temp->next != NULL)
			temp = temp->next;
	}
	string ltransactionId; // To do change to hex
	int lM = 0;
	vector<pair<string, long int>> lvOut;
	int lN = 0;
	vector<pair<string, long int>> lop;

	int columnCount = 0;
	ifstream fileHandle;
	fileHandle.open(fileName);
	if (fileHandle.is_open()) {
		string line;
		while (getline(fileHandle, line))
		{
			bool validRecordFlag = true;
			if (line.length() < 8)
			{
				if (verboseFlag)
					cout<<"Invalid/Empty record"<<endl;
				validRecordFlag = false;
			}
			line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
			columnCount = 0;
			char* buf;
			char* lineBuf = (char*)line.c_str();
			//rsize_t strmax = sizeof lineBuf;
			char* context = NULL;
			char delim[] = ";";
			//buf = strtok_s(lineBuf, delim, &context);
			buf = strtok(lineBuf,";");
			

			while (buf != NULL) {
				columnCount++;
				if (firstRecordFlag && columnCount == 3 && head == NULL) {
					if (lM != 0)
					{
						cout << "First record of the ledger should have 0 vto, TransId: " <<ltransactionId<< endl;
						if (verboseFlag)
							cout << "TransId: " << ltransactionId << ", result - fail" << endl;
						return false;
					}
					continue;
				}
				if (isTypeSafe(columnCount, buf, lM, lN, lvOut, lop, ltransactionId)) {
					switch (columnCount) {
					case 1:
					{
						ltransactionId = buf;
						auto itr = transMap.find(ltransactionId);
						if (itr != transMap.end())
						{
							if (verboseFlag)
								cout << "TransId: " << ltransactionId << ", result - bad" << endl;
							cout << "Transaction Id already used up, " << ltransactionId << endl;
							buf = NULL;
							validRecordFlag = false;
							continue;
						}
					}
					break;
					case 2:
					{
						lM = stoi(buf);
					}
					break;
					case 3:
					{
						// To Do create vector of vout

						
					}
					break;
					case 4:
					{
						lN = stoi(buf);
					}
					break;
					case 5:
					{
						// To do create vector of op
					}
					break;
					default:
#ifdef DEBUG_LOG
						debugUtil("parseFile", "Column number exceeding 5, " + to_string(columnCount));
#endif // DEBUG_LOG

						break;
					}
				}
				else {
#ifdef DEBUG_LOG
					debugUtil("parseFile", "Type check of column failed, " + to_string(columnCount));

#endif // DEBUG_LOG
					validRecordFlag = false;
					break;
				}				
				//buf = strtok_s(NULL, delim, &context);
				buf = strtok(NULL,";");
			}
			// Record is valid
			//if (buf == NULL && !firstRecordFlag)
				//validRecordFlag = false;
			if (validRecordFlag) {
				
				// Check for SHA1 hash
				string calculatedHash;
					if (!validateTransId(ltransactionId, lM, lvOut, lN, lop, calculatedHash))
					{
						// update transMap and recordMap with the new transId
						/*cout<<"Updating transId with correct hash value obtained from the record paylaod"<<"TransId: "<<node->transId<<", to transId: "<<calculatedHash<<endl;
						auto itrTrans = transMap.find(node->transId);
						auto vecTrans = itrTrans->second;
						transMap.insert(pair<string, vector<pair<string,long int>>> (calculatedHash, itrTrans->second));
						//auto itrRec = recordMap.find(node->transId);	
						recordMap.erase(node->transId);						
						transMap.erase(node->transId);
						node->transId = calculatedHash;
						recordMap.insert(pair<string, record>(node->transId, *node));					*/
						cout<<"Updating transId with correct hash value obtained from the record paylaod"<<"TransId: "<<ltransactionId<<", to transId: "<<calculatedHash<<endl;
						ltransactionId = calculatedHash;
					}

				
				
				
				
				if (head == NULL) {
					head = new record(ltransactionId, lM, lvOut, lN, lop);
					temp = head;
					firstRecordFlag = false;
				}
				else {
					temp->next = new record(ltransactionId, lM, lvOut, lN, lop);
					temp = temp->next;
				}
			}
			lM = -1;
			lN = -1;
			ltransactionId.erase();
			lvOut.erase(lvOut.begin(), lvOut.end());
			lop.erase(lop.begin(), lop.end());
		}
	}
	else {
#ifdef DEBUG_LOG
		debugUtil("parseFile", "file open failed");
#endif // DEBUG_LOG
		cerr << "File open failed";
	}
	return head;
}



/*// Utility function to check if the given transaction record is syntactically valid
record* isTransactionStringValid(string line, record* head)
{
	bool firstRecordFlag;
	record* temp = NULL;
	if (head == NULL)
		firstRecordFlag = true;
	else {
		firstRecordFlag = false;
		temp = head;
		while (temp->next != NULL)
			temp = temp->next;
	}
	
	string ltransactionId; // To do change to hex
	int lM = 0;
	vector<pair<string, long int>> lvOut;
	int lN = 0;
	vector<pair<string, long int>> lop;

	int columnCount = 0;
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
	columnCount = 0;
	char* buf;
	char* lineBuf = (char*)line.c_str();
	//rsize_t strmax = sizeof lineBuf;
	char* context = NULL;
	char delim[] = ";";
	//buf = strtok_s(lineBuf, delim, &context);
	buf = strtok(lineBuf,";");
	bool validRecordFlag = true;

	while (buf != NULL) {
		//ltransactionId = "";
		columnCount++;
		if (firstRecordFlag && columnCount == 3)
			continue;
		
		if (isTypeSafe(columnCount, buf, lM, lN, lvOut, lop, ltransactionId)) {
			switch (columnCount) {
			case 1:
			{
				ltransactionId = buf;
			}
			break;
			case 2:
			{
				lM = stoi(buf);
			}
			break;
			case 3:
			{
				// To Do create vector of vout


			}
			break;
			case 4:
			{
				lN = stoi(buf);
			}
			break;
			case 5:
			{
				// To do create vector of op
			}
			break;
			default:
#ifdef DEBUG_LOG
				debugUtil("parseFile", "Column number exceeding 5, " + to_string(columnCount));
#endif // DEBUG_LOG

				break;
			}
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("parseFile", "Type check of column failed, " + to_string(columnCount));

#endif // DEBUG_LOG
			validRecordFlag = false;
			break;
		}
		//buf = strtok_s(NULL, delim, &context);
		buf = strtok(NULL,";");
	}
	// Record is valid
	if (validRecordFlag) {
		if (head == NULL) {
			head = new record(ltransactionId, lM, lvOut, lN, lop);
			temp = head;
			firstRecordFlag = false;
		}
		else {
			temp->next = new record(ltransactionId, lM, lvOut, lN, lop);
			temp = temp->next;
		}
	}
	else
	{
		cout << "TransiId: "<<ltransactionId<<", Invalid record" << endl;
	}
	return head;
}*/

// Utility function to check if the given transaction record is syntactically valid
record* isTransactionStringValid(string line, record* head, map<string, vector<pair<string, long int>>> transMap, bool verboseFlag)
{
	
	bool validRecordFlag = true;
	if (line.length() < 8)
	{		
				//if (verboseFlag)
					cout<<"Invalid/Empty record"<<endl;
				validRecordFlag = false;
		return head;
	}
	lVerboseFlag = verboseFlag;
	bool firstRecordFlag;
	record* temp = NULL;
	if (head == NULL)
		firstRecordFlag = true;
	else {
		firstRecordFlag = false;
		temp = head;
		while (temp->next != NULL)
			temp = temp->next;
	}

	string ltransactionId; // To do change to hex
	int lM = 0;
	vector<pair<string, long int>> lvOut;
	int lN = 0;
	vector<pair<string, long int>> lop;

	int columnCount = 0;
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
	columnCount = 0;
	char* buf;
	char* lineBuf = (char*)line.c_str();
	//rsize_t strmax = sizeof lineBuf;
	char* context = NULL;
	char delim[] = ";";
	buf = strtok(lineBuf, ";");
	

	while (buf != NULL) {
		//ltransactionId = "";
		columnCount++;
		if (firstRecordFlag && columnCount == 3 && head == NULL) {
			if (lM != 0)
			{
				cout << "First record of the ledger should have 0 vto, TransId: " << ltransactionId << endl;
				if (verboseFlag)
					cout << "TransId: " << ltransactionId << ", result - fail" << endl;
				return false;
			}
			continue;
		}

		if (isTypeSafe(columnCount, buf, lM, lN, lvOut, lop, ltransactionId)) {
			switch (columnCount) {
			case 1:
			{
				ltransactionId = buf;
				auto itr = transMap.find(ltransactionId);
				if (itr != transMap.end())
				{
					if (verboseFlag)
						cout << "TransId: " << ltransactionId << ", result - bad" << endl;
					cout << "Transaction Id already used up, " << ltransactionId << endl;
					buf = NULL;
					validRecordFlag = false;
					continue;
				}
			}
			break;
			case 2:
			{
				lM = stoi(buf);
			}
			break;
			case 3:
			{
				// To Do create vector of vout


			}
			break;
			case 4:
			{
				lN = stoi(buf);
			}
			break;
			case 5:
			{
				// To do create vector of op
			}
			break;
			default:
#ifdef DEBUG_LOG
				debugUtil("parseFile", "Column number exceeding 5, " + to_string(columnCount));
#endif // DEBUG_LOG

				break;
			}
		}
		else {
#ifdef DEBUG_LOG
			debugUtil("parseFile", "Type check of column failed, " + to_string(columnCount));

#endif // DEBUG_LOG
			validRecordFlag = false;
			break;
		}
		buf = strtok(NULL, ";");
	}
	// Record is valid
	if (validRecordFlag) {
		if (head == NULL) {
			head = new record(ltransactionId, lM, lvOut, lN, lop);
			temp = head;
			firstRecordFlag = false;
		}
		else {
			temp->next = new record(ltransactionId, lM, lvOut, lN, lop);
			temp = temp->next;
		}
	}
	else
	{
		cout << "TransiId: " << ltransactionId << ", Invalid record" << endl;
	}
	return head;
}
