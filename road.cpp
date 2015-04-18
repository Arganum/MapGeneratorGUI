#include "road.h"

Road::Road()
{
    Road::isOneWay = "no";
}

Road::~Road()
{

}

std::string Road::getName()
{
    return Road::name;
}

void Road::setName( std::string name )
{
    Road::name = name;
}

std::string Road::getType()
{
    return Road::type;
}

void Road::setType( std::string type )
{
    Road::type = type;
}

std::string Road::getIsOneWay()
{
    return Road::isOneWay;
}

void Road::setIsOneWay( std::string isOneWay )
{
    Road::isOneWay = isOneWay;
}

double Road::getSpeedLimit()
{
    return Road::speedLimit;
}

void Road::setSpeedLimit( double speedLimit )
{
    Road::speedLimit = speedLimit;
}

std::vector<Node> Road::getNodes()
{
    return Road::nodes;
}

void Road::setNodes( std::vector<Node> nodes )
{
    Road::nodes = nodes;
}

void Road::addNode( Node node )
{
    Road::nodes.push_back( node );
}

void Road::clearNodes()
{
    Road::nodes.clear();
}

std::vector<unsigned long> Road::getNodeIDs()
{
    return Road::nodeIDs;
}

void Road::setNodeIDs(std::vector<unsigned long> nodeIDs )
{
    Road::nodeIDs = nodeIDs;
}

void Road::addNodeID( unsigned long nodeID )
{
    Road::nodeIDs.push_back( nodeID );
}

void Road::clearNodeIDs()
{
    Road::nodeIDs.clear();
}

std::vector<QLineF> Road::getLines()
{
    return Road::lines;
}

void Road::setLines( std::vector<QLineF> lines )
{
    Road::lines = lines;
}

void Road::addLine( QLineF line )
{
    Road::lines.push_back( line );
}

void Road::clearLines()
{
    Road::lines.clear();
}

QColor Road::getColor()
{
    return Road::color;
}

void Road::setColor( QColor color )
{
    Road::color = color;
}

double Road::getLength()
{
    return Road::length;
}

void Road::setLength( double length )
{
    Road::length = length;
}

