//
// Facility.cpp
//

#include"Facility.h"
#include"gcdistance.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

Facility::Facility(std::string s):  site_number_(s.substr(0,10)), 
                                    type_(s.substr(11, 13)),
                                    code_(s.substr(24, 4)),
                                    name_(s.substr(130, 50)),
                                    latitude_(convert_latitude(s.substr(535, 12))),
                                    longitude_(convert_longitude(s.substr(562, 12))) {}


double Facility::convert_latitude(std::string s) const {
  if (s.back() == 'N') {
    return ( stod(s.substr(0, s.size() - 1)) / 3600 );
  }

  else {
    return ( - (stod(s.substr(0, s.size() - 1)) / 3600) );
  }
}

double Facility::convert_longitude(std::string s) const {

  if (s.back() == 'E') {
    return ( stod(s.substr(0, s.size() - 1)) / 3600 );
  }

  else {
    return ( - (stod(s.substr(0, s.size() - 1)) / 3600) );
  }
}

string Facility::site_number(void) const {
  return site_number_;
}

string Facility::type(void) const {
  return type_;
}

string Facility::code(void) const {
  return code_;
}

string Facility::name(void) const {
  return name_;
}

double Facility::latitude(void) const {
  return latitude_;
}

double Facility::longitude(void) const {
  return longitude_;
}

double Facility::distance(double lat, double lon) const {
  return gcdistance(this->latitude_, this->longitude_, lat, lon);
}