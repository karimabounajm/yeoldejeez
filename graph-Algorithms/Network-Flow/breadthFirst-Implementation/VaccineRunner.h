#ifndef VACCINERUNNER_H
#define VACCINERUNNER_H


class Edge
{
public:
  int ID;
  int flow;
  Edge() : flow(0) {}
}; // class Edge

class CityInfo
{
public:
  int ID;
  int population;
  Edge edges[10];
  int adjCount;
  bool sink;
  CityInfo():adjCount(0), sink(false) {}
};


#endif /* VACCINERUNNER_H */

