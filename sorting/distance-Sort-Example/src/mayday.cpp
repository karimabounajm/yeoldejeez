//
// mayday.cpp
//

#include "Facility.h"
#include "Runway.h"
#include "Closer.h"
#include "SiteNumber.h"
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

int main(int argc, char **argv)
{

  // use: mayday current_latitude current_longitude min_runway_length
  // latitude and longitudes in degrees decimal
  // min runway length of runway in ft
  assert(argc==4);
  const double current_latitude = atof(argv[1]);
  const double current_longitude = atof(argv[2]);
  const int min_runway_length = atoi(argv[3]);
  


  // load facilities data
  // Insert your code here
  vector<Facility*> facilities;
  ifstream myfile1("Facilities.txt");

  string line;
  while(getline(myfile1, line)) {
      facilities.push_back(new Facility(line));
  }
  
  myfile1.close();

  // sort facilities in order of proximity to the current position
  sort(facilities.begin(), facilities.end(),
       Closer(current_latitude, current_longitude));



  // load runways data
  // Insert your code here
  vector<Runway*> runways;
  ifstream myfile2("Runways.txt");

  while(getline(myfile2, line)) {
      runways.push_back(new Runway(line));
  }

  myfile2.close();



  // list up to 10 nearest facilities that have a long enough runway
  // list each runway that is long enough
    int count = 0;
    for ( unsigned int i = 0; i < facilities.size() && count < 10; i++ ) {
      Facility *a = facilities[i];

      // Find all runways of this facility that are long enough
      vector<Runway*> good_runways;
      SiteNumber cur = SiteNumber(facilities[i]->site_number());
      vector<Runway*>::iterator it = find_if(runways.begin(), runways.end(), cur);
      for(; it < runways.end(); ) {
        if( (*it)->length() >= min_runway_length ) {
          good_runways.push_back(*it);
        }
        it = find_if(it + 1, runways.end(), cur);  
        if (it == runways.end()) break;
      }


      // print this facility if it has long enough runways
      if ( !good_runways.empty() ) {
        // increment count of good facilities
        count++;

        cout << a->type() << " " << a->code() << " "
             << a->name() << " ";
        cout.setf(ios_base::fixed,ios_base::floatfield);
        cout.setf(ios_base::right, ios_base::adjustfield);
        cout.width(5);
        cout.precision(1);
        cout << a->distance(current_latitude, current_longitude)
             << " NM" << endl;

        // print all runways that are long enough
        for ( unsigned int i = 0; i < good_runways.size(); i++ ) {
          Runway *r = good_runways[i];
          cout << "  Runway: " << r->name() << "  length: " << r->length()
               << " ft" << endl;
      }
    }
  }
}