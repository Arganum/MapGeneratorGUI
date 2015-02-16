#include "roadnetwork.h"

RoadNetwork::RoadNetwork()
{

}

RoadNetwork::~RoadNetwork()
{

}

Bounds RoadNetwork::getBoundary()
{
    return RoadNetwork::boundary;
}

void RoadNetwork::setBoundary( Bounds boundary )
{
    RoadNetwork::boundary = boundary;
}

std::vector<Road> RoadNetwork::getRoads()
{
    return RoadNetwork::roads;
}

void RoadNetwork::setRoads( std::vector<Road> roads )
{
    RoadNetwork::roads = roads;
}

void RoadNetwork::clearRoadsNodes()
{
    for ( std::vector<Road>::iterator it = RoadNetwork::roads.begin();
          it != RoadNetwork::roads.end(); ++it )
    {
        it->clearNodes();
    }
}

void RoadNetwork::clearRoadsNodeIDs()
{
    for ( std::vector<Road>::iterator it = RoadNetwork::roads.begin();
          it != RoadNetwork::roads.end(); ++it )
    {
        it->clearNodeIDs();
    }
}

void RoadNetwork::printBoundary()
{
    std::cout.precision(15);
    std::cout << "RoadNetwork Boundary: " << std::endl;
    std::cout << "  Min Latitude  : " << RoadNetwork::boundary.minLatitude << std::endl;
    std::cout << "  Min Longitude : " << RoadNetwork::boundary.minLongitude << std::endl;
    std::cout << "  Max Latitude  : " << RoadNetwork::boundary.maxLatitude << std::endl;
    std::cout << "  Max Longitude : " << RoadNetwork::boundary.maxLongitude << std::endl;
    std::cout << std::endl;
}

void RoadNetwork::printRoadsNodes()
{
    std::cout.precision(15);

    for ( std::vector<Road>::iterator it = RoadNetwork::roads.begin();
          it != RoadNetwork::roads.end(); ++it )
    {
        std::vector<Node> nodes = it->getNodes();

        std::cout << "Road: " << std::endl;
        std::cout << "  Name  : " << it->getName() << std::endl;
        std::cout << "  Type  : " << it->getType() << std::endl;
        std::cout << "  Nodes : " << nodes.size() << std::endl;

        for ( std::vector<Node>::iterator jt = nodes.begin();
              jt != nodes.end(); ++jt )
        {
            std::cout << "    Node: " << jt->number << std::endl;
            std::cout << "      ID        : " << jt->ID << std::endl;
            std::cout << "      Latitude  : " << jt->latitude << std::endl;
            std::cout << "      Longitude : " << jt->longitude << std::endl;
        }
    }

    std::cout << std::endl;
}

void RoadNetwork::printRoadsNodeIDs()
{
    std::cout.precision(15);

    for ( std::vector<Road>::iterator it = RoadNetwork::roads.begin();
          it != RoadNetwork::roads.end(); ++it )
    {
        std::vector<unsigned long> nodeIDs = it->getNodeIDs();

        std::cout << "Road: " << std::endl;
        std::cout << "  Name    : " << it->getName() << std::endl;
        std::cout << "  Type    : " << it->getType() << std::endl;
        std::cout << "  NodeIDs : " << nodeIDs.size() << std::endl;

        for ( std::vector<unsigned long>::iterator jt = nodeIDs.begin();
              jt != nodeIDs.end(); ++jt )
        {
            std::cout << "    ID: " << *jt << std::endl;
        }
    }

    std::cout << std::endl;
}
