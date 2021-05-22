#include <iostream>
#include <cstring>
#include "spreader.h"
#include "minheap.h"
using namespace std;


Spreader::Spreader(const Person *people, int population) {
	tableSubjects = new Subject[population];
	for(int i = 0; i < population; ++i) {
		tableSubjects[i] = Subject(people[i]);
	}
} // Spreader::Spreader




int Spreader::simulate(int starterIDs[], int starterCount, int commandLineOption) {
	int timeRet = -1; int time; 
	for(int i = 0; i < starterCount; i++) {
		tableSubjects[starterIDs[i]].infected = 1;
		incubating.insert(Cost(starterIDs[i], 0));
	}
	
	Cost bufCost; Subject bufSubject; Address bufAddress;
	while(!incubating.isEmpty()) {
		incubating.deleteMin(bufCost); 

		if(tableSubjects[bufCost.ID].visited > bufCost.time) {
			if(bufCost.time > timeRet) timeRet = bufCost.time;
			tableSubjects[bufCost.ID].visited = bufCost.time;

			bufSubject = tableSubjects[bufCost.ID];
			for(int i = 0; i < bufSubject.numVisiting; i++) {
				if(!(tableSubjects[bufSubject.peopleVisited[i].ID].infected)) {
					if(bufSubject.visited + 3 <= bufSubject.peopleVisited[i].period) {
						bufAddress = bufSubject.peopleVisited[i];
						time = bufAddress.period;
					}
					else {
						bufAddress = bufSubject.peopleVisited[i];
						int rem = (bufSubject.visited + 3) % bufAddress.period;
						if(rem == 0) time = bufSubject.visited + 3;
						else time = bufSubject.visited + 3 + bufAddress.period - rem;
					}

					tableSubjects[bufAddress.ID].infected = 1;
					incubating.insert(Cost(bufAddress.ID, time));
				}	
			}			
		} 
	}

  	return timeRet; 
} // Spreader::simulate



Subject::Subject() { }

Subject::Subject(Person person) {
	ID = person.ID; numVisiting = person.peopleMet; 
	visited = 4096; infected = 0;

	peopleVisited = new Address[person.peopleMet];
	memcpy(peopleVisited, person.visitors, sizeof(Visitor) * (person.peopleMet));
} // Subject::Subject


Address::Address() { }

Address::Address(int identification, int interval) {
	ID = identification;
	period = interval;
} // Address::Address


Cost::Cost() { }

Cost::Cost(int identification, int cost) {
	ID = identification;
	time = cost;
} // Cost::Cost

