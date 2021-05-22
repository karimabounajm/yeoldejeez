#include <iostream>
#include <climits>
#include "InternalNode.h"

using namespace std;


InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()

// InternalNode::~InternalNode() {
//   cout << "destroying internal node" << endl;
// }

/*
given a suitable position, try adding a pointer to the array of pointers 
children, and return pointer to the node that must be moved to the left 
or right sibling */
BTreeNode* InternalNode::addPtr(BTreeNode *ptr, int pos) { 
  // called when original was full, pos is where the node belongs.
  if(pos == internalSize) {
    return ptr;
  }

  BTreeNode *last = children[count - 1];

  for(int i = count - 2; i >= pos; i--) {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  } // shift things to right to make room for ptr, i can be -1!

  children[pos] = ptr;  // i will end up being the position that it is inserted
  keys[pos] = ptr->getMinimum();
  ptr->setParent(this);
  return last;
} // InternalNode:: addPtr()

/*
adding value to neighbor, which occurs when they have available
space and we would like to insert values; */
void InternalNode::addToLeft(BTreeNode *last) {
  ((InternalNode*)leftSibling)->insert(children[0]);

  for(int i = 0; i < count - 1; i++) {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
  }

  children[count - 1] = last;
  keys[count - 1] = last->getMinimum();
  last->setParent(this);
  if(parent)
    parent->resetMinimum(this);
} // InternalNode::ToLeft()

void InternalNode::addToRight(BTreeNode *ptr, BTreeNode *last) {
  ((InternalNode*) rightSibling)->insert(last);
  if(ptr == children[0] && parent)
    parent->resetMinimum(this);
} // InternalNode::addToRight()

/* 
method combining adding values to and removing values from the 
current InternalNode */
void InternalNode::addToThis(BTreeNode *ptr, int pos) {  
  // pos is where the ptr should go, guaranteed 
  // count < internalSize at start
  int i;

  for(i = count - 1; i >= pos; i--) {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  } // shift to the right to make room at pos

  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  count++;
  ptr->setParent(this);

  if(pos == 0 && parent)
    parent->resetMinimum(this);
} // InternalNode::addToThis()

void InternalNode::removeFromThis(int pos) {  
  // pos is where the ptr should go, guaranteed 
  // count < internalSize at start
  for(int i = pos; i < count - 1; i++) {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
    resetMinimum(children[i]);
  } count--;

  if(this->getParent()) {
    parent->resetMinimum(this);
  }
} // InternalNode::removeFromThis()

/*
methods for returning maximum and minimum, as well as returning
and then removing maxmium and minimum */
int InternalNode::getMaximum() const {
  if(count > 0) // should always be the case
    return children[count - 1]->getMaximum();
  else
    return INT_MAX;
}  // getMaximum();

int InternalNode::getMinimum() const {
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()

int InternalNode::popMinimum() { return 0; }
int InternalNode::popMaximum() { return 0; }

InternalNode* InternalNode::getParent() {
  return parent;
}

InternalNode* InternalNode::getInternalLeft() {
  return (InternalNode*) leftSibling;
}
InternalNode* InternalNode::getInternalRight() {
  return (InternalNode*) rightSibling;
}

BTreeNode* InternalNode::popMax() { 
  // cout << "InternalNode::popMin called" << endl;
  count--;
  return children[count];
}
BTreeNode* InternalNode::popMin() { 
  // cout << "InternalNode::popMax called" << endl;
  BTreeNode* buf = children[0]; 
  for(int i = 0; i < count; i++) {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
  }
  count--;

  if(parent)
    parent->resetMinimum(this);
  return buf;
}

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2) { 
  // Node must be the root, and node1
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count = 2;
  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) { 
  // called by sibling so either at beginning or end
  int pos;

  if(newNode->getMinimum() <= keys[0]) // from left sibling
    pos = 0;
  else // from right sibling
    pos = count;

  addToThis(newNode, pos);
} // InternalNode::insert(BTreeNode *newNode)

/* 
method for inserting values to the B+ list, taking into account cases
such as free space to the left or right, and if both of these cases 
are impossible splitting the current node into two separate nodes and 
returning a pointer to the node that called this method, which 
indicates to the parent InternalNode should itself call its insert 
method */
InternalNode* InternalNode::insert(int value) {  
  // count must always be >= 2 for an internal node
  int pos; // will be where value belongs

  for(pos = count - 1; pos > 0 && keys[pos] > value; pos--);

  BTreeNode *last, *ptr = children[pos]->insert(value);
  if(!ptr)  // child had room.
    return NULL;

  if(count < internalSize) {
    addToThis(ptr, pos + 1);
    return NULL;
  } // if room for value

  last = addPtr(ptr, pos + 1);

  if(leftSibling && leftSibling->getCount() < internalSize) {
    addToLeft(last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < internalSize)
    {
      addToRight(ptr, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(last);
} // InternalNode::insert()

/* 
method for removing values from the B+ list, taking into account cases
such as free elements to the left or right, capacity to merge left or 
right (at least one of the 4 special cases must be true for the method 
to be called). returns a pointer to the InternalNode that calls the 
method, returning the value to the parent InternalNode, which indicates 
to it that the node should be removed, calling its own remove method */
BTreeNode* InternalNode::remove(int value) {  
  int pos = 0; // init var that'll hold index of val to remove

  // finding the position in the array of keys that holds the closest 
  // value to what we want removed
  for(pos = count - 1; pos > 0 && keys[pos] > value; pos--) {  }
  BTreeNode *ptr = children[pos]->remove(value);

  // if the LeafNode or potentially InternalNode that returns a value 
  // from this call to remove returns a NULL, no further changes are 
  // needed, however if it returns a value that pointer must be removed
  if(!ptr) return NULL;
  else removeFromThis(pos); 

  resetMinimum(children[0]);

  // checking if the InternalNode requires a change after the removal of 
  // the node returned by the call the remove in a child node
  if(count < (internalSize + 1) / 2) {
    if(leftSibling && leftSibling->getCount() > (internalSize + 1) / 2) {
      // cout << "InternalNode::remove  adding from left" << endl;
      addFromLeft(); 
      return NULL;
    }

    else if(leftSibling) {
      // cout << "InternalNode::remove  merging to left" << endl;
      mergeLeft();
      return this;
    }

    else if(rightSibling && rightSibling->getCount() > (internalSize + 1) / 2) {
      // cout << "InternalNode::remove  adding from right" << endl;
      addFromRight(); 
      return NULL;
    }

    else if(rightSibling) {
      // cout << "InternalNode::remove  merging to right" << endl;
      mergeRight();
      return this;
    }

    else if(!parent) {
      return children[0];
    }

    else {
      // cout << "InternalNode::remove  shifting up, no merge/take possible, should not fire" << endl;
      return NULL;
    }
  }
  else {
    if(!parent && count == 1) {
      // cout << "InternalNode::remove  shifting up, no merge/take possible" << endl;
      return children[0];
    }
    else {
      // cout << "InternalNode::remove  nothing further needed here" << endl;
      return NULL;
    }
  }
} // InternalNode::remove(int value)

/*
method for resetting the minimum value of a key to the minimum 
of the array of values in a new node, and if that node is the 
only one and a parent exists, it resets the minimum of that parent 
with this node, effectively pushing up the array */
void InternalNode::resetMinimum(const BTreeNode* child) {
  for(int i = 0; i < count; i++)
    if(children[i] == child) {
      keys[i] = children[i]->getMinimum();
      if(i == 0 && parent)
        parent->resetMinimum(this);
      break;
    }
} // InternalNode::resetMinimum()

/*
method for splitting an InternalNode into two nodes */
InternalNode* InternalNode::split(BTreeNode *last) {
  InternalNode *newptr = new InternalNode(internalSize, leafSize,
    parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(newptr);

  rightSibling = newptr;

  for(int i = (internalSize + 1) / 2; i < internalSize; i++) {
    newptr->children[newptr->count] = children[i];
    newptr->keys[newptr->count++] = keys[i];
    children[i]->setParent(newptr);
  }

  newptr->children[newptr->count] = last;
  newptr->keys[newptr->count++] = last->getMinimum();
  last->setParent(newptr);
  count = (internalSize + 1) / 2;
  return newptr;
} // split()

/* 
display method for output all the values in an internal node */
void InternalNode::print(Queue <BTreeNode*> &queue) {
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

/*
merging the two leaf nodes together in regards to its pointers and values, calling the
previously defined methods to effectively combine two nodes, left to right and right 
left */
void InternalNode::combArrRight() {
  for(int i = 0; i < count; i++) {
    ((InternalNode*) rightSibling)->insert(children[i]);
  }  
}

void InternalNode::combArrLeft() {
  for(int i = 0; i < count; i++) {
    ((InternalNode*) leftSibling)->insert(children[i]);
  }
}

void InternalNode::mergeLeft() {
  this->combArrLeft();
  if(((InternalNode*)getInternalLeft())->getParent())
    ((InternalNode*)getInternalLeft())->getParent()->resetMinimum(getInternalLeft());

  if(this->getInternalRight()) {
    this->getInternalLeft()->setRightSibling(this->getInternalRight());
    this->getInternalRight()->setLeftSibling(this->getInternalLeft());
  }
  else {
    this->getInternalLeft()->setRightSibling(NULL);
  }
}

void InternalNode::mergeRight() {
  this->combArrRight();
  if(((InternalNode*)getInternalRight())->getParent()) {
    ((InternalNode*)getInternalRight())->getParent()->resetMinimum(getInternalRight());
    // (InternalNode*)getInternalRight())->getParent()->resetMinimum(getInternalRight());
  }
  
  if(this->getInternalLeft()) {
    this->getInternalRight()->setLeftSibling(this->getInternalLeft());
    this->getInternalLeft()->setRightSibling(this->getInternalRight());
  }
  else {
    this->getInternalRight()->setLeftSibling(NULL);
  }
}

/*
adding values from neigbors, which occurs when an element is
deleted from a node that subsequently needs more elements, and
the neighboring leaf nodes have more than the minimum; */
void InternalNode::addFromLeft() {
  if(dynamic_cast<InternalNode*> (this->leftSibling)) 
    this->insert(((InternalNode*)leftSibling)->popMax());

  if(parent)
    parent->resetMinimum(this);
} // InternalNode::AddFromLeft()

void InternalNode::addFromRight() {
  if(dynamic_cast<InternalNode*> (this->rightSibling)) 
    this->insert(((InternalNode*)rightSibling)->popMin());

  if(getInternalRight() && ((InternalNode*)getInternalRight())->getParent())
    ((InternalNode*)getInternalRight())->getParent()->resetMinimum(getInternalRight());
} // InternalNode::addFromRight()