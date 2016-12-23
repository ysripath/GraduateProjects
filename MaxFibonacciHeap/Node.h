#ifndef __NODE_HEADER__
#define __NODE_HEADER__

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;


// Node class to represent each of the node in the Fibonacci heap
class Node
{
public:
	string pData; // #tag string
	int pRankValue; // Rank value of the node in the Fibonacci heap
	int pFrequency; // Frequency value of the #tag
	Node* pChild; // Pointer to child if any in the Fibonacci heap structure
	Node* pLeftSibling; // Pointer to the left sibling of the node
	Node* pRightSibling; // Pointer to the right sibling of the node
	Node* pParent; // Pointer to the parent of the node in the Fibonacci heap
	int pChildCutValue;  //0 FALSE, 1 TRUE, -1 UNDEFINED (ROOT LEVEL NODES)

	// Parameterized construtor to initialize the Node class object with #tag string and its frequency values as the parameters
	Node(string dataValue,int frequencyValue);
	~Node();
};

#endif // !__NODE_HEADER__