#include <iostream>
#include <iomanip>
#include <fstream>

#include "BinaryTree.h"
#include "BinaryHeap.h"

using namespace std;


int* getCount(const char* filename) {
	ifstream inf(filename);
	inf >> std::noskipws;
	char c; char s[128];
	int* count = new int[128]();

	// iterating so long as the file stream has values, taking every
	// character one by one and adding it to its place in the values
	// array, and then skipping past a line when it has no values left
	// using getline; note, used noskipws to avoid skipping whitespaces
	while(inf) {
		while(inf >> c) {
			count[int(c)]++;
			// cout << c;
		}
		inf.getline(s, 128);
	} // cout << endl;

	/*
  	// printing out frequency for debugging
	for (int i = 0; i < 127; ++i) {
		if (count[i] > 0) {
			cout << "There are " << count[i] << " " << char(i) << "'s" << endl;
		}
	} */
	return count; 
}

Node* createTree(int* values) {
	BinaryHeap<NodePtr> inorder;
	for(int i = 0; i < 127; i++) {
		if(values[i] > 0) {
			Node* buf = new Node(char(i), values[i]);
			inorder.insert(NodePtr(buf));
		}
	} 

	while(!(inorder.isEmpty())) {
		NodePtr buf1;
		inorder.deleteMin(buf1);
		if(inorder.isEmpty()) {
			return buf1.pointer();
		}
		else {
			NodePtr buf2;
			inorder.deleteMin(buf2);
			Node* buf = new Node(buf1.pointer(), buf2.pointer());
			inorder.insert(NodePtr(buf));
		}
	}
	NodePtr buf;
	inorder.deleteMin(buf);
	return buf.pointer();
}

/* Given a binary tree, print its nodes in inorder*/
void printTree(struct Node* node, int count, bool* path) {
    if(!node) return;
 
    /* first recur on left child */
	path[count] = 0;
    printTree(node->leftChild, ++count, path);
 
    /* then print the data of node */
	if(node->ch != '\0') {
		//   cout << setw(12) << a << '#' << "suck my dick " << endl;
    	cout << node->ch << setw(5) << node->frequency << ' ';
		for(int i = 0; i < count - 1; i++) cout << path[i];
		cout << endl;
	} count--; 
 
    /* now recur on right child */
	path[count] = 1;
    printTree(node->rightChild, ++count, path);
}


int main(int argc, char const *argv[]) {
	const char* filename = argv[1];
	int* values = getCount(filename);
	
	Node* inorder = createTree(values);
	int count = 0;
	bool* path = new bool[63];
	printTree(inorder, count, path);
	return 0;
}
