#ifndef __MAIN__
#define __MAIN__

#include <fstream>
#include "StringRelated.h"
#include <unordered_map>
#include <utility>
#include "Node.h"
#include <cstring>
#include <vector>

using namespace std;

unordered_map<string, Node*> hashTable; // Container for all the nodes in the heap. Key - #tag string, Value - Corresponding node pointer
int maxValue = 0;
Node* insertFibHeap(Node* maxPointer, string data, int frequencyValue); // For inserting a new node in the Fibonacci heap
Node* remove(Node* maxPointer, string data); // For removing a node from the Fibonacci heap
Node* increaseKey(Node* maxPointer, Node* updatedNode); // For increasing the frequency value of a particular #tag string
Node* shiftToRootLevel(Node* maxPointer, Node* updatedNode); // For moving the node to root level
Node* updateParentRecursive(Node* maxPointer, Node* updatedParentNode); // For handling cascade cut operation
Node* removeMax(Node* maxPointer, int num); // For getting the node that has maximum frequency from the Fibonacci heap
Node* meldUtil(Node* bigger, Node* smaller); // Utility function to support meld - appends smaller as the child to the bigger
Node* meld(Node* maxPointer); // For merging nodes with same degrees at the root level
ofstream outputFile; // filestream to handl output to a file

//DEBUG
void traverse(Node* maxPointer); // Utility function for debugging


Node* buffer[26]; // Array that holds all the max nodes removed, to insert the same after successful meld
int bufCount = 0; // Counter to keep track of number max elements stored in buffer array
int TC = 1;

int main(int argc, char **argv)
{
	string fileName(argv[1]);  // fileName takes the name of the input file specified in the command line
	Node* maxPointer = NULL;	// A pointer that will always point to the node with maximum frequency
	ifstream inputFile; // Filestream for handling input
	string inputString;
	inputFile.open(fileName); // access the input file to start reading the inputs
	outputFile.open("output_file.txt"); // open the output file output_file.txt to record the outputs
	if (inputFile.is_open() && outputFile.is_open())
	{
		
		while (getline(inputFile, inputString)) // Loop as long as there exists input data in the file (Until EOF)
		{
			utilStruct InputData = checkHashTagPresence(inputString); // Utility function that fills the utilStruct structure from the input
			if (InputData.hashValue == true) // if there is #tag
			{				
				auto itr = hashTable.find(InputData.data);
				if (itr != hashTable.end())
				{
					// Increase the frequency value if tag already exists in the heap
					itr->second->pFrequency += InputData.frequencyValue;
					// Call increaseKey function for the same
					maxPointer = increaseKey(maxPointer, itr->second);
				}
				else // if the tag is not found in the heap
				{
					
					// First entry to Fibonacci heap
					if (maxPointer == NULL)
					{
						maxPointer = new Node(InputData.data, InputData.frequencyValue);
						maxPointer->pParent = maxPointer;
						// Child cut value for root level node is undefined = -1
						maxPointer->pChildCutValue = -1;
						// Circular Doubly Linked Lista - sibling nodes
						maxPointer->pLeftSibling = maxPointer;
						maxPointer->pRightSibling = maxPointer;
						maxPointer->pRankValue = 0;						
						hashTable.insert(make_pair(InputData.data, maxPointer)); // insert the new node into the hashTable map
					}
					else
					{
						maxPointer = insertFibHeap(maxPointer, InputData.data, InputData.frequencyValue); // insert the new node into the Fibonacci heap
					}
					
				}				
			}
			else if (InputData.data.compare("stop") == 0 || InputData.data.compare("STOP") == 0) // end of input on finding the string stop
			{
				break;
			}			
			else // No #tag found. Hence its a query to obtain query number of max nodes from the Fibonacci heap
			{				
				maxPointer = removeMax(maxPointer, InputData.queryNumber); // Removes InputData.queryNumber number of max nodes from the Fibonacci heap and reinsert the same into the heap after successful meld
				// Empty the contents of the buffer and initialize bufCount to 0, so that the same can be used for next query
				bufCount = 0;
				for (int i = 0; i <= 25; i++)
				{
					buffer[i] = NULL;
				}
			}
		}
		inputFile.close(); // Close the input file
		
		// DEBUG
		//traverse(maxPointer);
		outputFile.close(); // Close the output file

	}
	else
	{
		cout << "Error" << endl; // File IO error
	}

	return 0;
}


// DEBUG
// Utility function for printing the Node class's member values
void traverseUtil(Node* node)
{
	cout << node->pData << "		" << node->pParent->pData << "		";
	if (node->pChild == NULL)
		cout << "NULL		";
	else
		cout << node->pChild->pData << "		";

	cout << node->pLeftSibling->pData<< "		" << node->pRightSibling->pData << "		" << node->pFrequency << "		" << node->pRankValue << "		" << node->pChildCutValue << endl;	
}

//DEBUG traversal
// utility function for tracking the traversal
void traverse(Node* maxPointer)
{
	cout << "Max tag and its value " << maxPointer->pData << " - " << maxPointer->pFrequency << endl;
	cout << "All the nodes in the map" << endl;
	cout << "T		P		C		L		R		FRQ		RK		CCV" << endl;
	for (auto itr = hashTable.begin(); itr != hashTable.end(); itr++)
	{
		traverseUtil(itr->second);
	}
}



// Utility function for inserting the node into the Fibonacci heap
Node* insertFibHeapUtil(Node* maxPointer, Node* node)
{
	node->pParent = node;
	node->pChildCutValue = -1;
	//node->pRankValue = 0;
	if (maxPointer == NULL) // if there are no nodes in the heap
	{
		Node* temp = node->pLeftSibling;
		temp->pRightSibling = node->pRightSibling;
		node->pRightSibling->pLeftSibling = temp;

		maxPointer = node;
		maxPointer->pLeftSibling = maxPointer;
		maxPointer->pRightSibling = maxPointer;
		return maxPointer;
	}

	// link node's left and right before removing 
	if (node->pRightSibling != node  && node->pLeftSibling != node)
	{
		Node* temp = node->pLeftSibling;
		temp->pRightSibling = node->pRightSibling;
		node->pRightSibling->pLeftSibling = temp;
	}	

	// Overwrite node's left and right links at root level
	node->pRightSibling = maxPointer->pRightSibling;
	node->pLeftSibling = maxPointer;
	maxPointer->pRightSibling->pLeftSibling = node;
	maxPointer->pRightSibling = node;
	if (node->pFrequency >= maxPointer->pFrequency) // Update the maxPointer to the node that has maximum frequency value
	{
		maxPointer = node;
	}
	return maxPointer;	
}


// utility function for appending smaller node to bigger node and update corresponding members of both the nodes
Node* meldUtil(Node* bigger, Node* smaller)
{
	// update the smaller childCutValue to false. (false = 0)
	smaller->pChildCutValue = 0;
	// update smaller's parent to bigger node
	smaller->pParent = bigger;

	//update smaller's left and right at root level
	Node* temp = smaller->pRightSibling;
	smaller->pLeftSibling->pRightSibling = temp;
	temp->pLeftSibling = smaller->pLeftSibling;

	// check if bigger's child exist or not
	if (bigger->pChild == NULL) // if no child for the bigger, smaller becomes the child
	{
		bigger->pChild = smaller;
		smaller->pLeftSibling = smaller;
		smaller->pRightSibling = smaller;
	}
	else // there exists a child for the bigger, link the smaller to the right of existing child
	{
		Node* temp = bigger->pChild;
		smaller->pRightSibling = temp->pRightSibling;
		smaller->pLeftSibling = temp;
		temp->pRightSibling->pLeftSibling = smaller;
		temp->pRightSibling = smaller;
	}
	bigger->pRankValue++; // increment the rank/degree of the parent after appending a new child
	return bigger;
}

// utility function to check if there is a necessity of performing meld operation (if there are nodes of same rank value at the root level)
bool meldCheck(Node* maxPointer)
{
	if (maxPointer == NULL)
		return false;
	unordered_map<int, bool> boolMap;
	Node* temp = maxPointer;
	while (temp->pRightSibling != maxPointer)
	{
		auto itr = boolMap.find(temp->pRankValue);
		if (itr == boolMap.end())
		{
			boolMap.insert(make_pair(temp->pRankValue, true));
		}
		else
		{
			return true;   // Return true if there are redundant rank value nodes are present at the root level
		}
	}
	return false; // Return false if there is no redundant rank value nodes
}


// Function for merging nodes with same degrees at the root level
Node* meld(Node* maxPointer)
{
	// Check if there are redundant rank values at root level
	if (!meldCheck(maxPointer))
	{
		return maxPointer;
	}

	// check if maxPointer in the only node at the root level
	if ((maxPointer->pRightSibling == maxPointer) && (maxPointer->pLeftSibling == maxPointer))
	{
		return maxPointer;
	}
	// There are more than one node at the root level
	bool flag = true;
	Node* begin = maxPointer;
	unordered_map<int, bool> degreeMap; // map for storing bool values of the rank value of nodes at root level. Key - rank, value - true/false
	unordered_map<int, Node*> nodeMap; // map for storing nodes as per their rank value. Key - rank, value - pointer to corresponding node
	//unordered_map<int, bool> checkMap;

	while (flag)
	{
		auto itr = degreeMap.find(begin->pRankValue); // Check if same rank value nodes are found in the degreeMap
		if (itr == degreeMap.end())
		{
			// if there is no node with same rank in the map, insert the same into the map and move to next node to the righ at root level
			degreeMap.insert(make_pair(begin->pRankValue, true));
			nodeMap.insert(make_pair(begin->pRankValue, begin));
			begin = begin->pRightSibling;
			continue;
		}		
		else
		{
			// Node with same rank found in the map
			auto itr2 = nodeMap.find(begin->pRankValue);
			if (itr2->second == begin) // If both nodes are same, meld is over
			{
				flag = false;
				break;
			}
			// Make the smaller node child of bigger node (smaller and bigger in terms of their corresponding frequency)
			if (begin->pFrequency >= itr2->second->pFrequency)
			{
				begin = meldUtil(begin, itr2->second);
			}
			else
			{
				begin = meldUtil(itr2->second, begin);
			}
			itr->second = false;
			nodeMap.erase(itr->first);
			// clear the contents of the map for next iteration
			nodeMap.clear();
			degreeMap.clear();
			// update maxPointer
			if (begin->pFrequency >= maxPointer->pFrequency)
				maxPointer = begin;
			else
				begin = maxPointer;
		}		
	}
	
	return maxPointer;
}

// Function for getting the 'num' number of nodes that has maximum frequency from the Fibonacci heap
Node* removeMax(Node* maxPointer, int num)
{
	bool flag = false;
	while (num > 0)
	{		
		Node* temp = maxPointer->pChild;
		// Check for siblings at the root level
		// if only maxPointer is present at the root level
		if ((maxPointer->pLeftSibling == maxPointer) && (maxPointer->pRightSibling == maxPointer))
		{									
			if (!flag)
			{
				outputFile << maxPointer->pData; // output to file
				flag = true;
			}
			else
			{
				outputFile << "," << maxPointer->pData; // output to file
			}		

			maxPointer->pLeftSibling = maxPointer;
			maxPointer->pRightSibling = maxPointer;
			maxPointer->pChild = NULL;
			maxPointer->pRankValue = 0;

			//insert the max node into the buffer array to reinsert after meld
			buffer[bufCount++] = maxPointer;
			maxPointer = NULL;
			
			// check for children and update its level
			if (temp != NULL)
			{
				// check for siblings of child
				if (temp->pRightSibling == temp && temp->pLeftSibling == temp)
				{
					// insert the child of maxPointer to root level in Fibonacci heap
					maxPointer = insertFibHeapUtil(maxPointer, temp);					
				}
				else
				{
					// insert all the siblings of the child of maxPointer and the child also at the root level of the Fibonacci heap
					Node* temp2 = temp;
					while (temp2->pRightSibling != temp2)
					{						
						temp = temp2->pRightSibling;
						maxPointer = insertFibHeapUtil(maxPointer, temp2);
						temp2 = temp;
					}
					// insert the last of the siblings at root level
					maxPointer = insertFibHeapUtil(maxPointer, temp2);
				}
			}
			else
			{
				// if all the nodes are removed from the Fibonacci heap, set the maxPointer to NULL
				maxPointer = NULL;
				break;
			}
		}
		else
		{			
			if (!flag)
			{
				outputFile << maxPointer->pData; // File output
				flag = true;
			}
			else
			{
				outputFile << "," << maxPointer->pData; // File output
			}			

			// update the siblings' left and right link before removing the max node
			Node* temp4 = maxPointer->pLeftSibling;
			temp4->pRightSibling = maxPointer->pRightSibling;
			maxPointer->pRightSibling->pLeftSibling = temp4;
			
			maxPointer->pLeftSibling = maxPointer;
			maxPointer->pRightSibling = maxPointer;
			maxPointer->pChild = NULL;
			maxPointer->pRankValue = 0;

			//insert the max node into the buffer array to reinsert after meld
			buffer[bufCount++] = maxPointer;
			maxPointer = NULL;

			// update maxPointer at root level by checking the frequency of each of the nodes at the root level
			int max = -1;
			Node* temp5 = temp4;
			
			while (temp4->pRightSibling != temp5)
			{
				if (temp4->pFrequency >= max)
				{
					max = temp4->pFrequency;
					maxPointer = temp4;
				}
				temp4 = temp4->pRightSibling;
			}
			if (temp4->pFrequency >= max)
				maxPointer = temp4;
			if (maxPointer == NULL)
				maxPointer = temp5;

			
			// check for children of the max node and update its level to root
			if (temp != NULL)
			{
				// check for siblings of child
				
				if (temp->pRightSibling == temp && temp->pLeftSibling == temp)
				{
					// No siblings for child
					maxPointer = insertFibHeapUtil(maxPointer, temp);
				}
				else
				{
					// siblings are present for child
					Node* temp2 = temp;
					while (temp2->pRightSibling != temp2)
					{						
						temp = temp2->pRightSibling;
						maxPointer = insertFibHeapUtil(maxPointer, temp2);
						temp2 = temp;
					}
					maxPointer = insertFibHeapUtil(maxPointer, temp2);
				}
			}
			else
			{				
				//maxPointer = NULL;
			}
		}
		num--;
		// Invoke meld function after every max node removal from the Fibonacci heap
		maxPointer = meld(maxPointer);
	}
	outputFile << endl; // File output
	

	// Reinsert all the max nodes removed back into the Fibonacci heap
	for (int i = 0; i < bufCount; i++)
	{
		maxPointer = insertFibHeapUtil(maxPointer, buffer[i]);
	}	
	return maxPointer;
}


// Function for inserting a new node in the Fibonacci heap
Node* insertFibHeap(Node* maxPointer, string data, int frequencyValue)
{
	Node* node = new Node(data, frequencyValue);
	// ChildCut value undefined for root level nodes
	node->pChildCutValue = -1;
	node->pParent = node;
	// Check if new node is greater than existing max in Fib heap
	//Insert in top level and Update maxPointer to the new node
	Node* temp = maxPointer;

	// Insert to immediate right of max pointer
	temp->pRightSibling->pLeftSibling = node;
	node->pRightSibling = temp->pRightSibling;
	temp->pRightSibling = node;
	node->pLeftSibling = temp;

	// Insert the new node in the hashTable map
	hashTable.insert(make_pair(data, node));
	// return the node with maximum frequency as the pointer to max node
	if (frequencyValue >= maxPointer->pFrequency)
	{
		return node;
	}
	else
	{
		return maxPointer;
	}
}

// Utility functin for shifting the updatedNode to root level
Node* shiftToRootLevel(Node* maxPointer, Node* updatedNode)
{
	// Move the node to immediate right of max pointer and update the corresponding fields
	
	if (maxPointer == NULL)
	{
		// There are no nodes in Fibonacci heap
		updatedNode->pParent = updatedNode;
		updatedNode->pChildCutValue = -1;
		updatedNode->pLeftSibling = updatedNode;
		updatedNode->pRightSibling = updatedNode;
		return updatedNode;
	}

	// If there are nodes already present in the Fibonacci heap
	// Inseert the updatedNode to the right of maxPointer and update accordingly
	updatedNode->pRightSibling = maxPointer->pRightSibling;
	maxPointer->pRightSibling->pLeftSibling = updatedNode;
	maxPointer->pRightSibling = updatedNode;
	updatedNode->pLeftSibling = maxPointer;
	updatedNode->pChildCutValue = -1;
	updatedNode->pParent = updatedNode;


	// return the node with maximum frequency as the pointer to max node
	if (updatedNode->pFrequency >= maxPointer->pFrequency)
	{
		return updatedNode;
	}
	else
		return maxPointer;
}

// Utility function to perform cascadeCut operation in a Fibonacci heap
Node* updateParentRecursive(Node* maxPointer, Node* updatedParentNode)
{
	// updatedParentNode Child cut value is True, hence remove the sub tree and move it to root level
	// Check and update the parent of updateParentNode
	if (updatedParentNode->pChildCutValue == -1 || updatedParentNode->pChildCutValue == 0)
	{
		return maxPointer;
	}
	Node* tempParent = updatedParentNode->pParent;
	// check if updatedParentNode has siblings
	if ((updatedParentNode->pLeftSibling == updatedParentNode) && (updatedParentNode->pRightSibling == updatedParentNode))
	{
		tempParent->pChild = NULL;
		updatedParentNode->pLeftSibling = updatedParentNode;
		updatedParentNode->pRightSibling = updatedParentNode;
		updatedParentNode->pParent = updatedParentNode;

		// Decrement the rank of parent of updatedParent node
		tempParent->pRankValue--;

		// Update the child cut value of the parent of updatedParent node accordingly
		if (tempParent->pChildCutValue == -1)
		{
			// Child cut value undefined - root level
			return (shiftToRootLevel(maxPointer, updatedParentNode));
		}
		else if (tempParent->pChildCutValue == 0)
		{
			// Child cut value 0 - False
			// Update the parent's child cut value to 1 - True
			tempParent->pChildCutValue = 1;
			return (shiftToRootLevel(maxPointer, updatedParentNode));
		}
		else
		{
			// Child cut value is 1 - True			
			maxPointer = shiftToRootLevel(maxPointer, updatedParentNode);
			// Shift the parent of updatedParent node also to root level
			return (updateParentRecursive(maxPointer, tempParent));
		}
	}
	else // Siblings exist for updatedParentNode
	{
		Node* temp = updatedParentNode->pLeftSibling;
		updatedParentNode->pRightSibling->pLeftSibling = temp;
		temp->pRightSibling = updatedParentNode->pRightSibling;

		// check if updatedParentNode is the child of its parent and update accordingly
		if (tempParent->pChild == updatedParentNode)
		{
			tempParent->pChild = updatedParentNode->pLeftSibling;
		}
		updatedParentNode->pLeftSibling = updatedParentNode;
		updatedParentNode->pRightSibling = updatedParentNode;
		updatedParentNode->pParent = updatedParentNode;

		// Decrement the rank of parent of updatedParent node
		tempParent->pRankValue--;
		if (tempParent->pChildCutValue == -1)
		{
			// Child cut value undefined - root level
			return (shiftToRootLevel(maxPointer, updatedParentNode));
		}
		else if (tempParent->pChildCutValue == 0)
		{
			// Child cut value 0 - False
			// Update the parent's child cut value to 1 - True
			tempParent->pChildCutValue = 1;
			return (shiftToRootLevel(maxPointer, updatedParentNode));
		}
		else
		{
			// Child cut value is 1 - True
			maxPointer = shiftToRootLevel(maxPointer, updatedParentNode);
			// Shift the parent of updatedParent node also to root level
			return (updateParentRecursive(maxPointer, tempParent));
		}
	}
}



// Function for increasing the frequency of the hashtag if already present in the  Fibonacci heap and perform cascade cut if neccessary
Node* increaseKey(Node* maxPointer, Node* updatedNode)
{
	Node* tempParent = updatedNode->pParent;
	// Frequency value is already increased prior to this function call, just check for the balance in the heap
	// if the increased key node is at root level, just check for max value
	if (updatedNode->pChildCutValue == -1)
	{
		// Child cut value = -1, undefined - Root level
		// Update maxPointer
		if (updatedNode->pFrequency >= maxPointer->pFrequency)
		{
			return updatedNode;
		}
		return maxPointer;
	}
	
	// Check if the updatedNode's frequency value is greater than or equal to the parent value if parent exists
	if ((updatedNode->pParent == updatedNode) || (updatedNode->pParent->pFrequency > updatedNode->pFrequency))
	{
		// Node is in the root level. So ignore
		return maxPointer;
	}
	else
	{
		
		// Before removing the node, check if it has left and right siblings and update accordingly.
		// Also update the child pointer of the parent node.
		
		// updatedNode is the only child has no siblings
		if ((updatedNode->pLeftSibling == updatedNode) && (updatedNode->pRightSibling == updatedNode))
		{
			// Update parent node's child pointer
			updatedNode->pParent->pChild = NULL;
			updatedNode->pParent->pRankValue--;
			updatedNode->pParent = updatedNode;			

			maxPointer = shiftToRootLevel(maxPointer, updatedNode);
		}
		// updatedNode has siblings
		else
		{
			Node* temp = updatedNode->pLeftSibling;
			temp->pRightSibling = updatedNode->pRightSibling;
			updatedNode->pRightSibling->pLeftSibling = temp;			
			
			// update parent's child pointer to the immediate left sibling of updatedNode only if the parent node's child pointer points to updatedNode
			if (updatedNode->pParent->pChild == updatedNode)
			{
				updatedNode->pParent->pChild = updatedNode->pLeftSibling;
			}
			updatedNode->pParent->pRankValue--;
			updatedNode->pParent = updatedNode;
			updatedNode->pLeftSibling = updatedNode;
			updatedNode->pRightSibling = updatedNode;
			maxPointer = shiftToRootLevel(maxPointer, updatedNode);
		}

		// if parent node is at root level, child cut value -1, undefined
		if (tempParent->pChildCutValue == -1)
		{
			// Do nothing
			return maxPointer;
		}
		//if parent node child cut value is 0, false
		else if (tempParent->pChildCutValue == 0)
		{
			// Update parent child cut value from False to True, 0 to 1
			tempParent->pChildCutValue = 1;
			return maxPointer;
		}
		//if parent node child cut value is 1, true
		else
		{
			// Remove the parent-children tree and move them to root level
			maxPointer = updateParentRecursive(maxPointer, tempParent);
			return maxPointer;
		}

	}
}

// Function to remove the node associated with the hashtag
Node* remove(Node* maxPointer, string data)
{
	return maxPointer;
}
#endif // !__MAIN__
