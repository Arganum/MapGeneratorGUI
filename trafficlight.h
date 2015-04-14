#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QPoint>
#include <QColor>

#include "lane.h"
#include "utility.h"

class TrafficLight
{
public:
    TrafficLight();
    ~TrafficLight();

    Node getNode();
    void setNode( Node node );

    QPointF getPoint();
    void setPoint( QPointF point );

    QColor getColor();
    void setColor( QColor color );

    double getSpeedLimit();
    void setSpeedLimit( double speedLimit );

    std::vector<Lane> getLanes();
    void setLanes( std::vector<Lane> lanes );
    void addLane( Lane lane );

    std::vector<Lane> getSwitch1();
    void setSwitch1( std::vector<Lane> switch1 );

    std::vector<Lane> getSwitch2();
    void setSwitch2( std::vector<Lane> switch2 );

private:
    Node node;
    QPointF point;
    QColor color;

    double speedLimit;

    std::vector<Lane> lanes;
    std::vector<Lane> switch1;
    std::vector<Lane> switch2;
};

#endif // TRAFFICLIGHT_H
