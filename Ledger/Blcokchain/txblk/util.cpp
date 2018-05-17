#include "util.h"
#include "cryptoUtil.h"


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
			|| str.compare("EXIT") == 0
			|| str.compare("OUTPUT") == 0
			|| str.compare("CHECK") == 0
			|| str.compare("READ") == 0) {
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
        if (head->validRecord && !head->validated) {
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

string construct(record* node)
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

    // sha256 hash is done by the verification API itself

    // get sha256 hash of  this string
    /*unsigned char buf[SHA256_DIGEST_LENGTH] = { '\0' };
    char inputBuf[1000] = { '\0' };
    sprintf(inputBuf, "%s", str.c_str());

    SHA256((unsigned char*)inputBuf, strlen(inputBuf), (unsigned char*)&buf);
    char buf2[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
            sprintf(&buf2[i * 2], "%02x", (unsigned int)buf[i]);
    //cout << mdstr << endl;
    std::string hash(buf2);
    cout<<str<<", hash256  "<<hash<<endl;

    return hash;*/
    //cout<<"PT ,"<<str<<endl;
    return str;
}

bool checkUtil(string plainText, string publicKey, string base64)
{
    //char* base64Text = (char*)base64.c_str();
    //cout<<"base64 - "<<base64Text<<endl;
    //cout<<"public key - "<<publicKey<<endl;
    //cout<<"PT - "<<plainText<<endl;

    // Base 64 is actually in hex format (encrypted message)
   if (verifySignature(publicKey, plainText, base64))
     return true;
   else
    return false;
}

// Remove escaped new lines
std::string& fixNewLinesUtil(std::string& s)
{
    size_t start_pos = 0;
    while((start_pos = s.find("\\n", start_pos)) != std::string::npos) {
         s.replace(start_pos, 2, "\n");
         start_pos += 1;
    }
    return s;
}

// Utility function to check if signature is verified
bool checkSignature(record* node, map<string, string> fileMap, map<string, vector<pair<string,long int>>> transMap, record* head, bool verboseFlag, bool blockFlag)
{
    // if its the genesis record
    if (node->recordCount == 1
            && node->validRecord)
    {
        // get the account name
        auto itr = node->op;
        auto tempItr = itr.at(0);
        string acName = tempItr.first;
        auto itr2 = fileMap.find(acName);
        if (itr2 == fileMap.end())
        {
            cout<<"Key file not found for "<<acName<<endl;
            return false;
        }
        else
        {
            ifstream fileHandle;
            string fileName = itr2->second;
            fileHandle.open(fileName);
            if (fileHandle.is_open())
            {
                string key((std::istreambuf_iterator<char>(fileHandle)), std::istreambuf_iterator<char>());
                //getline(fileHandle, key);

                string message = construct(node);
                //cout<<"Message - "<<message<<endl;
              //  cout<<"Key - "<<key<<endl;
              //  cout<<"Signature - "<< node->signature<<endl;

                if (checkUtil(message, key, node->signature))
                {
                    fileHandle.close();
                    return true;
                }
                else
                {
                    fileHandle.close();
                    return false;
                }

            }
            else
            {
                cout<<"Error while openieng file "<<fileName<<" for "<<acName<<endl;
                return false;
            }
        }
    }
    else if (node->validRecord)
    {
        // Check if multiple owners present
        if (node->vOut.size() > 1)
        {
            //check for more than one owner
            int index;
            string tTid;
            string name = "";
            auto tItr = node->vOut.begin();
            while(tItr != node->vOut.end())
            {
                tTid = tItr->first;
                index = tItr->second;

                auto itr2 = transMap.find(tTid);
                auto itrTemp = itr2->second;
                auto utxoItr = itrTemp.at(index);
                string acName = utxoItr.first;
                if (name.length() == 0)
                    name = acName;
                if (name.compare(acName)) {
                    cout<<"TransId: "<<node->transId<<" has multiple owners for utxo"<<endl;
                    return false;
                }
                tItr++;
            }
        }


        // Get the account name
        auto itr = node->vOut.begin();
        string tId = itr->first;

        // Check if vout transaction is validated
        record* nodeTemp = head;
        while (nodeTemp != NULL)
        {
            if (!nodeTemp->transId.compare(tId)) {
                if (blockFlag && !nodeTemp->inBlock)
            {
                    if (verboseFlag)
                        cout<<node->transId<<" referring to transId "<<tId<<" which doesn't have valid signature"<<endl;
                return false;
            }
            }
            nodeTemp = nodeTemp->next;
        }


        int index = itr->second;
        auto opItr = transMap.find(tId);
        if (opItr != transMap.end())
        {

            auto itrTemp = opItr->second;
            auto utxoItr = itrTemp.at(index);
            string acName = utxoItr.first;
            auto itr2 = fileMap.find(acName);
            if (itr2 == fileMap.end())
            {
                cout<<"Key file not found for "<<acName<<endl;
                return false;
            }
            else
            {
                ifstream fileHandle;
                string fileName = itr2->second;
                fileHandle.open(fileName);
                if (fileHandle.is_open())
                {
                    string key((std::istreambuf_iterator<char>(fileHandle)), std::istreambuf_iterator<char>());
                    string message = construct(node);
                    if (checkUtil(message, key, node->signature))
                    {
                        fileHandle.close();
                        return true;
                    }
                    else
                    {
                        fileHandle.close();
                        return false;
                    }

                }
                else
                {
                    cout<<"Error while openieng file "<<fileName<<" for "<<acName<<endl;
                    return false;
                }
            }

        }
        else
        {
#ifdef DEBUG_LOG
            debugUtil("TransacionId no found in transMap");
#endif // DEBUG_LOG
            cout<<"Invalid vout"<<endl;
            return false;
        }
    }
    else
    {
        cout<<"Invalid transaction record - "<<node->transId<<endl;
        return false;
    }

	return true;
}
