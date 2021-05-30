//
// testFacility.cpp
//

#include "Facility.h"
#include <iostream>
#include <iomanip>
#include<fstream>
using namespace std;

int main()
{
  ifstream myfile("checkFacilityValues.txt");
  string line;

  while(getline(myfile, line)) {
    cout << line << endl;
    cout << "\n\n" << endl;
    Facility f(line);
    cout << "The site number is: " <<  f.site_number() << endl;
    cout << "The type is: " << f.type() << endl;
    cout << "The code is: " << f.code() << endl;
    // cout << "The name is: " << f.name() << endl;
    // cout << f.site_number() << " " << f.type() << " " << f.code() << " "
    //      << f.name() << " ";
    // cout << setw(12) << setprecision(4) << fixed << f.latitude() << " "
    //      << setw(12) << setprecision(4) << fixed << f.longitude() << " ";
    cout << "Its distance from position 40, -100 is: " << f.distance(40,-100) << endl;
    cout << "\n\n\n";
  }
}
