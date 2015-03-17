#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QPoint>
#include <QColor>

#include <utility.h>

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

    std::vector<Lane> getSwitch1();
    void setSwitch1( std::vector<Lane> switch1 );

    std::vector<Lane> getSwitch2();
    void setSwitch2( std::vector<Lane> switch2 );

private:
    Node node;
    QPointF point;
    QColor color;

    std::vector<Lane> switch1;
    std::vector<Lane> switch2;
};

#endif // TRAFFICLIGHT_H
