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

