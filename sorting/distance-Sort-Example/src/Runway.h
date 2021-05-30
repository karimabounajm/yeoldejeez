//
// Runway.h
//

#ifndef RUNWAY_H
#define RUNWAY_H
#include<string>
class Runway
{
  public:
    Runway(std::string s);
    std::string site_number(void) const;
    std::string name(void) const;
    int length(void) const;
  private:
    int convert_length(std::string s) const;
    const std::string site_number_;
    const std::string name_;
    const int length_;
};
#endif
