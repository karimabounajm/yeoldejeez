//
// testRunway.cpp
//

#include "Runway.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
  string line;
  getline(cin,line);
  Runway r(line);
  cout << r.site_number() << " " << r.name() << " "
       << setw(8) << r.length() << endl;
}
