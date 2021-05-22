#ifndef SPREADER_H
#define SPREADER_H
#include "spreaderRunner.h"
#include "minheap.h"



struct Address {
	Address(int indentification, int inverval);
	Address();
	int ID;
	int period;
};


struct Cost {
	Cost(int indentification, int cost);
	Cost();
	friend bool operator <(const Cost& leftCost, const Cost& rightCost) {
		if(leftCost.time < rightCost.time) {
			return true;
		}
		else return false;
	}
	int ID;
	int time;
};


class Subject { 
public:
	// the ID of the given subject and the last time they were visited, note
	// that visited time is used to keep from traversing further with person;
	int ID; int visited; bool infected;

	// array for the addresses of people visited, and number to iterate to;
	int numVisiting; 
	Address* peopleVisited;

	Subject(Person person);
	Subject();
};


class Spreader {

	// hash table for people, no need to rehash or create a hash 
	// function as everyone has a distinct integer ID
	Subject* tableSubjects;

	// queue to hold all the people who are incubating
	BinaryHeap<Cost> incubating;
  
public:
  Spreader(const Person *people, int population);
  int simulate(int starterIDs[], int starterCount, int commandLineOption);
  void visit(int i);
};



#endif /* SPREADER_H */

