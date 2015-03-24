#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QPointF>
#include <QColor>

#include "utility.h"

class Intersection
{
public:
    Intersection();
    ~Intersection();

    QPointF getPoint();
    void setPoint( QPointF point );

    QColor getColor();
    void setColor( QColor color );

    double getSpeedLimit();
    void setSpeedLimit( double speedLimit );

    std::vector<Lane> getLanes();
    void setLanes( std::vector<Lane> lanes );
    void addLane( Lane lane );

private:
    QPointF point;
    QColor color;

    double speedLimit;

    std::vector<Lane> lanes;
};

#endif // INTERSECTION_H
