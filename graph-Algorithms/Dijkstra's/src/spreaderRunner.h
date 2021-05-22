#ifndef SPREADERRUNNER_H
#define SPREADERRUNNER_H

#define INCUBATION_TIME 3

class Visitor
{
public:
  int ID;
  int period;
};

class Person
{
public:
  int ID;
  int peopleMet;
  Visitor *visitors;
};

#endif /* SPREADERRUNNER_H */

