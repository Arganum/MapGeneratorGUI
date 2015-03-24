#include "intersection.h"

Intersection::Intersection()
{

}

Intersection::~Intersection()
{

}

QPointF Intersection::getPoint()
{
    return Intersection::point;
}

void Intersection::setPoint( QPointF point )
{
    Intersection::point = point;
}

QColor Intersection::getColor()
{
    return Intersection::color;
}

void Intersection::setColor( QColor color )
{
    Intersection::color = color;
}

double Intersection::getSpeedLimit()
{
    return Intersection::speedLimit;
}

void Intersection::setSpeedLimit( double speedLimit )
{
    Intersection::speedLimit = speedLimit;
}

std::vector<Lane> Intersection::getLanes()
{
    return Intersection::lanes;
}

void Intersection::setLanes( std::vector<Lane> lanes )
{
    Intersection::lanes = lanes;
}

void Intersection::addLane( Lane lane )
{
    Intersection::lanes.push_back( lane );
}
