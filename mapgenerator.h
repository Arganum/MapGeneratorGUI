#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <QFile>
#include <QFileDialog>
#include <iostream>
#include <QDebug>

#include "roadnetwork.h"
#include "xmlparser.h"
#include "painter.h"
#include "road.h"
#include "utility.h"

class RoadNetwork;
class XmlParser;
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

    // Painting
    void initPainter();
    void createRoadLines();
    void drawRoads();
    void drawBoundary();
    void saveRoadImage();
    void deletePainter();

private:
    RoadNetwork* roadNetwork;
    XmlParser* xmlParser;
    Painter* painter;

    QFile* osmFile;

};

#endif // MAPGENERATOR_H
