//
// Karim Abou Najm
//

#include "VaccineRouter.h"
#include "VaccineRunner.h"
#include "QueueAr.h"
#include <iostream>
#include <cstring>

using namespace std;

// global static variable for time, is essentially number 
// of iterations of dijkstra's (whole algorithm)
static int worldClock = 1;
static int sizeEdge = sizeof(Edge);

// EdgeRoute::EdgeRoute
EdgeRoute::EdgeRoute() { }
EdgeRoute::EdgeRoute(int IDe, int flowe): ID(IDe), flow(flowe) { }

// BackRoute::BackRoute
BackRoute::BackRoute() { }
BackRoute::BackRoute(int IDe, int back): ID(IDe), backKey(back) { }


// Port::Port
Port::Port() { }
Port::Port(CityInfo cityInfo, int iden) {
	// ID passed as parameter, not stored well in CityInfo
    ID = iden; 
	population = cityInfo.population;
	
	// setting utility and adj to number of outward edges
	// for backedge creation process
	utility = cityInfo.adjCount; adj = cityInfo.adjCount;
	
	// copying the forward edges to the array
	edges = new EdgeRoute[20];
	memcpy(edges, cityInfo.edges, sizeEdge * adj);
} 


// VaccineRouter::VaccineRouter
VaccineRouter::VaccineRouter(CityInfo cities[], int numCities)  {
	sumVacci = 0; numberCities = numCities;
	cityArray = new Port[numCities];
	for(int i = 0; i < numCities; i++) {
		cityArray[i] = Port(cities[i], i);
	} 
} 


// method returning sum of sink vaccinations 
int VaccineRouter::setFlow(int numAirports, int sinks[], int numSinks)
{	
	// setting all the sink cities to being sinks
	for(int i = 0; i < numSinks; i++) {
		cityArray[sinks[i]].sink = 1;
	} 

	// set up backward adjacencies, create keys, reference implementation
	for(int i = 2; i < numberCities; i++) {
		Port& BufferCity1 = cityArray[i];
		if(!BufferCity1.sink) 
		{
			// note that backward edges exist for sinks
			for(int j = 0; j < BufferCity1.adj; j++) {
				Port& BufferCity2 = cityArray[BufferCity1.edges[j].ID];
				BufferCity2.edges[BufferCity2.utility] = EdgeRoute(BufferCity1.ID, 0);

				// incrementing real number of edges
				BufferCity2.utility++;
			} 
		} 
	} 

	// setting number of adjacencies to the sum of outward 
	// and inward edges, and utility to zero to track time
	for(int i = 0; i < numberCities; i++) {
		cityArray[i].adj = cityArray[i].utility;
		cityArray[i].utility = 0;
	} 
	
	// buffer variable for flow determined
	int bufRemove;

	// running BFT while reaching a sink is possible
	while(runBFT()) {
		// incrementing worldclock for visiting
		worldClock++;

		// checking what the flow should be, adjusting population to sink
		if(final.flow > cityArray[final.ID].population) 
		{
			bufRemove = cityArray[final.ID].population;
			cityArray[final.ID].population = 0;
		}
		else 
		{
			bufRemove = final.flow;
			cityArray[final.ID].population -= bufRemove;
		}

		// creating key to final pointer 
		int frontID = final.ID; 

		// iterating through the and subtracting the determined flow
		while(frontID > 1) 
		{
			// increasing flow from frontID to backID
			int backID = cityArray[frontID].pv.ID;
			for(int i = 0; i < cityArray[frontID].adj; i++) {
				if(cityArray[frontID].edges[i].ID == backID) {
					cityArray[frontID].edges[i].flow += bufRemove;
					break;
				}
			}

			// decreasing flow from backID to frontID
			for(int i = 0; i < cityArray[backID].adj; i++) {
				if(cityArray[backID].edges[i].ID == frontID) {
					cityArray[backID].edges[i].flow -= bufRemove;
					break;
				}
			}
			
			// updating the front ID
			frontID = cityArray[frontID].pv.ID;
		} 

		// adding the flow determined to network flow
		sumVacci += bufRemove;
	} 
		
	// returning the sum
	return sumVacci;

}  // VaccineRouter::setFlow()



// running a breadth first traversal 
bool VaccineRouter::runBFT() 
{
	// insert all sources, in this implementation only 2 sources
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < cityArray[i].adj; j++) {
			if(cityArray[i].edges[j].flow > 0) {
				solitaire.enqueue(EdgeRoute(cityArray[i].edges[j].ID, cityArray[i].edges[j].flow));
				cityArray[cityArray[i].edges[j].ID].pv = BackRoute(i, j); 
				cityArray[cityArray[i].edges[j].ID].utility = worldClock;
			} 
		}
	} 

	// if no edges in queue then sources are depleted
	if(solitaire.isEmpty()) return 0;

	// initialize buffer values
	EdgeRoute bufEdge; 
	
	// breadth first search loop
	do
	{
		bufEdge = solitaire.dequeue();

		Port& edgeSource = cityArray[bufEdge.ID];
		for(int i = 0; i < edgeSource.adj; i++) 
		{
			Port& edgeDestination = cityArray[ edgeSource.edges[i].ID ];
			if(edgeDestination.utility != worldClock && edgeSource.edges[i].flow > 0) 
			{				
				// setting the city visited as visited
				edgeDestination.utility = worldClock;
				if(bufEdge.flow > edgeSource.edges[i].flow) 
				{
					edgeDestination.pv = BackRoute(edgeSource.ID, i);
					if(cityArray[edgeDestination.ID].sink && cityArray[edgeDestination.ID].population > 0) {
						final = EdgeRoute(edgeDestination.ID, edgeSource.edges[i].flow);
						solitaire.makeEmpty();
						return 1;
					}
					else {
						cityArray[edgeDestination.ID].utility = worldClock;
						solitaire.enqueue(EdgeRoute(edgeDestination.ID, edgeSource.edges[i].flow));
					}
				}
				else 
				{
					edgeDestination.pv = BackRoute(edgeSource.ID, i);
					if(cityArray[edgeDestination.ID].sink && cityArray[edgeDestination.ID].population > 0) {
						final = EdgeRoute(edgeDestination.ID, bufEdge.flow);
						solitaire.makeEmpty();
						return 1;
					}
					else {
						cityArray[edgeDestination.ID].utility = worldClock;
						solitaire.enqueue(EdgeRoute(edgeDestination.ID, bufEdge.flow));
					}
				}
			}
		}

	// if empty, then no path exists to source
	if(solitaire.isEmpty()) return 0;

	} while(true);
	
	// this should not fire
	return 0;
	
} // VaccineRouter::runDijkstra()