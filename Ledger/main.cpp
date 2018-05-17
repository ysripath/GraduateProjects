#include "header.h"
#include "util.h"
#include "parser.h"
#include <map>
#include <openssl/sha.h>

#define HELP_DESCRIPTION "\
[F]ile:  Supply filename : <infilename>.Read in a file of transactions.Any invalid transaction shall be identified with an error message to stderr, but not stored.Print an error message to stderr if the input file named cannot be opened.The message shall be \"Error : file <infilename> cannot be opened for reading\" on a single line, where <infilename> is the name provided as additional command input.\n \
[T]ransaction : Supply Transaction : <see format below>   Read in a single transaction in the format shown below.It shall be checked for validity against the ledger, and added if it is valid.If it is not valid, then do not add it to the ledger and print a message to stderr with the transaction number followed by a colon, a space, and the reason it is invalid on a single line.\n\
[E]xit : Quit the program\n\
[P]rint : Print current ledger(all transactions in the order they were added) to stdout in the transaction format given below, one transaction per line.\n\
[H]elp : Command Summary\n\
[D]ump : Supply filename : <outfilename>.Dump ledger to the named file.Print an error message to stderr if the output file named cannot be opened.The message shall be \"Error : file <outfilename> cannot be opened for writing\" on a single line, where <outfilename> is the name provided as additional command input.\n\
[W]ipe : Wipe the entire ledger to start fresh.\n\
[I]nteractive : Toggle interactive mode.Start in non - interactive mode, where no command prompts are printed.Print command prompts and prompts for additional input in interactive mode, starting immediately(i.e., print a command prompt following the I command).\n\
[V]erbose : Toggle verbose mode.Start in non - verbose mode.In verbose mode, print additional diagnostic information as you wish.At all times, output each transaction number as it is read in, followed by a colon, a space, and the result(\"good\" or \"bad\").\n\
[B]alance : Supply username : (e.g.Alice).This command prints the current balance of a user.\n"

using namespace std;

map<string, vector<pair<string,long int>>> transMap;
map<string, long int> balanceMap;
map<string, record> recordMap;

bool verboseFlag = false;

void displayOptions()
{
	cout << "[F]ile\n[T]ransaction\n[P]rint\n[H]elp\n[D]ump\n[W]ipe\n[I]nteractive\n[V]erbose\n[B]alance\n[E]xit\nSelect a Command: ";
}


void updateMap(record* node)
{
	if (node == NULL) {
#ifdef DEBUG_LOG
		debugUtil("updateMap", "empty List");
#endif // DEBUG_LOG

	} else {
		while (node != NULL) {
			// Check for double spending
			auto redundantT = transMap.find(node->transId);
			if (redundantT != transMap.end())
			{
				/*node->validRecord = false;
				cout << "Invalid record - Double spending, " << endl;
				printNodeUtil(node);*/
				node = node->next;
				continue;
			} 
			else {
				transMap.insert(pair<string, vector<pair<string, long int>>>(node->transId, node->op));
				recordMap.insert(pair<string, record>(node->transId, *node));
			}
			node = node->next;
		}
	}
}

/*void getRecord(record* node)
{
	string str = "";
	str = str + to_string(node->M) + "; ";
	auto vec = node->vOut;

	auto itr1 = vec.begin();
	while (itr1 != vec.end())
	{
		str = str + "(" + itr1->first + ", " + to_string(itr1->second) + ")";
		itr1++;
	}

	str = str + "; " + to_string(node->N) + "; ";
	auto vecO = node->op;
	auto itr2 = vecO.begin();
	while (itr2 != vecO.end())
	{
		str = str + "(" + itr2->first + ", " + to_string(itr2->second) + ")";
		itr2++;
	}
	str = str + "\n";
	cout << str << endl;
}*/

/*bool validateTransId(record* node)
{
	string inputStr = getRecord(node);
	unsigned char buf[SHA_DIGEST_LENGTH] = { '\0' };
	char inputBuf[1000] = { '\0' };
	sprintf(inputBuf, "%s", inputStr.c_str());

	SHA1((unsigned char*)inputBuf, strlen(inputBuf), (unsigned char*)&buf);
	char mdstr[SHA_DIGEST_LENGTH * 2 + 1];
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
		sprintf(&mdstr[i * 2], "%02x", (unsigned int)buf[i]);
	cout << mdstr << endl;
}*/
bool checkDoubleSpend(int index, string transId)
{
	auto itr = recordMap.find(transId);
	if (itr != recordMap.end())
	{
		auto itrMap = itr->second.spentMap;
		auto itrFlag = itrMap.find(index);
		if (itrFlag != itrMap.end())
		{
			if (itrFlag->second == true) {
				if (verboseFlag)
					cout << "TransId: " << transId << ", result - bad" << endl;
				cout << "The utxo is already used : " << transId << ", " << index << endl;
				return false;
			}
			else
				return true;
		}
		else
		{
#ifdef DEBUG_LOG
			debugUtil("checkDoubleSpend", "index not found");
#endif // DEBUG_LOG
			if (verboseFlag)
				cout << "TransId: " << transId << ", result - bad" << endl;
			cout << "Invalid index in uxto: " << transId << ", " << index << endl;
		}
	}
}

void updateFlag(string transId, int index)
{
	auto itr = recordMap.find(transId);
	if (itr != recordMap.end())
	{
		//auto itrMap = itr->second.spentMap;
		auto itrMapItr = itr->second.spentMap.find(index);
		if (itrMapItr != itr->second.spentMap.end())
		{
			if (itrMapItr->second == true)
			{
#ifdef DEBUG_LOG
				debugUtil("updateFlag", "logic is wrong");
#endif // DEBUG_LOG
			}
			else
			{
				itrMapItr->second = true;
			}
		}
		else
		{
#ifdef DEBUG_LOG
			debugUtil("updateFlag", "Spentmap index not found: " + index);
#endif // DEBUG_LOG

		}
	}
	else
	{
#ifdef DEBUG_LOG
		debugUtil("updateFlag", "Record not found for transId: " + transId);
#endif // DEBUG_LOG
		cout << "Record not found in ledger for TransId: " << transId << endl;

	}
}


string getRecord(record* node)
{
	string str = "";
	str = str + to_string(node->M) + "; ";
	auto vec = node->vOut;

	auto itr1 = vec.begin();
	while (itr1 != vec.end())
	{
		str = str + "(" + itr1->first + ", " + to_string(itr1->second) + ")";
		itr1++;
	}

	str = str + "; " + to_string(node->N) + "; ";
	auto vecO = node->op;
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

bool validateTransId(record* node, string &hashValue)
{
	string inputStr = getRecord(node);
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
	if (!transId.compare(node->transId))
		return true;
	else
		{
			cout<<"TransactioId Hash is wrong. TransId: "<<node->transId<<", should be :"<<transId<<endl;
			hashValue = transId;
			return false;
		}
}

void updateBalance(record* node)
{
	int indexUtil[100] = { 0 };
	string transIdUtil[100];
	//memset(transIdUtil, '\0', sizeof(transIdUtil));
	int vOutCtr = 0;
	int voutSum = 0;
	int opSum = 0;
	if (node == NULL) {
#ifdef DEBUG_LOG
		debugUtil("updateBalance", "Empty list");
#endif // DEBUG_LOG
	}
	else {
		while (node != NULL) {
			if (node->neglect)
			{
				node = node->next;
				continue;
			}
			if (node->includedRecord == false ) {
				if (node->recordCount == 1) {
					// No need to check the vouts
					auto itr = node->op.begin();
					while (itr != node->op.end())
					{
						auto itr2 = balanceMap.find(itr->first);
						if (itr2 == balanceMap.end())
						{
							balanceMap.insert(pair<string, long int>(itr->first, itr->second));
						}
						else
						{
							itr2->second += itr->second;
						}
						itr++;
					}
					node->validRecord = true;
				}
				else {
					// Get the vouts and check if transaction is feasible and update accordingly
					auto itrVout = node->vOut.begin();
					while (itrVout != node->vOut.end())
					{
						if (!checkDoubleSpend(itrVout->second, itrVout->first))
						{
							node->validRecord = false;
							//transMap.erase(node->transId);
							break;
						}
						// get current balance for if corresponding to the previous record
						auto itrRecord = transMap.find(itrVout->first);
						if (itrRecord != transMap.end())
						{
							// Syntactically valid
							auto itrOp_prev = itrRecord->second[itrVout->second];
							voutSum += itrOp_prev.second;
							indexUtil[vOutCtr] = itrVout->second;
							transIdUtil[vOutCtr] = itrVout->first;
							vOutCtr++;
						}
						else
						{
							// Not a valid transaction
#ifdef DEBUG_LOG
							debugUtil("updateBalance", "Invalid transactionID found in " + itrVout->first);
#endif // DEBUG_LOG
							if (verboseFlag)
								cout << "Invalid transaction Id referenced in " << itrVout->first << endl;
						}

						itrVout++;
					}

					auto itrOp = node->op.begin();
					while (itrOp != node->op.end())
					{
						opSum += itrOp->second;
						itrOp++;
					}

				}
				if (voutSum == opSum) {
					// Validate SHA1
					//getRecord(node);
					
					string calculatedHash;
					if (!validateTransId(node, calculatedHash))
					{
						// update transMap and recordMap with the new transId
						cout<<"Updating transId with correct hash value obtained from the record paylaod"<<"TransId: "<<node->transId<<", to transId: "<<calculatedHash<<endl;
						auto itrTrans = transMap.find(node->transId);
						auto vecTrans = itrTrans->second;
						transMap.insert(pair<string, vector<pair<string,long int>>> (calculatedHash, itrTrans->second));
						//auto itrRec = recordMap.find(node->transId);	
						recordMap.erase(node->transId);						
						transMap.erase(node->transId);
						node->transId = calculatedHash;
						recordMap.insert(pair<string, record>(node->transId, *node));					
					}

					node->validRecord = true;
					node->includedRecord = true;
					for (int j = 0; j < vOutCtr; j++)
					{
						updateFlag(transIdUtil[j], indexUtil[j]);
					}
					//if (verboseFlag)
					//{
						cout << "TransId: " << node->transId << ", result - good" << endl;
					//}
				}
				else
				{
					// remove the record from the transMap
					// Test logic
					if (verboseFlag)
					{
						cout << "TransId: " << node->transId << ", result - bad" << endl;
					}
					if (voutSum < opSum)
						cout<<"Not enough money for TransId: "<<node->transId<<endl;
					else if (voutSum > opSum)
						cout<<"Complete utxo not used, TransId: "<<node->transId<<endl;
					node->neglect = true;
					transMap.erase(node->transId);
				}
			}
			voutSum = 0;
			opSum = 0;
			node = node->next;
		}
	}
}


void updateBalanceMap(record* node)
{
	if (node == NULL)
	{
#ifdef DEBUG_LOG
		debugUtil("updateBalanceMap", "Empty List");
#endif // DEBUG_LOG
	}
	else
	{
		while (node != NULL)
		{
			if (node->neglect)
			{
				node = node->next;
				continue;
			}
			//if (node->includedRecord == false) {
				string acName[1000];
				int countUtil = -1;
				int acValue[1000] = { 0 };
				if (node->recordCount == 1)
				{
				
					// update balance map for 1st record
					auto itr = node->op.begin();
					while (itr != node->op.end())
					{
						balanceMap.insert(pair<string, long int>(itr->first, itr->second));
						itr++;
					}
					node = node->next;
					continue;
				}
				else if (node->validRecord)
				{

					auto vecVout = node->vOut;
					auto voutItr = vecVout.begin();
					while (voutItr != vecVout.end())
					{
						// Get the transaction id and corresponding utxo
						string tId = voutItr->first;
						int index = voutItr->second;

						auto transItr = transMap.find(tId);
						if (transItr != transMap.end())
						{
							countUtil++;
							// A valid transaction found
							auto opItr = transItr->second;
							auto utxoItr = opItr.at(index);
							acName[countUtil] = utxoItr.first;
							acValue[countUtil] = utxoItr.second;
						}
						else
						{
							// Transaction in vto is invalid
#ifdef DEBUG_LOG
							debugUtil("updateBalanceMap", "Transaction identifier is not valid, " + tId);
#endif // DEBUG_LOG

							continue;
						}

						voutItr++;
					}

					for (int i = 0; i <= countUtil; i++)
					{
						auto accountDebitItr = balanceMap.find(acName[i]);
						if (accountDebitItr == balanceMap.end())
						{
#ifdef DEBUG_LOG
							debugUtil("updateBalanceMap", "Account not found in balanceMap");
#endif // DEBUG_LOG
							if (verboseFlag)
								cout << "TransId: " << node->transId << " Account holder not found in the record, " << acName << endl;
						}
						accountDebitItr->second -= acValue[i];
					}
					
					auto opVector = node->op;
					auto opItr_cur = opVector.begin();
					while (opItr_cur != opVector.end())
					{
						string name = opItr_cur->first;
						auto tempName = balanceMap.find(name);
						if (tempName == balanceMap.end())
						{
							// First entry of the account name in the balance map
							balanceMap.insert(pair<string, long int>(name, opItr_cur->second));
							//accountDebitItr->second -= opItr_cur->second;
						}
						else
						{
							// Update balance for the account name
							tempName->second += opItr_cur->second;
							//accountDebitItr->second -= opItr_cur->second;
						}
						opItr_cur++;
					}
				}
		//	}
			node = node->next;
		}
	}
}



void printBalance(string name)
{
	auto itr = balanceMap.find(name);
	if (itr == balanceMap.end())
	{
		cout << "No record found for " << name << endl;
	}
	else
	{
		cout << name << ", " << itr->second << endl;
	}
}


void outputToFile(fstream &fileHandle, record* head)
{
	if (head == NULL)
	{
		if (verboseFlag)
			cout << "Empty ledger" << endl;
		return;
	}
	while (head != NULL)
	{
		if (head->validRecord)
		{
			printNodeToFileUtil(fileHandle, head);
		}
		head = head->next;
	}
}


record* wipeRecord(record* head)
{
	if (head == NULL)
	{
		if (verboseFlag)
			cout << "Ledger is already empty" << endl;
		return NULL;
	}
	else
	{
		while (head != NULL)
		{
			record* temp = head;
			head = head->next;
			delete temp;
		}
	}
	return NULL;
}
int main()
{
	record* head = NULL;
	bool INTERACTIVE_MODE = false;
	string choiceStr;
	bool loopFlag = true;
	while (loopFlag) {
		if (INTERACTIVE_MODE)
			displayOptions();
		cin >> choiceStr;
		char choiceInput = choiceUtil(choiceStr);
		switch (choiceInput) {
		case FILE: {
#ifdef DEBUG_LOG
			debugUtil("FILE option selected");
#endif // DEBUG_LOG
			// Test parser
			if (INTERACTIVE_MODE)
				cout << "Enter the filename" << endl;
			ofstream inFile;
			string fileName;
			cin >> fileName;
			// Open file in input only mode
			inFile.open(fileName, ios::in);
			if (inFile.is_open()) {
				// TO DO
				//transMap.erase(transMap.begin(), transMap.end());
				balanceMap.erase(balanceMap.begin(), balanceMap.end());
				head = parseFile(fileName, head, transMap, verboseFlag, recordMap);
				//cout << "Parsed file" << endl;
				updateMap(head);
				updateBalance(head);
				updateBalanceMap(head);
				//printListUtil(head);
#ifdef DEBUG_LOG
				printBalance(balanceMap);
#endif // DEBUG_LOG
				continue;
;			}
			else {
				cerr << "File " << fileName << " open with input mode failed" << endl;
#ifdef DEBUG_LOG
				debugUtil("File "+fileName+" open failed");
#endif // DEBUG_LOG
			}
			inFile.close();
		} break;
		case TRANSACTION: {
#ifdef DEBUG_LOG
			debugUtil("TRANSACTION option selected");
#endif // DEBUG_LOG
			string transString;
			if (INTERACTIVE_MODE)
				cout << "Enter transaction record" << endl;
			//cin >> transString; Get line
			cin.ignore();
			getline(cin, transString);
			head = isTransactionStringValid(transString, head, transMap, verboseFlag);
			//transMap.erase(transMap.begin(), transMap.end());
			balanceMap.erase(balanceMap.begin(), balanceMap.end());
			updateMap(head);
			updateBalance(head);
			updateBalanceMap(head);
		} break;
		case PRINT: {
#ifdef DEBUG_LOG
			debugUtil("PRINT option selected");
#endif // DEBUG_LOG
			if (head == NULL)
			{
				if (verboseFlag)
					cout << "Ledger is empty" << endl;
			}
			else
			{
				printListUtil(head);
				cout << endl << endl;
			}

		} break;
		case HELP: {
#ifdef DEBUG_LOG
			debugUtil("HELP option selected");
#endif // DEBUG_LOG
			cout << HELP_DESCRIPTION;
		} break;
		case DUMP: {
#ifdef DEBUG_LOG
			debugUtil("DUMP option selected");
#endif // DEBUG_LOG
			if (INTERACTIVE_MODE)
				cout << "Entere the file name" << endl;
			string opFileName;
			cin >> opFileName;
			fstream opFileHandle;
			opFileHandle.open(opFileName, fstream::out);
			if (opFileHandle.is_open())
			{
				outputToFile(opFileHandle, head);
				if (INTERACTIVE_MODE || verboseFlag)
					cout << "Dumped in " << opFileName << endl;
			}
			else
			{
#ifdef DEBUG_LOG
				debugUtil("File open fail for the oputput file");
#endif // DEBUG_LOG
				cerr << "Error: file "<<opFileName<<" cannot be opened for writing";
			}
			opFileHandle.close();
		} break;
		case WIPE: {
#ifdef DEBUG_LOG
			debugUtil("WIPE option selected");
#endif // DEBUG_LOG
			head = wipeRecord(head);
			transMap.erase(transMap.begin(), transMap.end());
			balanceMap.erase(balanceMap.begin(), balanceMap.end());
			recordMap.erase(recordMap.begin(), recordMap.end());
			record::recordNumber = 1;
			if (verboseFlag)
				cout << "Ledger is wiped out and empty now" << endl;
		} break;
		case INTERACTIVE: {
#ifdef DEBUG_LOG
			debugUtil("INTERACTIVE option selected");
#endif // DEBUG_LOG
			INTERACTIVE_MODE = !INTERACTIVE_MODE;
		} break;
		case VERBOSE: {
#ifdef DEBUG_LOG
			debugUtil("VERBOSE option selected");
#endif // DEBUG_LOG
			verboseFlag = !verboseFlag;
		} break;
		case BALANCE: {
#ifdef DEBUG
			debugUtil("BALANCE option selected");
#endif // DEBUG
			if (INTERACTIVE_MODE)
				cout << "Enter name" << endl;
			string str;
			cin >> str;
			printBalance(str);
			cout << endl;
		} break;
		case EXIT: {
#ifdef DEBUG_LOG
			debugUtil("EXIT option selected");
#endif // DEBUG_LOG
			cout << "Good Bye!" << endl;
			loopFlag = false;
		} break;
		
		default: {
#ifdef DEBUG_LOG
			debugUtil("INVALID option selected");
#endif // DEBUG_LOG
			cout << "Invalid option. Try again" << endl;
		}
		}
	}
#ifdef DEBUG_LOG
	debugUtil("Exited while loop");
#endif // DEBUG_LOG
	return 0;
}
