//
// gcdistance.cpp
//
// great circle distance between points (lat1,lon1) and (lat2,lon2)
// input in degrees decimal
// output in nautical miles (NM)

#include "gcdistance.h"
#include <cmath>
double gcdistance(double lat1, double lon1, double lat2, double lon2)
{
  // convert latitudes and longitudes from degrees to radians
  const double lat1r = lat1 * (M_PI/180.0);
  const double lon1r = lon1 * (M_PI/180.0);
  const double lat2r = lat2 * (M_PI/180.0);
  const double lon2r = lon2 * (M_PI/180.0);
  // psi = central angle between the points (lat1,lon1) and
  // (lat2,lon2) on a sphere
  double c = cos(lat1r)*cos(lat2r)*cos(lon1r-lon2r) + sin(lat1r)*sin(lat2r);
  // truncate possible numerical errors in cos to [-1,1] interval
  c = fmin(c,1.0);
  c = fmax(c,-1.0);
  const double psi = acos(c);
  // R_Earth = 6371 km
  // 1 NM = 1.852 km
  // 1 degree = 60.0405 NM on a great circle
  return 60.0405 * psi * (180.0/M_PI);
}
