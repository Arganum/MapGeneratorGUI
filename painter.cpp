#include "painter.h"

Painter::Painter( RoadNetwork* roadNet )
{
    roadNetwork = roadNet;
}

Painter::~Painter()
{

}

void Painter::initRoadImage()
{
    double imageWidth, imageHeight;

    Bounds boundary = roadNetwork->getBoundary();

    imageWidth  = latLon2Length( boundary.minLatitude,
                                 boundary.maxLongitude,
                                 boundary.minLatitude,
                                 boundary.minLongitude);
    imageHeight = latLon2Length( boundary.maxLatitude,
                                 boundary.minLongitude,
                                 boundary.minLatitude,
                                 boundary.minLongitude);

    boundary.imageWidth  = imageWidth;
    boundary.imageHeight = imageHeight;

    roadNetwork->setBoundary( boundary );

    A.setX( 0 );
    A.setY( 0 );
    B.setX( imageWidth );
    B.setY( 0 );
    C.setX( imageWidth );
    C.setY( imageHeight );
    D.setX( 0 );
    D.setY( imageHeight);

    a.setPoints( A, B );
    b.setPoints( B, C );
    c.setPoints( C, D );
    d.setPoints( D, A );

    QImage tempRoadImage( imageWidth + 1, imageHeight + 1,
                          QImage::Format_RGB32 );
    tempRoadImage.fill( Qt::black );
    roadImage = tempRoadImage;
}

void Painter::initRoadPainter()
{
    roadPainter.begin( &roadImage );
    roadPainter.setRenderHint( QPainter::Antialiasing );
    roadPainter.setBrush( Qt::NoBrush );
    roadPainter.setPen( Qt::white );
}

void Painter::endRoadPainter()
{
    roadPainter.end();
}

void Painter::createRoadLines()
{
    QLineF tempLine;
    QPointF beginPoint, endPoint;

    std::vector<Road> roads = roadNetwork->getRoads();

    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        std::vector<Node> nodes = it->getNodes();
        for ( std::vector<Node>::iterator jt = nodes.begin();
              jt < (nodes.end() - 1); ++jt )
        {
            beginPoint = circle2CircleIntersect( jt->latitude, jt->longitude );
            endPoint   = circle2CircleIntersect( (jt+1)->latitude, (jt+1)->longitude );
            tempLine.setPoints( beginPoint, endPoint );
            it->addLine( tempLine );
        }
    }

    roadNetwork->setRoads( roads );
}

void Painter::createTrafficLightCoordinates()
{
    std::vector<TrafficLight> trafficLights = roadNetwork->getTrafficLights();
    std::vector<TrafficLight> newTrafficLights;
    QPointF trafficLightPoint;
    struct TrafficLight newTrafficLight;
    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        trafficLightPoint = circle2CircleIntersect( it->node.latitude, it->node.longitude );
        if ( trafficLightPoint.x() > A.x() && trafficLightPoint.y() > A.y() )
            if ( trafficLightPoint.x() < C.x() && trafficLightPoint.y() < C.y() )
            {
                newTrafficLight.node = it->node;
                newTrafficLight.x = trafficLightPoint.x();
                newTrafficLight.y = trafficLightPoint.y();
                newTrafficLights.push_back( newTrafficLight );
            }
    }
    roadNetwork->setTrafficLights( newTrafficLights );
}

double Painter::latLon2Length( double endLatitude, double endLongitude,
                               double beginLatitude, double beginLongitude )
{
    double theta, dist;
    theta = endLongitude - beginLongitude;
    dist = sin( deg2Rad( endLatitude ) ) *
           sin( deg2Rad( beginLatitude ) ) +
           cos( deg2Rad( endLatitude ) ) *
           cos( deg2Rad( beginLatitude ) ) *
           cos( deg2Rad( theta ) );
    dist = acos( dist );
    dist = rad2Deg( dist );
    dist = dist * 60 * 1.1515 * 1.609344; //M nothing, KM 1.609344
    dist = dist * MAP_SCALE;

    return dist;
}

double Painter::deg2Rad(double deg )
{
    return ( deg * M_PI / 180 );
}

double Painter::rad2Deg( double rad )
{
    return ( rad * 180 / M_PI );
}

QPointF Painter::circle2CircleIntersect( double latitude, double longitude )
{
    double dx, dy, dist, adist, r1, r2, r3, hdist, dist1, dist2;
    QPointF temp(0,0), point1(0,0), point2(0,0), result(0,0);

    dx   = a.p2().x() - a.p1().x();
    dy   = a.p2().y() - a.p1().y();
    dist = sqrt( (dy * dy) + (dx * dx) );

    r1 = latLon2Length( latitude, longitude,
                        roadNetwork->getBoundary().minLatitude,
                        roadNetwork->getBoundary().minLongitude );
    r2 = latLon2Length( latitude, longitude,
                        roadNetwork->getBoundary().minLatitude,
                        roadNetwork->getBoundary().maxLongitude );
    r3 = latLon2Length( latitude, longitude,
                        roadNetwork->getBoundary().maxLatitude,
                        roadNetwork->getBoundary().minLongitude );

    adist = ( (r1 * r1) - (r2 * r2) + (dist * dist) ) / (2.0 * dist);

    temp.setX( a.p1().x() + (dx * adist / dist) );
    temp.setY( a.p1().y() + (dy * adist / dist) );

    hdist = sqrt( (r1 * r1) - (adist * adist) );

    result.setX( (0 - dy) * (hdist / dist) );
    result.setY( dx * (hdist / dist) );

    point1.setX( temp.x() + result.x() );
    point1.setY( temp.y() + result.y() );

    point2.setX( temp.x() - result.x() );
    point2.setY( temp.y() - result.y() );

    dist1 = sqrt( ( (point1.x() - d.p1().x()) *
                    (point1.x() - d.p1().x()) ) +
                  ( (point1.y() - d.p1().y()) *
                    (point1.y() - d.p1().y()) ) );

    dist2 = sqrt( ( (point2.x() - d.p1().x()) *
                    (point2.x() - d.p1().x()) ) +
                  ( (point2.y() - d.p1().y()) *
                    (point2.y() - d.p1().y()) ) );

    double choseP1 = fabs(r3 - dist1);
    double choseP2 = fabs(r3 - dist2);
    if ( choseP1 < choseP2 )
    {
        result.setX( point1.x() );
        result.setY( point1.y() );
    }
    else
    {
        result.setX( point2.x() );
        result.setY( point2.y() );
    }

    //NOTE!!! If the output image is wrong change CIRCLE2CIRCLE_INTERSECT. DEBUG output below.
    //If result.x() is 0 and result.y() is a high value change CIRCLE2CIRCLE_INTERSECT.

    /*
    std::cout << "r3: " << r3 << " Dist1: " << dist1 << " r3 - dist1: " << r3 - dist1 << std::endl;
    std::cout << "r3: " << r3 << " Dist2: " << dist2 << " r3 - dist2: " << r3 - dist2 << std::endl;
    std::cout << "Point1: " << point1.x() << " , " << point1.y() << std::endl;
    std::cout << "Point2: " << point2.x() << " , " << point2.y() << std::endl;
    std::cout << "Result: " << result.x() << " , " << result.y() << std::endl;
    std::cout << std::endl;
    //*/

    return result;
}

void Painter::drawRoads()
{
    std::vector<Road> roads = roadNetwork->getRoads();

    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        std::vector<QLineF> lines = it->getLines();
        for ( std::vector<QLineF>::iterator jt = lines.begin();
              jt != lines.end(); ++jt )
        {
            roadPainter.drawLine( *jt );
        }
    }
}

void Painter::drawBoundary()
{
    roadPainter.drawLine( a );
    roadPainter.drawLine( b );
    roadPainter.drawLine( c );
    roadPainter.drawLine( d );
}

QImage Painter::getRoadImage()
{
    return Painter::roadImage;
}
