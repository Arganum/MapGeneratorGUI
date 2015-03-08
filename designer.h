#ifndef DESIGNER_H
#define DESIGNER_H

#include <QLineF>
#include <qmath.h>
#include <QColor>
#include <iostream>

#include <roadnetwork.h>

class RoadNetwork;
class Designer
{
public:
    Designer( RoadNetwork* roadNetwork );
    ~Designer();

    void createBoundary();
    void createRoadLines();
    void createTrafficLightCoordinates();

private:
    RoadNetwork* roadNetwork;

    QPointF A,B,C,D;    // The Four cornors  A-------a-------B
                        // of a rectangle    |               |
    QLineF a,b,c,d;     // The Four sides    d               b
                        // of a rectangle    |               |
                        //                   D-------c-------C
    double latLon2Length( double endLatitude, double endLongitude,
                          double beginLatitude, double beginLongitude );
    double deg2Rad( double deg );
    double rad2Deg( double rad );

    QPointF circle2CircleIntersect( double latitude, double longitude);

    QColor createColorScheme( double totalNumber, double number );
};

#endif // DESIGNER_H
