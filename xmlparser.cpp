#include "xmlparser.h"

XmlParser::XmlParser(RoadNetwork* roadNet)
{
    roadNetwork = roadNet;
}

XmlParser::~XmlParser()
{

}

bool XmlParser::hasParsed(bool parseNumber, QIODevice* device )
{
    xmlReader = new QXmlStreamReader;
    xmlReader->setDevice( device );
    device->open( QFile::ReadOnly | QFile::Text );

    bool isDone = false;

    if ( xmlReader->readNextStartElement() )
    {
        bool isRightVersion =
            xmlReader->name() == "osm" &&
            xmlReader->attributes().value( "version" ) == "0.6";
        if ( isRightVersion )
        {
            if ( parseNumber == FIRST_PARSE ) {
                firstParse();
                isDone = true;
            } else if ( parseNumber == SECOND_PARSE ) {
                secondParse();

                sortRoadsNodes();
                roadNetwork->setRoads( XmlParser::roads );
                roadNetwork->setTrafficLights( XmlParser::trafficLights );
                XmlParser::roads.clear();
                XmlParser::trafficLights.clear();

                isDone = true;
            }
        } else {
            xmlReader->raiseError("The file is not an .osm version 0.6 file");
            isDone = false;
        }
    }

    xmlReader->clear();
    device->close();
    delete xmlReader;

    return isDone;
}

void XmlParser::firstParse()
{ 
    while ( !xmlReader->atEnd() )
    {
        if ( xmlReader->readNextStartElement() )
        {
            if ( xmlReader->name() == "way" )
            {
                Road tempRoad;
                elementWay( tempRoad );
                //bool hasRoadType = !tempRoad.getType().empty();
                //bool hasRoadName = !tempRoad.getName().empty(); //ROAD TYPE HERE!!!
                //if ( hasRoadType && hasRoadName )
                //    XmlParser::roads.push_back( tempRoad );

                if ( tempRoad.getType() == "motorway" && MOTORWAY )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "trunk" && TRUNK )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "primary" && PRIMARY )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "secondary" && SECONDARY )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "tertiary" && TERTIARY )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "unclassified" && UNCLASSIFIED )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "residential" && RESIDENTIAL )
                    XmlParser::roads.push_back( tempRoad );
                else if ( tempRoad.getType() == "service" && SERVICE )
                    XmlParser::roads.push_back( tempRoad );
            }
            else
                xmlReader->skipCurrentElement();
        }
    }
}

void XmlParser::elementWay( Road & road )
{
    while ( xmlReader->readNextStartElement() )
    {
        if ( xmlReader->name() == "nd" )
            elementNd( road );
        else if ( xmlReader->name() == "tag" )
            elementTag( road );
        else
            xmlReader->skipCurrentElement();
    }
}

void XmlParser::elementNd( Road & road )
{
    road.addNodeID( xmlReader->attributes().value( "ref" ).toULong() );

    while ( xmlReader->readNextStartElement() )
    {
        if ( xmlReader->name() == "nd" )
            elementNd( road );
        else if ( xmlReader->name() == "tag" )
            elementTag( road );
        else
            xmlReader->skipCurrentElement();
    }
}

void XmlParser::elementTag( Road & road )
{
    if ( xmlReader->attributes().value( "k" ) == "highway" )
        road.setType( xmlReader->attributes().value( "v" ).toUtf8().constData() );

    if ( xmlReader->attributes().value( "k" ) == "name" )
        road.setName( xmlReader->attributes().value( "v" ).toUtf8().constData() );

    while ( xmlReader->readNextStartElement() )
    {
        if ( xmlReader->name() == "nd" )
            elementNd( road );
        else if ( xmlReader->name() == "tag" )
            elementTag( road );
        else
            xmlReader->skipCurrentElement();
    }
}

void XmlParser::secondParse()
{
    while ( !xmlReader->atEnd() )
    {
        if ( xmlReader->readNextStartElement() )
        {
            if ( xmlReader->name() == "bounds" )
                elementBounds();
            else if ( xmlReader->name() == "node" )
                elementNode();
            else
                xmlReader->skipCurrentElement();
        }
    }
}

void XmlParser::elementBounds()
{
    Bounds bounds;
    bounds.minLatitude = xmlReader->attributes().value( "minlat" ).toDouble();
    bounds.minLongitude = xmlReader->attributes().value( "minlon" ).toDouble();
    bounds.maxLatitude = xmlReader->attributes().value( "maxlat" ).toDouble();
    bounds.maxLongitude = xmlReader->attributes().value( "maxlon" ).toDouble();

    roadNetwork->setBoundary( bounds );
}

void XmlParser::elementNode()
{
    struct Node tempNode;

    tempNode.number = 0;
    tempNode.ID = xmlReader->attributes().value( "id" ).toULong();
    tempNode.latitude = xmlReader->attributes().value( "lat" ).toDouble();
    tempNode.longitude = xmlReader->attributes().value( "lon" ).toDouble();

    for ( std::vector<Road>::iterator it = XmlParser::roads.begin();
          it != XmlParser::roads.end(); ++it )
    {
        std::vector<unsigned long> nodeIDs = it->getNodeIDs();
        int index = 0;
        for ( std::vector<unsigned long>::iterator jt = nodeIDs.begin();
              jt != nodeIDs.end(); ++jt )
        {
            ++index;
            if ( xmlReader->attributes().value( "id" ).toULong() == *jt )
            {
                tempNode.number = index;
                it->addNode( tempNode );
            }
        }
    }

    while ( xmlReader->readNextStartElement() )
    {
        if ( xmlReader->name() == "node" )
            elementNode();
        else if ( xmlReader->name() == "tag" )
            elementTagTrafficLights( tempNode );
        else
            xmlReader->skipCurrentElement();
    }

}

void XmlParser::elementTagTrafficLights( Node & node )
{
    if ( xmlReader->attributes().value( "k" ) == "highway" )
        if ( xmlReader->attributes().value( "v" ) == "traffic_signals" )
            if ( node.number != 0 )
            {
                struct TrafficLight tempTrafficLight;
                tempTrafficLight.node = node;
                XmlParser::trafficLights.push_back( tempTrafficLight );
            }

    while ( xmlReader->readNextStartElement() )
    {
        if ( xmlReader->name() == "node" )
            elementNode();
        else if ( xmlReader->name() == "tag" )
            elementTagTrafficLights( node );
        else
            xmlReader->skipCurrentElement();
    }
}

void XmlParser::sortRoadsNodes()
{
    for ( std::vector<Road>::iterator it = XmlParser::roads.begin();
          it != XmlParser::roads.end(); ++it )
    {
        std::vector<Node> nodes = it->getNodes();
        std::sort( nodes.begin(), nodes.end() );
        it->setNodes( nodes );
    }
}

