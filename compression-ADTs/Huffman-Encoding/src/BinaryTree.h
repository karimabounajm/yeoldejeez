    #ifndef BinaryTree
    #define BinaryTree

#include <iostream>

struct Node {
	// constructor to be used from array of characters;
	Node() { }
	Node(char c, int freq) {
		ch = c; frequency = freq;
		leftChild = NULL; rightChild = NULL;
	}

	// constructor for non-leaf node, which includes pointers to other 
	// nodes, no character, and a frequency that's the sum of children's
	Node(Node* leftNode, Node* rightNode) {
		ch = '\0'; frequency = leftNode->frequency + rightNode->frequency;
		leftChild = leftNode; rightChild = rightNode;
	}

	// defining variables to hold character and its number 
	// of appearances 
	int frequency; char ch; 
	Node *leftChild, *rightChild;

	// overloading the less than operator for the BinaryHeap
	friend bool operator <(const Node& leftNode, const Node& rightNode) {
		if(leftNode.frequency < rightNode.frequency) {
			return true;
		}
		else return false;
	}

};

struct NodePtr { 
	NodePtr() { ptr = NULL; }
	NodePtr(Node* pointer): ptr(pointer) {}
	Node* ptr;
	Node* pointer() { return ptr; }
	friend bool operator <(const NodePtr& leftNodePtr, const NodePtr& rightNodePtr) {
		if(leftNodePtr.ptr->frequency < rightNodePtr.ptr->frequency) {
			return true;
		}
		else return false;
	}
};

#endif