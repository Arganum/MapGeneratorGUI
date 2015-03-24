#include "mapgenerator.h"

MapGenerator::MapGenerator()
{
    MapGenerator::roadNetwork = new RoadNetwork;

    MapGenerator::xmlParser = new XmlParser( MapGenerator::roadNetwork );
    MapGenerator::designer = new Designer( MapGenerator::roadNetwork );
    MapGenerator::painter = new Painter( MapGenerator::roadNetwork );

    MapGenerator::cartographer = new Cartographer( MapGenerator::roadNetwork );
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

void MapGenerator::createTrafficLightDirections()
{
    MapGenerator::designer->createTrafficLightDirections();
}

void MapGenerator::createIntersectionCoordinates()
{
    MapGenerator::designer->createIntersectionCoordinates();
}

void MapGenerator::createIntersectionLanes()
{
    MapGenerator::designer->createIntersectionLanes();
}

void MapGenerator::createColorScheme()
{
    MapGenerator::designer->createColorScheme();
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

void MapGenerator::drawBoundary()
{
    MapGenerator::painter->drawBoundary();
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

void MapGenerator::createVertices()
{
     MapGenerator::cartographer->createVertices();
}

void MapGenerator::createEdges()
{
    MapGenerator::cartographer->createEdges();
}

void MapGenerator::createLuaFile()
{
    MapGenerator::cartographer->createLuaFile();
}

void MapGenerator::printGraph()
{
    MapGenerator::cartographer->printGraph();
}

void MapGenerator::deleteCartographer()
{
    delete MapGenerator::cartographer;
}

void MapGenerator::saveTrafficLightsCoordinates()
{
    QFile* trafficLightsFile = new QFile( "trafficLights.lua" );
    if ( !trafficLightsFile->open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;
    QTextStream out( trafficLightsFile );

    std::vector<TrafficLight> trafficLights = MapGenerator::roadNetwork->getTrafficLights();

    int index = 0;
    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        index++;
        out << "Entry( { " << "\n";
        out << "index = " << index << ",\n";
        out << "x = " << floor( it->getPoint().x() + 0.5 ) << ",\n";
        out << "y = " << floor( ( MapGenerator::roadNetwork->getBoundary().height - it->getPoint().y() ) + 0.5 ) << ",\n";
        out << "red   = " << it->getColor().red() << ",\n";
        out << "green = " << it->getColor().green() << ",\n";
        out << "blue  = " << it->getColor().blue() << ",\n";

        std::vector<Lane> tempSwitch = it->getSwitch1();
        out << "switch1 = { ";
        for ( std::vector<Lane>::iterator jt = tempSwitch.begin();
              jt != tempSwitch.end(); ++jt )
        {
            if ( jt+1 == tempSwitch.end() )
                out << "{ red = " << jt->red << " , green = " << jt->green << " , blue = " << jt->blue << " }";
            else
                out << "{ red = " << jt->red << " , green = " << jt->green << " , blue = " << jt->blue << " }, ";
        }
        out << " } ,\n";

        tempSwitch = it->getSwitch2();
        out << "switch2 = { ";
        for ( std::vector<Lane>::iterator jt = tempSwitch.begin();
              jt != tempSwitch.end(); ++jt )
        {
            if ( jt+1 == tempSwitch.end() )
                out << "{ red = " << jt->red << " , green = " << jt->green << " , blue = " << jt->blue << " }";
            else
                out << "{ red = " << jt->red << " , green = " << jt->green << " , blue = " << jt->blue << " }, ";
        }
        out << " }\n";
        out << "} )" << "\n" << "\n";
    }

    trafficLightsFile->close();
    delete trafficLightsFile;
}

void MapGenerator::printRoadNetwork()
{
    //MapGenerator::roadNetwork->printBoundary();
    //MapGenerator::roadNetwork->printTrafficLights();
    //MapGenerator::roadNetwork->printRoadsNodes();
    //MapGenerator::roadNetwork->printRoadsLines();
    //MapGenerator::roadNetwork->printRoadsNodeIDs();
}



