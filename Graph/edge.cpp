#include "edge.h"

Edge::Edge()
{

}

Edge::~Edge()
{

}

double Edge::getLength()
{
    return Edge::length;
}

void Edge::setLength( double length )
{
    Edge::length = length;
}

double Edge::getPixelLength()
{
    return Edge::pixelLength;
}

void Edge::setPixelLength( double pixelLength )
{
    Edge::pixelLength = pixelLength;
}

double Edge::getSpeedLimit()
{
    return Edge::speedLimit;
}

void Edge::setSpeedLimit( double speedLimit )
{
    Edge::speedLimit = speedLimit;
}

QColor Edge::getColor()
{
    return Edge::color;
}

void Edge::setColor( QColor color )
{
    Edge::color = color;
}

int Edge::getFrom()
{
    return Edge::from;
}

void Edge::setFrom( int from )
{
    Edge::from = from;
}

int Edge::getTo()
{
    return Edge::to;
}

void Edge::setTo( int to )
{
    Edge::to = to;
}

std::string Edge::getIsOneWay()
{
    return Edge::isOneWay;
}

void Edge::setIsOneWay( std::string isOneWay )
{
    Edge::isOneWay = isOneWay;
}
