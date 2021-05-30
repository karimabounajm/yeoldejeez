//
// testFacility.cpp
//

#include "Facility.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
  string line;
  getline(cin,line);
  Facility f(line);
  cout << f.site_number() << " " << f.type() << " " << f.code() << " "
       << f.name() << " ";
  cout << setw(12) << setprecision(4) << fixed << f.latitude() << " "
       << setw(12) << setprecision(4) << fixed << f.longitude() << " ";
  cout << f.distance(40,-100) << endl;
}
