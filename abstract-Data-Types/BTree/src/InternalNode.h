#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
private:
  int internalSize;
  BTreeNode **children;
  int *keys;

public:
  // constructor and destructor 
  InternalNode(int ISize, int LSize, InternalNode *p,

  // pointers to potential left and right nodes
  BTreeNode *left, BTreeNode *right);
  // ~InternalNode();


  
  InternalNode* getParent();
  // returning values
  int getMaximum() const;
  int getMinimum() const;
  int popMinimum();
  int popMaximum();
  InternalNode* getInternalLeft();
  InternalNode* getInternalRight();
  virtual BTreeNode* popMin();
  virtual BTreeNode* popMax();

  // inserting values
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  InternalNode* insert(int value); 
  InternalNode* split(BTreeNode *last);
  BTreeNode* addPtr(BTreeNode *ptr, int pos);

  // removing values
  virtual BTreeNode* remove(int value);

  // manipulating values in the array of children
  void addToLeft(BTreeNode *last);
  void addToRight(BTreeNode *ptr, BTreeNode *last);
  void addToThis(BTreeNode *ptr, int pos); // pos is where the node should go
  void addFromLeft();
  void addFromRight();
  void removeFromThis(int pos);

  // merging nodes in leaf nodes
  void mergeLeft();
  void mergeRight();
  virtual void combArrLeft();
  virtual void combArrRight();

  // helper/debugger functions
  void print(Queue <BTreeNode*> &queue);
  
  void resetMinimum(const BTreeNode* child);
  


}; // InternalNode class



#endif
