// Author Sean Davis

#include <iostream>
#include <fstream>
#include <cstring>
#include "VaccineRunner.h"
#include "VaccineRouter.h"
#include "CPUTimer.h"

using namespace std;


CityInfo* readFile(const char *filename, int *numCities, 
  int *numAirports, int sinks[], int *numSinks)
{
  char comma;
  int ID;
  ifstream inf(filename);
  inf >> *numCities >> comma >> *numAirports  >> comma >> *numSinks;
  CityInfo *cities = new CityInfo[*numCities];
  
  for(int i = 0; i < *numCities; i++)
  {
    inf >> ID >> comma;
    inf >> cities[i].population >> comma >> cities[i].adjCount;
    
    for(int j = 0; j < cities[i].adjCount; j++)
      inf >> comma >> cities[i].edges[j].ID >> comma >> cities[i].edges[j].flow;
  } // for each city
   
  inf.ignore(100, ':');
  
  for(int i = 0; i < *numSinks; i++)
    inf >> comma >> sinks[i] ;
 
  return cities;
}  // readFile()

int main(int argc, char** argv)
{
  int numCities, numAirports, numSinks, flow;
  // int debugFlow;
  int sinks[18000];
  CityInfo *cities =  readFile(argv[1], &numCities, &numAirports, sinks, 
    &numSinks);
  CPUTimer ct;
  VaccineRouter *vaccineRouter = new VaccineRouter(cities, numCities);
  delete [] cities;
  // debugFlow = vaccineRouter->debugSF(numAirports, sinks, numSinks);
  flow = vaccineRouter->setFlow(numAirports, sinks, numSinks);
  // flow = vaccineRouter->debugSF(numAirports, sinks, numSinks);
  cout << "CPU Time: " << ct.cur_CPUTime() << " Flow: " << flow << endl;
  // cout << "Debug Flow:" << debugFlow << endl;
  return 0;
} // main()

