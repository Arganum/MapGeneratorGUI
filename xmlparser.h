#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

#include "roadnetwork.h"
#include "utility.h"

class RoadNetwork;
class XmlParser
{
public:
    XmlParser(RoadNetwork* roadNet);
    ~XmlParser();

    bool hasParsed( bool parse, QIODevice* device );

private:
    RoadNetwork* roadNetwork;

    QXmlStreamReader* xmlReader;
    std::vector<Road> roads;
    std::vector<TrafficLight> trafficLights;

    void firstParse();
    void elementWay( Road & road );
    void elementNd( Road & road );
    void elementTag( Road & road );

    void secondParse();
    void elementBounds();
    void elementNode();
    void elementTagTrafficLights( Node & node );

    void sortRoadsNodes();
};

#endif // XMLPARSER_H
