#ifndef LANE_H
#define LANE_H

#include <string>
#include <QPoint>
#include <QColor>

#include "utility.h"

class Lane
{
public:
    Lane();
    ~Lane();

    double getLength();
    void setLength( double length );

    QPointF getPoint();
    void setPoint( QPointF point );

    QColor getColor();
    void setColor( QColor color );

    double getSpeedLimit();
    void setSpeedLimit( double speedLimit );

    std::string getIsOneWay();
    void setIsOneWay( std::string isOneWay );

private:
    double length;
    QPointF point;
    QColor color;

    double speedLimit;

    std::string isOneWay;
};

#endif // LANE_H
