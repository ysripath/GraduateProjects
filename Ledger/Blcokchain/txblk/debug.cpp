#include "debug.h"

void debugUtil(string functionName, string message)
{
	cout << "DEBUG>> Function : " << functionName << ", " << message << endl;
}

void debugUtil(string message)
{
	cout << "DEBUG>> " << message << endl;
}
