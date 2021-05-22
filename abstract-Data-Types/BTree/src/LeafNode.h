#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
private:
  int *values;

public:
  // constructor and destructor
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  // ~LeafNode();

  // manipulating values in the array of values
  void addToLeft(int value, int last);
  void addToRight(int value, int last);
  void addToThis(int value);
  void addFromLeft();
  void addFromRight();
  void removeFromThis(int value);
  
  // inserting values
  LeafNode* insert(int value); 
  LeafNode* split(int value, int last);
  void addValue(int value, int &last);

  // removing values
  LeafNode* remove(int value); 

  // merging nodes in leaf nodes
  void mergeLeft();
  void mergeRight();
  virtual void combArrLeft();
  virtual void combArrRight();  
  
  // returning and deleting values
  int getMaximum() const;
  int getMinimum() const;
  int popMinimum();
  int popMaximum();
  virtual BTreeNode* popMin();
  virtual BTreeNode* popMax();
  int* getValues();
  LeafNode* getLeafLeft();
  LeafNode* getLeafRight();
  InternalNode* getParent();
  
  // helper/debugger functions
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
