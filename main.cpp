#include "mainwindow.h"
#include <QApplication>

#include <mapgenerator.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    MapGenerator* mapGenerator = new MapGenerator;
    mapGenerator->setOsmFile( w.getFilePath() );
    mapGenerator->parseOsmFile();
    mapGenerator->deleteOsmParser();

    mapGenerator->createBoundary();
    mapGenerator->createRoadLines();
    mapGenerator->createTrafficLightCoordinates();
    mapGenerator->createIntersectionCoordinates();
    mapGenerator->createColorScheme();
    mapGenerator->createTrafficLightDirections();
    mapGenerator->createIntersectionLanes();
    mapGenerator->deleteDesigner();

    mapGenerator->initPainter();
    mapGenerator->drawRoadNetwork();
    mapGenerator->saveRoadImage();
    mapGenerator->deletePainter();

    mapGenerator->createVertices();
    mapGenerator->createEdges();
    mapGenerator->createLuaFile();
    mapGenerator->printGraph();
    mapGenerator->deleteCartographer();

    mapGenerator->saveTrafficLightsCoordinates();
    mapGenerator->printRoadNetwork();


    delete mapGenerator;

    return a.exec();
}
