#include "mapgenerator.h"

MapGenerator::MapGenerator()
{
    roadNetwork = new RoadNetwork;
    xmlParser = new XmlParser( roadNetwork );
    painter = new Painter( roadNetwork );
}

MapGenerator::~MapGenerator()
{
    delete roadNetwork;
}

void MapGenerator::setOsmFile( QString path )
{
    osmFile = new QFile( path ); // C:/Users/LAU/Downloads/
}

void MapGenerator::parseOsmFile()
{
    if ( xmlParser->hasParsed( FIRST_PARSE, osmFile ) )
    {
        //std::cout << "First parse successful" << std::endl;
    } else {
        std::cout << "Error at first parse" << std::endl;
    }

    if ( xmlParser->hasParsed( SECOND_PARSE, osmFile ) )
    {
        //std::cout << "Second parse successful" << std::endl;
    } else {
        std::cout << "Error at second parse" << std::endl;
    }
}

void MapGenerator::deleteOsmParser()
{
    delete xmlParser;
    delete osmFile;
}

void MapGenerator::initPainter()
{
    painter->initRoadImage();
    painter->initRoadPainter();
}

void MapGenerator::createRoadLines()
{
    painter->createRoadLines();
}

void MapGenerator::drawRoads()
{
    painter->drawRoads();
}

void MapGenerator::drawBoundary()
{
    painter->drawBoundary();
}

void MapGenerator::saveRoadImage()
{
    QFile* imageFile = new QFile( "roads.png" );

    QImage roadImage = painter->getRoadImage();
    roadImage = roadImage.mirrored();
    if ( !roadImage.save( imageFile ) )
        std::cout << "Cannot save map" << std::endl;

    delete imageFile;
}

void MapGenerator::deletePainter()
{
    painter->endRoadPainter();
    delete painter;
}

void MapGenerator::printRoadNetwork()
{
    roadNetwork->printBoundary();
    roadNetwork->printRoadsNodes();
    roadNetwork->printRoadsLines();
    roadNetwork->printRoadsNodeIDs();
}




