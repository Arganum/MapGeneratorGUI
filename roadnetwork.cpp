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

std::vector<TrafficLight> RoadNetwork::getTrafficLights()
{
    return RoadNetwork::trafficLights;
}

void RoadNetwork::setTrafficLights( std::vector<TrafficLight> trafficLights )
{
    RoadNetwork::trafficLights = trafficLights;
}

std::vector<Intersection> RoadNetwork::getIntersections()
{
    return RoadNetwork::intersections;
}

void RoadNetwork::setIntersections( std::vector<Intersection> intersections )
{
    RoadNetwork::intersections = intersections;
}

void RoadNetwork::addIntersection( Intersection intersection )
{
    RoadNetwork::intersections.push_back( intersection );
}

void RoadNetwork::clearRoads()
{
    RoadNetwork::roads.clear();
}

void RoadNetwork::clearTrafficLights()
{
    RoadNetwork::trafficLights.clear();
}

void RoadNetwork::clearIntersections()
{
    RoadNetwork::intersections.clear();
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
    std::cout << "  Width   : " << RoadNetwork::boundary.width << std::endl;
    std::cout << "  Height  : " << RoadNetwork::boundary.height << std::endl;
    std::cout << "    Min Latitude  : " << RoadNetwork::boundary.minLatitude << std::endl;
    std::cout << "    Min Longitude : " << RoadNetwork::boundary.minLongitude << std::endl;
    std::cout << "    Max Latitude  : " << RoadNetwork::boundary.maxLatitude << std::endl;
    std::cout << "    Max Longitude : " << RoadNetwork::boundary.maxLongitude << std::endl;
    std::cout << std::endl;
}

void RoadNetwork::printTrafficLights()
{
    std::cout.precision(15);

    std::cout << "Traffic Light Nodes:" << std::endl;
    for ( std::vector<TrafficLight>::iterator it = RoadNetwork::trafficLights.begin();
          it != RoadNetwork::trafficLights.end(); ++it )
    {
        std::cout << "  Node  : " << it->getNode().number << std::endl;
        std::cout << "    ID        : " << it->getNode().ID << std::endl;
        std::cout << "    Latitude  : " << it->getNode().latitude << std::endl;
        std::cout << "    Longitude : " << it->getNode().longitude << std::endl;

        std::cout << "  Color : " << std::endl;
        std::cout << "    Red   : " << it->getColor().red() << std::endl;
        std::cout << "    Green : " << it->getColor().green() << std::endl;
        std::cout << "    Blue  : " << it->getColor().blue() << std::endl;

        std::cout << "  x     : " << it->getPoint().x() << std::endl;
        std::cout << "  y     : " << it->getPoint().y() << std::endl;
    }
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
        std::cout << "  Color : " << std::endl;
        std::cout << "    Red   : " << it->getColor().red() << std::endl;
        std::cout << "    Green : " << it->getColor().green() << std::endl;
        std::cout << "    Blue  : " << it->getColor().blue() << std::endl;
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
        std::cout << "  Color : " << std::endl;
        std::cout << "    Red   : " << it->getColor().red() << std::endl;
        std::cout << "    Green : " << it->getColor().green() << std::endl;
        std::cout << "    Blue  : " << it->getColor().blue() << std::endl;
        std::cout << "  NodeIDs : " << nodeIDs.size() << std::endl;

        for ( std::vector<unsigned long>::iterator jt = nodeIDs.begin();
              jt != nodeIDs.end(); ++jt )
        {
            std::cout << "    ID: " << *jt << std::endl;
        }
    }

    std::cout << std::endl;
}

void RoadNetwork::printRoadsLines()
{
    std::cout.precision(15);

    for ( std::vector<Road>::iterator it = RoadNetwork::roads.begin();
          it != RoadNetwork::roads.end(); ++it )
    {
        std::vector<QLineF> lines = it->getLines();
        QLineF Temp;
        std::cout << "Road: " << std::endl;
        std::cout << "  Name  : " << it->getName() << std::endl;
        std::cout << "  Type  : " << it->getType() << std::endl;
        std::cout << "  Color : " << std::endl;
        std::cout << "    Red   : " << it->getColor().red() << std::endl;
        std::cout << "    Green : " << it->getColor().green() << std::endl;
        std::cout << "    Blue  : " << it->getColor().blue() << std::endl;
        std::cout << "  Lines : " << lines.size() << std::endl;

        for ( std::vector<QLineF>::iterator jt = lines.begin();
              jt != lines.end(); ++jt )
        {
            std::cout << "    Line: " << "Segment[( "
                      << jt->p1().x() << " , "
                      << jt->p1().y() << " )"
                      << " , ( "
                      << jt->p2().x() << " , "
                      << jt->p2().y() << " )]"
                      << std::endl;

        }
    }

    std::cout << std::endl;
}
