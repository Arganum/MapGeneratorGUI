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

    std::cout << "Image Width : " << imageWidth  << std::endl;
    std::cout << "Image Height: " << imageHeight << std::endl;

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
              jt != (nodes.end() - 1); ++jt )
        {
            beginPoint = circle2CircleIntersect( jt->latitude, jt->longitude );
            endPoint   = circle2CircleIntersect( (jt+1)->latitude, (jt+1)->longitude );
            tempLine.setPoints( beginPoint, endPoint );
            tempLine = boundaryAdjustedLine( tempLine );
            it->addLine( tempLine );
        }
    }

    roadNetwork->setRoads( roads );
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

    if ( ((r3 - dist1) <= 0.005) && ((r3 - dist1) >= -0.005) )
    {
        result.setX( point1.x() );
        result.setY( point1.y() );
    }
    else if ( ((r3 - dist2) <= 0.005) && ((r3 - dist2) >= -0.005) )
    {
        result.setX( point2.x() );
        result.setY( point2.y() );
    }

    return result;
}

QLineF Painter::boundaryAdjustedLine( QLineF line )
{
    QLineF tempLine = line;

    QPointF aIntersectionPoint;
    QPointF bIntersectionPoint;
    QPointF cIntersectionPoint;
    QPointF dIntersectionPoint;

    int aIntersect = line.intersect( a, &aIntersectionPoint );
    int bIntersect = line.intersect( b, &bIntersectionPoint );
    int cIntersect = line.intersect( c, &cIntersectionPoint );
    int dIntersect = line.intersect( d, &dIntersectionPoint );

    if ( aIntersect == QLineF::BoundedIntersection )
    {
        if ( pointOutOfBoundary( line.p1() ) )
            tempLine.p1() = aIntersectionPoint;
        if ( pointOutOfBoundary( line.p2() ) )
            tempLine.p2() = aIntersectionPoint;
    }

    if ( bIntersect == QLineF::BoundedIntersection )
    {
        if ( pointOutOfBoundary( line.p1() ) )
            tempLine.p1() = bIntersectionPoint;
        if ( pointOutOfBoundary( line.p2() ) )
            tempLine.p2() = bIntersectionPoint;
    }

    if ( cIntersect == QLineF::BoundedIntersection )
    {
        if ( pointOutOfBoundary( line.p1() ) )
            tempLine.p1() = cIntersectionPoint;
        if ( pointOutOfBoundary( line.p2() ) )
            tempLine.p2() = cIntersectionPoint;
    }

    if ( dIntersect == QLineF::BoundedIntersection )
    {
        if ( pointOutOfBoundary( line.p1() ) )
            tempLine.p1() = dIntersectionPoint;
        if ( pointOutOfBoundary( line.p2() ) )
            tempLine.p2() = dIntersectionPoint;
    }

    return tempLine;
}

bool Painter::pointOutOfBoundary( QPointF point )
{
    bool temp;
    if ( point.x() < a.p1().x() && point.y() < a.p1().y() )
        temp = true;
    else if ( point.x() > c.p1().x() && point.y() > c.p1().y() )
        temp = true;
    else
        temp = false;

    return temp;
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
