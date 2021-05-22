#ifndef BTREE_H
  #define BTREE_H

#include "BTreeNode.h"

class BTree
{
  BTreeNode *root;
  int internalSize;
  int leafSize;
public:
  BTree(int ISize, int LSize);
  void insert(int value);
  void print();
  void remove(int value);
}; // BTree class
#endif

