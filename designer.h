#ifndef DESIGNER_H
#define DESIGNER_H

#include <QLineF>
#include <qmath.h>
#include <QColor>
#include <iostream>

#include "roadnetwork.h"
#include "lane.h"

class RoadNetwork;
class Designer
{
public:
    Designer( RoadNetwork* roadNetwork );
    ~Designer();

    void createBoundary();
    void createRoadLines();
    void createTrafficLightCoordinates();
    void createTrafficLightDirections();
    void createIntersectionCoordinates();
    void createIntersectionLanes();
    void createColorScheme();

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
    void circle2CircleIntersect1();

    QColor createColorScheme( double totalNumber, double number );
    std::vector<Road> splitRoads( std::vector<Road> roads );
    double splitRoadsCount( std::vector<Road> roads );
    std::vector<QPointF> sortIntersectionCoordinates( std::vector<QPointF> coordinates );
};

#endif // DESIGNER_H
