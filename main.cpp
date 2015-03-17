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
    mapGenerator->createTrafficLightDirections();
    mapGenerator->deleteDesigner();

    mapGenerator->initPainter();
    mapGenerator->drawRoadNetwork();
    mapGenerator->saveRoadImage();
    mapGenerator->deletePainter();

    mapGenerator->saveTrafficLightsCoordinates();
    mapGenerator->printRoadNetwork();


    delete mapGenerator;

    return a.exec();
}
