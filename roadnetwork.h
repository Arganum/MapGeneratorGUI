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

    void clearRoadsNodes();
    void clearRoadsNodeIDs();

    void printBoundary();
    void printRoadsNodes();
    void printRoadsNodeIDs();

private:
    Bounds boundary;
    std::vector<Road> roads;
};

#endif // ROADNETWORK_H
