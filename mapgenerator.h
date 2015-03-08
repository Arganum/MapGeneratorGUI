#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <QFile>
#include <QFileDialog>
#include <iostream>
#include <QDebug>

#include "roadnetwork.h"
#include "xmlparser.h"
#include "designer.h"
#include "painter.h"
#include "road.h"
#include "utility.h"

class RoadNetwork;
class XmlParser;
class Designer;
class Painter;
class MapGenerator
{
public:
    MapGenerator();
    ~MapGenerator();

    // XmlParsing via. .osm file
    void setOsmFile( QString path );
    void parseOsmFile();
    void deleteOsmParser();

    // Designing
    void createBoundary();
    void createRoadLines();
    void createTrafficLightCoordinates();
    void deleteDesigner();

    // Painting
    void initPainter();
    void drawRoadNetwork();
    void drawRoads();
    void drawTrafficLights();
    void saveRoadImage();
    void deletePainter();

    // RoadNetwork
    void saveTrafficLightsCoordinates();
    void printRoadNetwork();

private:
    RoadNetwork* roadNetwork;

    XmlParser* xmlParser;
    Designer* designer;
    Painter* painter;

    QFile* osmFile;

};

#endif // MAPGENERATOR_H
