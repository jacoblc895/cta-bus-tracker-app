#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include "busstops.h"

using namespace std;


//
// constructor
//
BusStops::BusStops(string fileName){
    ifstream File(fileName);
    string line;
    while(getline(File, line)){

        stringstream parser(line);

        string stopID, busRoute, stopName, direction, location, latitude, longitude;

        getline(parser, stopID, ',');
        getline(parser, busRoute, ',');
        getline(parser, stopName, ',');
        getline(parser, direction, ',');
        getline(parser, location, ',');
        getline(parser, latitude, ',');
        getline(parser, longitude);

        long long StopID = stoll(stopID);
        int BusRoute = stoi(busRoute);
        double lat = stod(latitude);
        double lon = stod(longitude);

        pair<double, double> loc = make_pair(lat, lon);

        BusStop BS(StopID, BusRoute, stopName, direction, location, loc);

        this -> MapBusStops.push_back(BS);
    }
}
//
// getNumMapBusStops
//
// returns number of bus stops
//
int BusStops::getNumMapBusStops(){
    return (int) this->MapBusStops.size();
}
//
// print
//
// Sorts the bus stops in order of the IDs
// and then prints out each bus stop
//
void BusStops::print(){
    sortBusStops();
    
    for(auto B : this -> MapBusStops){
        B.print();
    }
}
//
// sort
//
// Sorts all of the bus stops depending on
// the stop IDs and then sets the MapBusStops
// equal to the sorted version
//
void BusStops::sortBusStops(){
    vector<long long> IDs; // holds all of the ids
    vector<BusStop> New; // will hold the new order of bus stops
    
    for(auto busstop : this -> MapBusStops){
        IDs.emplace_back(busstop.stopID);
        // get all of the ids
    }

    sort(IDs.begin(), IDs.end());
    // sorts the ids

    for(long long ID : IDs){
        // find the bus stop in question
        const BusStop temp = find(ID);
        // add it to the vector now
        New.emplace_back(temp);
    }
    // sets map bus stops to the new one
    this -> MapBusStops = New;
}
//
// find
//
// This finds the bus stop based on the id and
// returns the bus stop.
//
const BusStop BusStops::find(long long ID){
    for(auto stop : this -> MapBusStops){
        if(stop.stopID == ID){// found!
            return stop;
        }
    }
    return this -> MapBusStops[0]; // this will never run, but gets rid of warning
}