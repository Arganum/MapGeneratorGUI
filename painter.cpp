#include "painter.h"

Painter::Painter( RoadNetwork* roadNetwork )
{
    Painter::roadNetwork = roadNetwork;
}

Painter::~Painter()
{

}

void Painter::initRoadImage()
{
    double imageWidth, imageHeight;

    Bounds boundary = Painter::roadNetwork->getBoundary();

    imageWidth = boundary.width + 1;
    imageHeight = boundary.height + 1;

    QImage tempRoadImage( imageWidth, imageHeight,
                          QImage::Format_RGB32 );
    tempRoadImage.fill( Qt::black );
    Painter::roadImage = tempRoadImage;
}

void Painter::initRoadPainter()
{
    Painter::roadPainter.begin( &roadImage );
    //roadPainter.setRenderHint( QPainter::Antialiasing );
}

void Painter::endRoadPainter()
{
    Painter::roadPainter.end();
}

void Painter::drawRoadNetwork()
{
    Painter::drawRoads();
    Painter::drawTrafficLights();
}

void Painter::drawRoads()
{
    if ( SERVICE )
        Painter::drawRoadsType( "service" );
    if ( RESIDENTIAL )
        Painter::drawRoadsType( "residential" );
    if ( UNCLASSIFIED )
        Painter::drawRoadsType( "unclassified" );
    if ( TERTIARY )
        Painter::drawRoadsType( "tertiary" );
    if ( SECONDARY )
        Painter::drawRoadsType( "secondary" );
    if ( PRIMARY )
        Painter::drawRoadsType( "primary" );
    if ( TRUNK )
        Painter::drawRoadsType( "trunk" );
    if ( MOTORWAY )
        Painter::drawRoadsType( "motorway" );
}

void Painter::drawTrafficLights()
{
    std::vector<TrafficLight> trafficLights = Painter::roadNetwork->getTrafficLights();

    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        QPen pen;

        pen.setStyle( Qt::SolidLine );
        pen.setWidth( 3 );
        pen.setColor( it->getColor() );
        pen.setCapStyle( Qt::SquareCap );
        pen.setJoinStyle( Qt::BevelJoin );

        Painter::roadPainter.setPen( pen );

        Painter::roadPainter.drawPoint( floor ( it->getPoint().x() + 0.5 ), floor ( it->getPoint().y() + 0.5 ) );
    }
}

QImage Painter::getRoadImage()
{
    return Painter::roadImage;
}

void Painter::drawRoadsType( std::string type )
{
    std::vector<Road> roads = Painter::roadNetwork->getRoads();

    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        if ( it->getType() == type )
        {
            QPen pen;

            pen.setStyle( Qt::SolidLine );
            pen.setWidth( 1 );
            pen.setColor( it->getColor() );
            pen.setCapStyle( Qt::SquareCap );
            pen.setJoinStyle( Qt::BevelJoin );

            Painter::roadPainter.setPen( pen );
            //CompositionMode

            std::vector<QLineF> lines = it->getLines();
            for ( std::vector<QLineF>::iterator jt = lines.begin();
                  jt != lines.end(); ++jt )
            {
                Painter::roadPainter.drawLine( *jt );
            }
        }
    }
}
