/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include <iostream>

#include "building.h"

#include "json.hpp"
#include <stdexcept>

using json = nlohmann::json;
using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// print
// 
// prints information about a building --- id, name, etc. -- to
// the console. The function is passed the Nodes for searching 
// purposes.
//
void Building::print(Nodes& nodes, BusStops busstops, CURL* curl)
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;

  pair<double, double> loc = this -> getLocation(nodes);

  cout << "Location: (" << loc.first << ", " << loc.second << ")" << endl;

  cout << "Closest southbound bus stop:" << endl;
  closest(loc, "Southbound", busstops, nodes, curl);

  cout << "Closest northbound bus stop:" << endl;
  closest(loc, "Northbound", busstops, nodes, curl);
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}
//
// getLocation
//
// This function finds the location of the building by taking in
// Nodes. It calculates the avg lat and lon and then returns a pair
// of lat lon based on the number of perimeter nodes as well.
//
pair<double, double> Building::getLocation (const Nodes& nodes)
{
  double avgLat = 0.0;
  double avgLon = 0.0;
  int count = 0;

  for (long long nodeid : this -> NodeIDs){// go through the nodes
    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    // the nodes are found
    bool found = nodes.find(nodeid, lat, lon, entrance);

    if(found){
      avgLat += lat;
      avgLon += lon;
      count += 1;
      // update the lat, lon, and count to calculate average
    }

  }
  //calculate averages
  avgLat = (double) avgLat / (double) count;
  avgLon = (double) avgLon / (double) count;
  //print out number of perimeter nodes
  cout << "# perimeter nodes: " << count << endl;

  return make_pair(avgLat, avgLon);
}
//
// closest
//
// This function finds the closest southbound/northbound busstop to the current building.
// This function outputs the information regarding this busstop and then also outputs
// different predictions based on the busstop.
//
void Building::closest(pair<double, double> location, string bound, BusStops busstops, const Nodes& nodes, CURL* curl){
  BusStop close(busstops.MapBusStops[0].stopID, busstops.MapBusStops[0].busRoute, busstops.MapBusStops[0].stopName, busstops.MapBusStops[0].directionOfTravel, busstops.MapBusStops[0].location, busstops.MapBusStops[0].LatLon);
  for(BusStop busstop : busstops.MapBusStops){// iterate through the bus stops
    if(busstop.directionOfTravel == bound){
      close.stopID = busstop.stopID;
      close.busRoute = busstop.busRoute;
      close.stopName = busstop.stopName;
      close.directionOfTravel = busstop.directionOfTravel;
      close.location = busstop.location;
      close.LatLon = busstop.LatLon;
      break;//found the first correct bound bus stop
    }
  }
  double minimum_distance = 0.0;
  minimum_distance = distBetween2Points(location.first, location.second, close.LatLon.first, close.LatLon.second);
  double newDist = 0.0;
  //now find the acutal minimum
  for(BusStop busstop : busstops.MapBusStops){
    if(busstop.directionOfTravel == bound){
      // distance from building to this new bus stop
      newDist = distBetween2Points(location.first, location.second, busstop.LatLon.first, busstop.LatLon.second);
      if(minimum_distance > newDist){// this busstop is closer?
        minimum_distance = newDist;// update minimum and the busstop
        close.stopID = busstop.stopID;
        close.busRoute = busstop.busRoute;
        close.stopName = busstop.stopName;
        close.directionOfTravel = busstop.directionOfTravel;
        close.location = busstop.location;
        close.LatLon = busstop.LatLon;
      }
    }
  }
  // print out information regarding the busstop
  cout << "  " << close.stopID << ": " << close.stopName << ", bus #" << close.busRoute << ", " << close.location << ", " << minimum_distance << " miles" << endl;

  // predictions
  string url = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=LUrrQArPaVMxMnL6gNz3qryYN&rt=" + to_string(close.busRoute) + "&stpid=" + to_string(close.stopID) + "&format=json";
  string name = "";
  string& response = name;
  bool found = callWebServer(curl, url, response);

  if(found){
    auto jsondata = json::parse(response);
    auto bus_response = jsondata["bustime-response"];

    auto predictions = bus_response["prd"];
    // for each prediction (a map) in the list:
    if(predictions.size() == 0){ // means that there were no predictions
      cout << "  <<no predictions available>>" << endl;
    }
    else{
      for (auto& M : predictions)
      {
        try{
          cout << "  vehicle #" << stoi(M["vid"].get_ref<std::string&>()) << " on route " <<
               stoi(M["rt"].get_ref<std::string&>()) << " travelling " << M["rtdir"].get_ref<std::string&>()
               << " to arrive in " << stoi(M["prdctdn"].get_ref<std::string&>()) << " mins" << endl;
        }
        catch(exception &e){
          cout << " error" << endl;
          cout << " malformed CTA response, prediction unavailable"
               << " (error: " << e.what() << ")" << endl;
        }
      }
    }
  }
  else{
    cout << "  <<bus predictions unavailable, call failed>>" << endl;
  }
}
