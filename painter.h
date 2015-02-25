#ifndef PAINTER_H
#define PAINTER_H

#include <QImage>
#include <QPainter>
#include <QLineF>
#include <qmath.h>
#include <QFile>

#include <roadnetwork.h>

class RoadNetwork;
class Painter
{
public:
    Painter( RoadNetwork* roadNet );
    ~Painter();

    void initRoadImage();
    void initRoadPainter();
    void endRoadPainter();

    void createRoadLines();
    void createTrafficLightCoordinates();

    void drawRoads();
    void drawBoundary();

    QImage getRoadImage();

private:
    RoadNetwork* roadNetwork;

    QImage roadImage;
    QPainter roadPainter;

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
};

#endif // PAINTER_H
