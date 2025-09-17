#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include "node.h"
#include "nodes.h"

using namespace std;

class BusStop
{
public:
  long long stopID;
  int busRoute;
  string stopName;
  string directionOfTravel;
  string location;
  pair<double, double> LatLon;

  //
  // constructor
  //
  BusStop(long long stopID, int busRoute, string stopName, string directionOfTravel, string location, pair<double, double> LatLon);
  //
  // print
  //
  void print();
};