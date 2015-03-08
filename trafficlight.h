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

private:
    Node node;
    QPointF point;
    QColor color;
};

#endif // TRAFFICLIGHT_H
