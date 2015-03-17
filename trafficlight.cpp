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

