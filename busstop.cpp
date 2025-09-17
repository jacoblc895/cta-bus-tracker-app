#include <iostream>
#include <fstream>
#include <string>

#include "busstop.h"

using namespace std;


//
// constructor
//
BusStop::BusStop(long long stopID, int busRoute, string stopName, string directionOfTravel, string location, pair<double, double> LatLon)
    : stopID(stopID), busRoute(busRoute), stopName(stopName), directionOfTravel(directionOfTravel), location(location), LatLon(LatLon)
{}
//
// print
//
// print out information regarding the bus stop
//
void BusStop::print(){
    cout << this -> stopID << ": bus " << this -> busRoute << ", " << this -> stopName << ", " << this -> directionOfTravel << ", " <<
            this -> location << ", location (" << this -> LatLon.first << ", " << this -> LatLon.second << ")" << endl;
}