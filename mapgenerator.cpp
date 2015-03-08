#include "mapgenerator.h"

MapGenerator::MapGenerator()
{
    MapGenerator::roadNetwork = new RoadNetwork;

    MapGenerator::xmlParser = new XmlParser( MapGenerator::roadNetwork );
    MapGenerator::designer = new Designer( MapGenerator::roadNetwork );
    MapGenerator::painter = new Painter( MapGenerator::roadNetwork );
}

MapGenerator::~MapGenerator()
{
    delete MapGenerator::roadNetwork;
}

void MapGenerator::setOsmFile( QString path )
{
    MapGenerator::osmFile = new QFile( path ); // C:/Users/LAU/Downloads/
}

void MapGenerator::parseOsmFile()
{
    if ( MapGenerator::xmlParser->hasParsed( FIRST_PARSE, MapGenerator::osmFile ) )
    {
        //std::cout << "First parse successful" << std::endl;
    } else {
        std::cout << "Error at first parse" << std::endl;
    }

    if ( MapGenerator::xmlParser->hasParsed( SECOND_PARSE, MapGenerator::osmFile ) )
    {
        //std::cout << "Second parse successful" << std::endl;
    } else {
        std::cout << "Error at second parse" << std::endl;
    }
}

void MapGenerator::deleteOsmParser()
{
    delete MapGenerator::xmlParser;
    delete MapGenerator::osmFile;
}

void MapGenerator::createBoundary()
{
    MapGenerator::designer->createBoundary();
}

void MapGenerator::createRoadLines()
{
    MapGenerator::designer->createRoadLines();
}

void MapGenerator::createTrafficLightCoordinates()
{
    MapGenerator::designer->createTrafficLightCoordinates();
}

void MapGenerator::deleteDesigner()
{
    delete MapGenerator::designer;
}

void MapGenerator::initPainter()
{
    MapGenerator::painter->initRoadImage();
    MapGenerator::painter->initRoadPainter();
}

void MapGenerator::drawRoadNetwork()
{
    MapGenerator::painter->drawRoadNetwork();
}

void MapGenerator::drawRoads()
{
    MapGenerator::painter->drawRoads();
}

void MapGenerator::drawTrafficLights()
{
    MapGenerator::painter->drawTrafficLights();
}

void MapGenerator::saveRoadImage()
{
    QFile* imageFile = new QFile( "roads.png" );

    QImage roadImage = MapGenerator::painter->getRoadImage();
    roadImage = roadImage.mirrored();
    if ( !roadImage.save( imageFile ) )
        std::cout << "Cannot save map" << std::endl;

    delete imageFile;
}

void MapGenerator::deletePainter()
{
    MapGenerator::painter->endRoadPainter();
    delete MapGenerator::painter;
}

void MapGenerator::saveTrafficLightsCoordinates()
{
    QFile* trafficLightsFile = new QFile( "trafficLights.lua" );
    if ( !trafficLightsFile->open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;
    QTextStream out( trafficLightsFile );

    std::vector<TrafficLight> trafficLights = MapGenerator::roadNetwork->getTrafficLights();

    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        out << "Entry { " << "\n";
        out << "x = " << floor( it->getPoint().x() + 0.5 ) << ",\n";
        out << "y = " << floor( ( MapGenerator::roadNetwork->getBoundary().height - it->getPoint().y() ) + 0.5 ) << ",\n";
        out << "Red   = " << it->getColor().red() << ",\n";
        out << "Green = " << it->getColor().green() << ",\n";
        out << "Blue  = " << it->getColor().blue() << "\n";
        out << "}" << "\n" << "\n";
    }

    trafficLightsFile->close();
    delete trafficLightsFile;
}

void MapGenerator::printRoadNetwork()
{
    //MapGenerator::roadNetwork->printBoundary();
    MapGenerator::roadNetwork->printTrafficLights();
    MapGenerator::roadNetwork->printRoadsNodes();
    //MapGenerator::roadNetwork->printRoadsLines();
    //MapGenerator::roadNetwork->printRoadsNodeIDs();
}



