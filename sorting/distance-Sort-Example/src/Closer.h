#include "Facility.h"
#include "Runway.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cmath>
using namespace std;


struct Closer
{
	double latitude, longitude;
	Closer(double current_latitude, double current_longitude): latitude(current_latitude),
															   longitude(current_longitude)
															   {}

	bool operator() (const Facility* first, const Facility* second) {
		return (first->distance(latitude, longitude) < second->distance(latitude, longitude));
	}
};
