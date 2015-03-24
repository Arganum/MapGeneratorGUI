#include "trafficlight.h"

TrafficLight::TrafficLight()
{

}

TrafficLight::~TrafficLight()
{

}

Node TrafficLight::getNode()
{
    return TrafficLight::node;
}

void TrafficLight::setNode( Node node )
{
    TrafficLight::node = node;
}

QPointF TrafficLight::getPoint()
{
    return TrafficLight::point;
}

void TrafficLight::setPoint( QPointF point )
{
    TrafficLight::point = point;
}

QColor TrafficLight::getColor()
{
    return TrafficLight::color;
}

void TrafficLight::setColor( QColor color )
{
    TrafficLight::color = color;
}

double TrafficLight::getSpeedLimit()
{
    return TrafficLight::speedLimit;
}

void TrafficLight::setSpeedLimit( double speedLimit )
{
    TrafficLight::speedLimit = speedLimit;
}

std::vector<Lane> TrafficLight::getLanes()
{
    return TrafficLight::lanes;
}

void TrafficLight::setLanes( std::vector<Lane> lanes )
{
    TrafficLight::lanes = lanes;
}

void TrafficLight::addLane( Lane lane )
{
    TrafficLight::lanes.push_back( lane );
}

std::vector<Lane> TrafficLight::getSwitch1()
{
    return TrafficLight::switch1;
}

void TrafficLight::setSwitch1( std::vector<Lane> switch1 )
{
    TrafficLight::switch1 = switch1;
}

std::vector<Lane> TrafficLight::getSwitch2()
{
    return TrafficLight::switch2;
}

void TrafficLight::setSwitch2( std::vector<Lane> switch2 )
{
    TrafficLight::switch2 = switch2;
}

