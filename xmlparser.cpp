#include "xmlparser.h"

XmlParser::XmlParser(RoadNetwork* roadNetwork)
{
    XmlParser::roadNetwork = roadNetwork;
}

XmlParser::~XmlParser()
{

}

bool XmlParser::hasParsed(bool parseNumber, QIODevice* device )
{
    XmlParser::xmlReader = new QXmlStreamReader;
    XmlParser::xmlReader->setDevice( device );
    device->open( QFile::ReadOnly | QFile::Text );

    bool isDone = false;

    if ( XmlParser::xmlReader->readNextStartElement() )
    {
        bool isRightVersion =
            XmlParser::xmlReader->name() == "osm" &&
            XmlParser::xmlReader->attributes().value( "version" ) == "0.6";
        if ( isRightVersion )
        {
            if ( parseNumber == FIRST_PARSE ) {
                XmlParser::firstParse();
                isDone = true;
            } else if ( parseNumber == SECOND_PARSE ) {
                XmlParser::secondParse();

                XmlParser::sortRoadsNodes();
                XmlParser::roadNetwork->setRoads( XmlParser::roads );
                XmlParser::roadNetwork->setTrafficLights( XmlParser::trafficLights );
                XmlParser::roads.clear();
                XmlParser::trafficLights.clear();

                isDone = true;
            }
        } else {
            XmlParser::xmlReader->raiseError("The file is not an .osm version 0.6 file");
            isDone = false;
        }
    }

    XmlParser::xmlReader->clear();
    device->close();
    delete XmlParser::xmlReader;

    return isDone;
}

void XmlParser::firstParse()
{ 
    while ( !XmlParser::xmlReader->atEnd() )
    {
        if ( XmlParser::xmlReader->readNextStartElement() )
        {
            if ( XmlParser::xmlReader->name() == "way" )
            {
                Road tempRoad;
                XmlParser::elementWay( tempRoad );
                //bool hasRoadType = !tempRoad.getType().empty();
                //bool hasRoadName = !tempRoad.getName().empty(); //ROAD TYPE HERE!!!
                //if ( hasRoadType && hasRoadName )
                //    XmlParser::roads.push_back( tempRoad );

                if ( tempRoad.getType() == "motorway" && MOTORWAY ) {
                    tempRoad.setSpeedLimit( MOTORWAY_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "trunk" && TRUNK ) {
                    tempRoad.setSpeedLimit( TRUNK_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "primary" && PRIMARY ) {
                    tempRoad.setSpeedLimit( PRIMARY_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "secondary" && SECONDARY ) {
                    tempRoad.setSpeedLimit( SECONDARY_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "tertiary" && TERTIARY ) {
                    tempRoad.setSpeedLimit( TERTIARY_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "unclassified" && UNCLASSIFIED ) {
                    tempRoad.setSpeedLimit( UNCLASSIFIED_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "residential" && RESIDENTIAL ) {
                    tempRoad.setSpeedLimit( RESIDENTIAL_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                } else if ( tempRoad.getType() == "service" && SERVICE ) {
                    tempRoad.setSpeedLimit( SERVICE_SPEED );
                    XmlParser::roads.push_back( tempRoad );
                }
            }
            else
                XmlParser::xmlReader->skipCurrentElement();
        }
    }
}

void XmlParser::elementWay( Road & road )
{
    while ( XmlParser::xmlReader->readNextStartElement() )
    {
        if ( XmlParser::xmlReader->name() == "nd" )
            XmlParser::elementNd( road );
        else if ( XmlParser::xmlReader->name() == "tag" )
            XmlParser::elementTag( road );
        else
            XmlParser::xmlReader->skipCurrentElement();
    }
}

void XmlParser::elementNd( Road & road )
{
    road.addNodeID( XmlParser::xmlReader->attributes().value( "ref" ).toULong() );

    while ( XmlParser::xmlReader->readNextStartElement() )
    {
        if ( XmlParser::xmlReader->name() == "nd" )
            XmlParser::elementNd( road );
        else if ( XmlParser::xmlReader->name() == "tag" )
            XmlParser::elementTag( road );
        else
            XmlParser::xmlReader->skipCurrentElement();
    }
}

void XmlParser::elementTag( Road & road )
{
    if ( XmlParser::xmlReader->attributes().value( "k" ) == "highway" )
        road.setType( XmlParser::xmlReader->attributes().value( "v" ).toUtf8().constData() );

    if ( XmlParser::xmlReader->attributes().value( "k" ) == "name" )
        road.setName( XmlParser::xmlReader->attributes().value( "v" ).toUtf8().constData() );

    if ( XmlParser::xmlReader->attributes().value( "k" ) == "junction" &&
         XmlParser::xmlReader->attributes().value( "v" ) == "roundabout" )
        road.setIsOneWay( "yes" );

    if ( XmlParser::xmlReader->attributes().value( "k" ) == "oneway" &&
         XmlParser::xmlReader->attributes().value( "v" ) == "yes" )
        road.setIsOneWay( XmlParser::xmlReader->attributes().value( "v" ).toUtf8().constData() );

    if ( XmlParser::xmlReader->attributes().value( "k" ) == "oneway" &&
         XmlParser::xmlReader->attributes().value( "v" ) == "-1" )
        road.setIsOneWay( XmlParser::xmlReader->attributes().value( "v" ).toUtf8().constData() );

    while ( XmlParser::xmlReader->readNextStartElement() )
    {
        if ( XmlParser::xmlReader->name() == "nd" )
            XmlParser::elementNd( road );
        else if ( XmlParser::xmlReader->name() == "tag" )
            XmlParser::elementTag( road );
        else
            XmlParser::xmlReader->skipCurrentElement();
    }
}

void XmlParser::secondParse()
{
    while ( !XmlParser::xmlReader->atEnd() )
    {
        if ( XmlParser::xmlReader->readNextStartElement() )
        {
            if ( XmlParser::xmlReader->name() == "bounds" )
                XmlParser::elementBounds();
            else if ( XmlParser::xmlReader->name() == "node" )
                XmlParser::elementNode();
            else
                XmlParser::xmlReader->skipCurrentElement();
        }
    }
}

void XmlParser::elementBounds()
{
    Bounds bounds;
    bounds.minLatitude = XmlParser::xmlReader->attributes().value( "minlat" ).toDouble();
    bounds.minLongitude = XmlParser::xmlReader->attributes().value( "minlon" ).toDouble();
    bounds.maxLatitude = XmlParser::xmlReader->attributes().value( "maxlat" ).toDouble();
    bounds.maxLongitude = XmlParser::xmlReader->attributes().value( "maxlon" ).toDouble();

    XmlParser::roadNetwork->setBoundary( bounds );
}

void XmlParser::elementNode()
{
    struct Node tempNode;

    tempNode.number = 0;
    tempNode.ID = XmlParser::xmlReader->attributes().value( "id" ).toULong();
    tempNode.latitude = XmlParser::xmlReader->attributes().value( "lat" ).toDouble();
    tempNode.longitude = XmlParser::xmlReader->attributes().value( "lon" ).toDouble();

    for ( std::vector<Road>::iterator it = XmlParser::roads.begin();
          it != XmlParser::roads.end(); ++it )
    {
        std::vector<unsigned long> nodeIDs = it->getNodeIDs();
        int index = 0;
        for ( std::vector<unsigned long>::iterator jt = nodeIDs.begin();
              jt != nodeIDs.end(); ++jt )
        {
            ++index;
            if ( XmlParser::xmlReader->attributes().value( "id" ).toULong() == *jt )
            {
                tempNode.number = index;
                it->addNode( tempNode );
            }
        }
    }

    while ( XmlParser::xmlReader->readNextStartElement() )
    {
        if ( XmlParser::xmlReader->name() == "node" )
            XmlParser::elementNode();
        else if ( XmlParser::xmlReader->name() == "tag" )
            XmlParser::elementTagTrafficLights( tempNode );
        else
            XmlParser::xmlReader->skipCurrentElement();
    }

}

void XmlParser::elementTagTrafficLights( Node & node )
{
    if ( XmlParser::xmlReader->attributes().value( "k" ) == "highway" )
        if ( XmlParser::xmlReader->attributes().value( "v" ) == "traffic_signals" )
            if ( node.number != 0 )
            {
                TrafficLight tempTrafficLight;
                tempTrafficLight.setNode( node );
                XmlParser::trafficLights.push_back( tempTrafficLight );
            }

    while ( XmlParser::xmlReader->readNextStartElement() )
    {
        if ( XmlParser::xmlReader->name() == "node" )
            XmlParser::elementNode();
        else if ( XmlParser::xmlReader->name() == "tag" )
            XmlParser::elementTagTrafficLights( node );
        else
            XmlParser::xmlReader->skipCurrentElement();
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

