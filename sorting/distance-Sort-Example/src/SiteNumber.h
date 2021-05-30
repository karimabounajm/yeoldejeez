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


struct SiteNumber
{
	string siteNumber;
	SiteNumber(string sn): siteNumber(sn) {}

	bool operator() (const Runway* runway) {
		return( ( siteNumber.compare(runway->site_number()) ) == 0 );
	}
};