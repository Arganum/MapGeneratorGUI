#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <QFile>
#include <QFileDialog>
#include <iostream>
#include <QDebug>

#include "utility.h"
#include "roadnetwork.h"
#include "xmlparser.h"
#include "designer.h"
#include "painter.h"
#include "Cartographer/cartographer.h"

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
    void createTrafficLightDirections();
    void createIntersectionCoordinates();
    void createIntersectionLanes();
    void createColorScheme();
    void deleteDesigner();

    // Painting
    void initPainter();
    void drawRoadNetwork();
    void drawRoads();
    void drawTrafficLights();
    void drawBoundary();
    void saveRoadImage();
    void deletePainter();

    // Cartography
    void createVertices();
    void createEdges();
    void createLuaFile();
    void printGraph();
    void deleteCartographer();

    // RoadNetwork
    void saveTrafficLightsCoordinates();
    void printRoadNetwork();

private:
    RoadNetwork* roadNetwork;

    XmlParser* xmlParser;
    Designer* designer;
    Painter* painter;

    Cartographer* cartographer;

    QFile* osmFile;

};

#endif // MAPGENERATOR_H
