#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "BTree.h"

using namespace std;

int main(int argc, char *argv[])
{
  BTree tree(atoi(argv[2]), atoi(argv[3]));
  int value;
  char s[80], command;

  ifstream inf(argv[1]);

  inf.getline(s, 80);

  while(inf >> command >> value)
  {
    if(command == 'i')
      tree.insert(value);
    else
      tree.remove(value);
  } // while

  tree.print();
  return 0;
}  // main

