#include "Node.h"


// Constructor to initialize the members of the node that represents an #tag in Fibonacci heap
Node::Node(string dataValue, int frequencyValue)
{
	pData = dataValue; // #tag string
	pRankValue = 0; // Rank value of the node in the Fibonacci heap
	pFrequency = frequencyValue; // Frequency value of the #tag
	pChild = NULL; // Pointer to child if any in the Fibonacci heap structure
	pLeftSibling = NULL; // Pointer to the left sibling of the node
	pRightSibling = NULL; // Pointer to the right sibling of the node
	pParent = NULL; // Pointer to the parent of the node in the Fibonacci heap
	pChildCutValue = -1; // Child cut value of the node in Fibonacci heap, initialized to -1 undefined (root level node)
}

Node::~Node()
{
	pData = "";
	delete(pChild);
	pChild = NULL;
	delete(pLeftSibling);
	pLeftSibling = NULL;
	delete(pRightSibling);
	pRightSibling = NULL;
}