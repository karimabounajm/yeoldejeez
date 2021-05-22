// Author Sean Davis

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

const int MAX_MEETING_PERIOD = 7;
const int MAX_MEETING_PEOPLE = 10;

class Person
{
public:
  int meetingCount;
  int meetings[20];
  bool operator< (const Person &rhs) const {return meetingCount < rhs.meetingCount;}
}; 

class Meeting
{
public:
  int IDs[MAX_MEETING_PEOPLE];
  int meetingIDs[MAX_MEETING_PEOPLE];
  int IDCount;
  int setSize;
  int period;
  Meeting(): IDCount(0), setSize(0){}
};


int intCmp(const void* ptr1, const void *ptr2)
{
   return *((const int*) ptr1) - *((const int*) ptr2);
}  // intCmp()

void   writeFile(const char *filename, int population, const Person *people, 
   const Meeting *meetings, int starters)
{
 ofstream outf(filename);
 outf << population << ',' << starters;
 
 for(int i = 0; i < starters; i++)
   outf << ',' << rand() % population;

outf << endl; 
 
 for(int i = 0; i < population; i++)
 {
   int peopleMet = 0;
   
   for(int j = 0; j < people[i].meetingCount; j++)
     peopleMet += meetings[people[i].meetings[j]].IDCount - 1;
   
     
   outf << i << ',' << peopleMet;
   
   for(int j = 0; j < people[i].meetingCount; j++)
     for(int k = 0; k < meetings[people[i].meetings[j]].IDCount; k++)
        if(meetings[people[i].meetings[j]].IDs[k] != i)
          outf << ',' << meetings[people[i].meetings[j]].IDs[k] <<  ',' 
            << meetings[people[i].meetings[j]].period;
   
   outf << endl;
   
 } // for each person
}  // writeFile()

int findSetPerson(int meetingID, const Meeting *meetings, int meetingCount)
{
  while(meetings[meetingID].setSize != meetings[meetingID].IDCount) // internal meeting
    meetingID = meetings[meetingID].meetingIDs[rand() % meetings[meetingID].IDCount ];

  
  return meetings[meetingID].IDs[rand() % meetings[meetingID].IDCount];
} // findSetPerson()


void setMeetings(int population, Person *people, Meeting *meetings, int meetingCount)
{
  int *meetingIDs = new int[meetingCount];
  
  for(int i = 0; i < meetingCount; i++)
    meetingIDs[i] = i;
  
  for(int setCount = meetingCount; setCount > 1;)
  {
    int meetingSize = rand() % MAX_MEETING_PEOPLE + 1;
    
    if (meetingSize > setCount)
      meetingSize = setCount;
    
    meetings[meetingCount].IDCount = meetings[meetingCount].setSize = meetingSize;
    meetings[meetingCount].period = rand() % MAX_MEETING_PERIOD + 1;
    
    for(int i = 0; i < meetingSize; i++)
    {
      int pos = rand() % setCount;
      int meetingID = meetingIDs[pos];
      meetings[meetingCount].meetingIDs[i] = meetingID;
      meetings[meetingCount].setSize += meetings[meetingID].setSize;
      int personID = findSetPerson(meetingID, meetings, meetingCount);
      meetings[meetingCount].IDs[i] = personID;
      people[personID].meetings[people[personID].meetingCount++] = meetingCount;
      meetingIDs[pos] = meetingIDs[--setCount];  // replace used meetings
    }  //  for each person in a meeting 
    
    meetingIDs[setCount++] = meetingCount++;  // add meeting to "deck"
  }  // for setCount

  for (int i = 0; i < meetingCount; i++)
    qsort(meetings[i].IDs, meetings[i].IDCount, sizeof(int), intCmp);
  
  delete [] meetingIDs;
} // setMeetings()

int createPopulation(int population, Person *people, Meeting *meetings)
{

  int *worldIDs = new int[population];
  
  for(int i = 0;  i < population; i++)
    worldIDs[i] = i;

  int meetingCount = 0;
  
  for(int worldCount = population; worldCount > 0;)
  {
    int meetingSize = rand() % MAX_MEETING_PEOPLE + 1;
    
    if (meetingSize > worldCount)
      meetingSize = worldCount;
    
    meetings[meetingCount].IDCount = meetings[meetingCount].setSize = meetingSize;
    meetings[meetingCount].period = rand() % MAX_MEETING_PERIOD + 1;
    
    for(int i = 0; i < meetingSize; i++)
    {
      int pos = rand() % worldCount;
      int ID = worldIDs[pos];
      meetings[meetingCount].IDs[i] = ID;
      people[ID].meetings[people[ID].meetingCount++] = meetingCount;
      worldIDs[pos] = worldIDs[--worldCount];
    }
    
    meetingCount++;
  }  // for worldCount

  delete [] worldIDs;
  return meetingCount;
} // createPopulation
int main(int argc, char** argv)
{
  int population = 20, seed = 20, starters = 2;
  char filename[80];
  cout << "Population>> ";
  cin >> population;
  cout << "Number of starters (1 - 10)>> ";
  cin >> starters;
  cout << "Seed>> ";
  cin >> seed;
  sprintf(filename, "covid-%d-%d-%d.csv", population, starters, seed);
  srand(seed);
 
  Person *people = new Person[population];
  Meeting *meetings = new Meeting[population];
  int meetingCount = createPopulation(population, people, meetings);
  setMeetings(population, people, meetings, meetingCount);
  writeFile(filename, population, people, meetings, starters);
  return 0;
} // main()

