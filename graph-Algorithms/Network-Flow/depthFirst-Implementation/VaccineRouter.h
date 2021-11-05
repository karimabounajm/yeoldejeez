#ifndef VACCINEROUTER_H
#define VACCINEROUTER_H

#include "VaccineRunner.h"
#include "StackAr.h"


class EdgeRoute 
{
public:
	int ID; int flow; 

	EdgeRoute(int IDe, int flowe);
	EdgeRoute();

	friend bool operator <(const EdgeRoute& leftEdgeRoute, const EdgeRoute& rightEdgeRoute) {
		if(leftEdgeRoute.flow > rightEdgeRoute.flow) {
			return true;
		}   else return false;
	}
}; // class EdgeRoute::EdgeRoute()


class BackRoute 
{
public:
	int ID; int backKey; 

	BackRoute(int IDe, int back);
	BackRoute();
}; // class BackRoute::BackRoute()


class Port 
{
public:
	int ID; int utility; 
	int population;
	bool sink; 
    
	EdgeRoute* edges;
    int adj; 
	
    BackRoute pv;

	Port();
	Port(CityInfo cityInfo, int iden);
}; // class Port::Port


class VaccineRouter {
public:
	int sumVacci; int numberCities;
	EdgeRoute final;

	Port* cityArray;
	StackAr<EdgeRoute> solitaire;

	VaccineRouter(CityInfo cities[], int numCities);
    bool runDFS(); 
	int setFlow(int numAirports, int sinks[], int numSinks);
};

#endif /* VACCINEROUTER_H */
