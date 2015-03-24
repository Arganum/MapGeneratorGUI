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

    QImage tempRoadImage( int(imageWidth + 0.5), int(imageHeight + 0.5),
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
    Painter::drawIntersections();
    Painter::drawBoundary();
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
        pen.setCapStyle( Qt::SquareCap );
        pen.setJoinStyle( Qt::BevelJoin );

        pen.setColor( it->getColor() );
        Painter::roadPainter.setPen( pen );

        Painter::roadPainter.drawEllipse( QPoint(int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5)), 1, 1 );
        Painter::roadPainter.drawEllipse( QPoint(int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5)), 2, 2 );
        Painter::roadPainter.drawEllipse( QPoint(int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5)), 3, 3 );

        pen.setWidth( 3 );
        Painter::roadPainter.setPen( pen );
        Painter::roadPainter.drawPoint( int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5) );

        pen.setWidth( 1 );
        pen.setColor( Qt::black );
        Painter::roadPainter.setPen( pen );

        Painter::roadPainter.drawPoint( int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5) );
    }
}

void Painter::drawIntersections()
{
    std::vector<Intersection> intersections = Painter::roadNetwork->getIntersections();

    for ( std::vector<Intersection>::iterator it = intersections.begin();
          it != intersections.end(); ++it )
    {
        QPen pen;

        pen.setStyle( Qt::SolidLine );
        pen.setCapStyle( Qt::SquareCap );
        pen.setJoinStyle( Qt::BevelJoin );

        pen.setColor( it->getColor() );
        Painter::roadPainter.setPen( pen );

        Painter::roadPainter.drawEllipse( QPoint(int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5)), 1, 1 );
        Painter::roadPainter.drawEllipse( QPoint(int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5)), 2, 2 );
        Painter::roadPainter.drawEllipse( QPoint(int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5)), 3, 3 );

        pen.setWidth( 3 );
        Painter::roadPainter.setPen( pen );
        Painter::roadPainter.drawPoint( int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5) );

        pen.setWidth( 1 );
        pen.setColor( Qt::black );
        Painter::roadPainter.setPen( pen );

        Painter::roadPainter.drawPoint( int(it->getPoint().x() + 0.5), int(it->getPoint().y() + 0.5) );
    }
}

void Painter::drawBoundary()
{
    QPen pen;

    pen.setStyle( Qt::SolidLine );
    pen.setWidth( 1 );
    pen.setColor( Qt::black );
    pen.setCapStyle( Qt::SquareCap );
    pen.setJoinStyle( Qt::BevelJoin );

    Painter::roadPainter.setPen( pen );

    Painter::roadPainter.drawLine( 0, 0, Painter::roadImage.width()-1, 0 );
    Painter::roadPainter.drawLine( Painter::roadImage.width()-1, 0, Painter::roadImage.width()-1, Painter::roadImage.height()-1 );
    Painter::roadPainter.drawLine( Painter::roadImage.width()-1, Painter::roadImage.height()-1, 0, Painter::roadImage.height()-1 );
    Painter::roadPainter.drawLine( 0, Painter::roadImage.height()-1, 0, 0 );
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
            pen.setWidth( 3 );
            pen.setColor( it->getColor() );
            pen.setCapStyle( Qt::SquareCap );
            pen.setJoinStyle( Qt::BevelJoin );

            Painter::roadPainter.setPen( pen );
            //CompositionMode

            std::vector<QLineF> lines = it->getLines();
            for ( std::vector<QLineF>::iterator jt = lines.begin();
                  jt != lines.end(); ++jt )
            {
                Painter::roadPainter.drawLine( int(jt->p1().x() + 0.5), int(jt->p1().y() + 0.5),
                                               int(jt->p2().x() + 0.5), int(jt->p2().y() + 0.5) );
                //Painter::roadPainter.drawLine( *jt );
            }
        }
    }
}
