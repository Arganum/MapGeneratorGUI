#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <iostream>

#include "road.h"
#include "utility.h"

class RoadNetwork
{
public:
    RoadNetwork();
    ~RoadNetwork();

    Bounds getBoundary();
    void setBoundary( Bounds boundary );

    std::vector<Road> getRoads();
    void setRoads( std::vector<Road> roads );

    std::vector<Node> getTrafficLights();
    void setTrafficLights( std::vector<Node> trafficLights );

    void clearRoads();
    void clearTrafficLights();

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
    std::vector<Node> trafficLights;
};

#endif // ROADNETWORK_H
