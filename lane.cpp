#include "lane.h"

Lane::Lane()
{

}

Lane::~Lane()
{

}

double Lane::getLength()
{
    return Lane::length;
}

void Lane::setLength( double length )
{
    Lane::length = length;
}

QPointF Lane::getPoint()
{
    return Lane::point;
}

void Lane::setPoint( QPointF point )
{
    Lane::point = point;
}

QColor Lane::getColor()
{
    return Lane::color;
}

void Lane::setColor( QColor color )
{
    Lane::color = color;
}

double Lane::getSpeedLimit()
{
    return Lane::speedLimit;
}

void Lane::setSpeedLimit( double speedLimit )
{
    Lane::speedLimit = speedLimit;
}

std::string Lane::getIsOneWay()
{
    return Lane::isOneWay;
}

void Lane::setIsOneWay( std::string isOneWay )
{
    Lane::isOneWay = isOneWay;
}
