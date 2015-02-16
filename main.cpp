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

    mapGenerator->initPainter();
    mapGenerator->createRoadLines();
    mapGenerator->drawRoads();
    mapGenerator->drawBoundary();
    mapGenerator->saveRoadImage();
    mapGenerator->deletePainter();

    delete mapGenerator;

    return a.exec();
}
