//
// Facility.h
//

#ifndef FACILITY_H
#define FACILITY_H
#include<string>
class Facility
{
  public:
    Facility(std::string s);
    std::string site_number(void) const;
    std::string type(void) const;
    std::string code(void) const;
    std::string name(void) const;
    double latitude(void) const;
    double longitude(void) const;
    double distance(double lat, double lon) const;
  private:
    const std::string site_number_;
    const std::string type_;
    const std::string code_;
    const std::string name_;
    const double latitude_, longitude_;
    double convert_latitude(std::string s) const;
    double convert_longitude(std::string s) const;
};
#endif
