#include "StringRelated.h"
#include <cstring>

// Utility function for parsing the input string and initialize the InputData structure and return the same
utilStruct checkHashTagPresence(string dataValue)
{
	utilStruct InputData;	

	char buf[1000] = { 0 };
	int length = dataValue.size();
	strncpy(buf, dataValue.c_str(), length); // copy the string into the char array buf
	if (buf[0] == '#') // check for #
	{
		// # is present, create an array with the actual hashtag string without #
		char nBuf[1000] = { 0 };
		char fBuf[100] = { 0 };
		// update the hashValue member of the InputData structure as true
		InputData.hashValue = true;
		
		int j = 0;
		while (buf[j + 1] != ' ') // Delimeter is a space character
		{
			nBuf[j] = buf[j + 1];
			j++;
		}
		nBuf[j] = '\0';
		j += 2;

		// Obtain the frequency value from the hashtag string
		int k = 0;
		while (j != length)
		{
			fBuf[k] = buf[j];
			j++;
			k++;
		}
		fBuf[k] = '\0';
		// Initialize the data member of InputData structure with the hastag string without #
		InputData.data = string(nBuf);
		// Initialize the frequencyValue member of InputData structure with the frequency value obtained from the hashtag string
		InputData.frequencyValue = atoi(string(fBuf).c_str());

		InputData.queryNumber = -1;

		return InputData;
	}
	else
	{
		// If no # found, then obtain the query number and update the same in InputData structure
		InputData.hashValue = false;
		InputData.data = "";
		InputData.frequencyValue = -1;
		InputData.queryNumber = atoi(dataValue.c_str());
		return InputData;
	}
}
