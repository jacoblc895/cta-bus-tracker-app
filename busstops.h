#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include "node.h"
#include "nodes.h"
#include "busstop.h"

using namespace std;

class BusStops
{
public:
  vector<BusStop> MapBusStops;

  //
  // constructor
  //
  BusStops(string fileName);
  //
  // getNumMapBusStops
  //
  // Find number of bus stops
  //
  int getNumMapBusStops();
  //
  // print
  //
  // Prints the bus stops
  //
  void print();
  //
  // sortBusStops
  //
  // Sorts the bus stops
  //
  void sortBusStops();
  //
  // find
  //
  // Finds the bus stop based on the ID
  //
  const BusStop find(long long ID);
};