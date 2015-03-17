#ifndef PAINTER_H
#define PAINTER_H

#include <QImage>
#include <QPainter>
#include <QFile>
#include <string>

#include <roadnetwork.h>

class RoadNetwork;
class Painter
{
public:
    Painter( RoadNetwork* roadNetwork );
    ~Painter();

    void initRoadImage();
    void initRoadPainter();
    void endRoadPainter();

    void drawRoadNetwork();
    void drawRoads();
    void drawTrafficLights();
    void drawBoundary();

    QImage getRoadImage();

private:
    RoadNetwork* roadNetwork;

    QImage roadImage;
    QPainter roadPainter;

    // string function parameter sendt
    // med i stedet for alle functionerne neden under

    void drawRoadsType( std::string type );
};

#endif // PAINTER_H
