#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <iostream>

#include "utility.h"
#include "road.h"
#include "trafficlight.h"
#include "intersection.h"

class RoadNetwork
{
public:
    RoadNetwork();
    ~RoadNetwork();

    Bounds getBoundary();
    void setBoundary( Bounds boundary );

    std::vector<Road> getRoads();
    void setRoads( std::vector<Road> roads );

    std::vector<TrafficLight> getTrafficLights();
    void setTrafficLights( std::vector<TrafficLight> trafficLights );

    std::vector<Intersection> getIntersections();
    void setIntersections( std::vector<Intersection> intersections );
    void addIntersection( Intersection intersection );

    void clearRoads();
    void clearTrafficLights();
    void clearIntersections();

    void clearRoadsNodes();
    void clearRoadsNodeIDs();

    void printBoundary();
    void printTrafficLights();
    void printRoadsNodes();
    void printRoadsNodeIDs();
    void printRoadsLines();

private:
    Bounds boundary;
    std::vector<Road> roads;
    std::vector<TrafficLight> trafficLights;
    std::vector<Intersection> intersections;
};

#endif // ROADNETWORK_H
