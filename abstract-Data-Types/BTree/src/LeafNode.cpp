#include <iostream>
#include <climits>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right): BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

// LeafNode::~LeafNode() {
//   cout << "destroying LeafNode" << endl;
// }
/*
adding value to neighbor, which occurs when they have available
space and we would like to insert values; */
void LeafNode::addToLeft(int value, int last) {
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::AddToLeft()

void LeafNode::addToRight(int value, int last) {
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

/*
adding values from neigbors, which occurs when an element is
deleted from a node that subsequently needs more elements, and
the neighboring leaf nodes have more than the minimum; */
void LeafNode::addFromLeft() {
  if(dynamic_cast<LeafNode*> (this->leftSibling)) 
    this->insert(getLeafLeft()->popMaximum());

  if(parent)
    parent->resetMinimum(this);
} // LeafNode::AddFromLeft()

void LeafNode::addFromRight() {
  if(dynamic_cast<LeafNode*> (this->rightSibling)) 
    this->insert(getLeafRight()->popMinimum());

  if(getLeafRight() && getLeafRight()->getParent())
    getLeafRight()->getParent()->resetMinimum(getLeafRight());
} // LeafNode::addFromRight()

/* 
method combining adding values to and removing values from the 
current leaf node */
void LeafNode::addToThis(int value) {
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()

void LeafNode::removeFromThis(int value) {
  int i = 0;
  
  for(i = 0; i < count && values[i] < value; ++i) {
    if(values[i] == value) break;
  } if(values[i] != value) return; count--; 
  for (; i < count; ++i) {
    values[i] = values[i + 1];
  }
  
  if(value == values[0] && parent) {
    parent->resetMinimum(this);
  }
} // LeafNode::removeFromThis()

/*
methods for returning maximum and minimum, as well as returning
and then removing maxmium and minimum */
int LeafNode::getMaximum() const {
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()

int LeafNode::getMinimum() const {
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;
} // LeafNode::getMinimum()

int LeafNode::popMaximum() {
  if(count > 0) {
    count--;
    return values[count];
  }
  else
    return 0;
} // LeafNode::popMaximum()

int LeafNode::popMinimum() {
  if(count > 0) {
    int buf = values[0]; 
    for(int i = 0; i < count; i++) {
      values[i] = values[i + 1];
    } count--;
    return buf;
  }
  else {
    // cout << "this should not be happening" << endl;
    return 0;
  }
    
} // LeafNode::popMinimum()

LeafNode* LeafNode::getLeafLeft() {
  return (LeafNode*) leftSibling;
}
LeafNode* LeafNode::getLeafRight() {
  return (LeafNode*) rightSibling;
}

InternalNode* LeafNode::getParent() {
  return (InternalNode*) parent;
}

BTreeNode* LeafNode::popMin() { return NULL; }
BTreeNode* LeafNode::popMax() { return NULL; }

/* 
method for inserting values to the B+ list, taking into account cases
such as free space to the left or right, and if both of these cases 
are impossible splitting the current node into two separate nodes and 
returning a pointer to the node that called this method, which 
indicates to the parent InternalNode should itself call its insert 
method */
LeafNode* LeafNode::insert(int value) {
  int last;

  if(count < leafSize) {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize) {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize) {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

/* 
method for removing values from the B+ list, taking into account cases
such as free elements to the left or right, capacity to merge left or 
right (at least one of the 4 special cases must be true for the method 
to be called). returns a pointer to the leaf node that calls the method,
returning the value to the parent InternalNode, which indicates to it 
that the node should be removed, calling its own remove method */
LeafNode* LeafNode::remove(int value) {   
  int first = values[0];
  removeFromThis(value);

  if(count < (leafSize + 1) / 2 && parent) {
    if(this->getLeafLeft() && leftSibling->getCount() > (leafSize + 1) / 2) {
      // cout << "LeafNode::remove  adding from left" << endl;
      addFromLeft(); 
      return NULL;
    }

    else if(leftSibling) {
      // cout << "LeafNode::remove  merging to left" << endl;
      mergeLeft();
      return this;
    }

    else if(this->getLeafRight() && rightSibling->getCount() > (leafSize + 1) / 2) {
      // cout << "LeafNode::remove  adding from right" << endl;
      addFromRight(); 
      return NULL;
    }

    else if(rightSibling) {
      // cout << "LeafNode::remove  merging to right" << endl;
      mergeRight();
      return this;
    }

    else return this;
  }
  else {
    // cout << "LeafNode::remove  shaving a number off" << endl;
    if(first != values[0] && parent)
        parent->resetMinimum(this);
    return NULL;
  }
} // LeafNode::remove()

/*
finding the largest value among the array values and the integer 
variable value that is passed as a parameter. in this method, if 
value is greater than values[count-1], which is the largest number
in values, then the reference is set to it, and if the value is not 
larger than any in the array the reference to last is set to that 
largest value and value is added to the array, pushing up the necessary
values */
void LeafNode::addValue(int value, int &last) {
  int i;

  if(value > values[count - 1])
    last = value;
  else {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode::addValue()

/* 
returning pointer to the array of values in the leaf node */
int* LeafNode::getValues() { return values; }

/*
methods for merging two leaf node arrays together */
void LeafNode::combArrRight() {
  for (int i = 0; i < getCount(); ++i) {
    this->getLeafRight()->insert(values[i]);
  }  
}

void LeafNode::combArrLeft() {
  for (int i = 0; i < getCount(); ++i) {
    this->getLeafLeft()->insert(values[i]);
  }
}

/*
merging the two leaf nodes together in regards to its pointers and values, calling the
previously defined methods to effectively combine two nodes, left to right and right 
left */
void LeafNode::mergeLeft() {
  this->combArrLeft();
  if(getLeafLeft()->getParent())
      getLeafLeft()->getParent()->resetMinimum(getLeafLeft());
      
  if(this->getLeafRight()) {
    this->getLeafLeft()->setRightSibling(this->getLeafRight());
    this->getLeafRight()->setLeftSibling(this->getLeafLeft());;
  }
  else {
    this->getLeafLeft()->setRightSibling(NULL);
  }
  if(parent)
    parent->resetMinimum(this);
}

void LeafNode::mergeRight() {
  this->combArrRight();
  if(getLeafRight()->getParent())
      getLeafRight()->getParent()->resetMinimum(getLeafRight());

  if(this->getLeafLeft()) {
    this->getLeafRight()->setLeftSibling(this->getLeafLeft());
    this->getLeafLeft()->setRightSibling(this->getLeafRight());
  }
  else {
    this->getLeafRight()->setLeftSibling(NULL);
  }
  if(parent)
    parent->resetMinimum(this);
}

/*
method for splitting a LeafNode into two values */
LeafNode* LeafNode::split(int value, int last) {
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);

  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()

/*
debugging method to print out all the values of an array */
void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()
