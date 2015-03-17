#include "designer.h"

Designer::Designer( RoadNetwork* roadNetwork )
{
    Designer::roadNetwork = roadNetwork;
}

Designer::~Designer()
{

}

void Designer::createBoundary()
{
    //Designer::circle2CircleIntersect1();

    double width, height;

    Bounds boundary = Designer::roadNetwork->getBoundary();

    width  = Designer::latLon2Length( boundary.minLatitude,
                                      boundary.maxLongitude,
                                      boundary.minLatitude,
                                      boundary.minLongitude );
    height = Designer::latLon2Length( boundary.maxLatitude,
                                      boundary.minLongitude,
                                      boundary.minLatitude,
                                      boundary.minLongitude );

    //std::cout << width  << " , " << floor ( width  + 0.5 ) << std::endl;
    //std::cout << height << " , " << floor ( height + 0.5 ) << std::endl;
    //width  = floor ( width  + 0.5 );
    //height = floor ( height + 0.5 );

    boundary.width  = width;
    boundary.height = height;

    Designer::roadNetwork->setBoundary( boundary );

    Designer::A.setX( 0 );
    Designer::A.setY( 0 );
    Designer::B.setX( width );
    Designer::B.setY( 0 );
    Designer::C.setX( width );
    Designer::C.setY( height );
    Designer::D.setX( 0 );
    Designer::D.setY( height);

    Designer::a.setPoints( Designer::A, Designer::B );
    Designer::b.setPoints( Designer::B, Designer::C );
    Designer::c.setPoints( Designer::C, Designer::D );
    Designer::d.setPoints( Designer::D, Designer::A );
}

void Designer::createRoadLines()
{
    QLineF tempLine;
    QPointF beginPoint, endPoint;

    std::vector<Road> roads = Designer::roadNetwork->getRoads();
    double totalNumber = roads.size() + 1;
    int index = 0;

    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        ++index;
        QColor color;
        color = Designer::createColorScheme( totalNumber, index );
        it->setColor ( color );

        std::vector<Node> nodes = it->getNodes();
        for ( std::vector<Node>::iterator jt = nodes.begin();
              jt < (nodes.end() - 1); ++jt )
        {
            beginPoint = Designer::circle2CircleIntersect( jt->latitude, jt->longitude );
            endPoint   = Designer::circle2CircleIntersect( (jt+1)->latitude, (jt+1)->longitude );
            tempLine.setPoints( beginPoint, endPoint );
            it->addLine( tempLine );
        }
    }

    Designer::roadNetwork->setRoads( roads );
}

void Designer::createTrafficLightCoordinates()
{
    QColor color;
    color = Designer::createColorScheme( Designer::roadNetwork->getRoads().size() + 1, Designer::roadNetwork->getRoads().size() + 1 );

    std::vector<TrafficLight> trafficLights = Designer::roadNetwork->getTrafficLights();
    std::vector<TrafficLight> newTrafficLights;
    QPointF trafficLightPoint;
    TrafficLight newTrafficLight;
    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        trafficLightPoint = Designer::circle2CircleIntersect( it->getNode().latitude, it->getNode().longitude );
        if ( trafficLightPoint.x() > Designer::A.x() && trafficLightPoint.y() > Designer::A.y() )
            if ( trafficLightPoint.x() < Designer::C.x() && trafficLightPoint.y() < Designer::C.y() )
            {
                newTrafficLight.setNode( it->getNode() );
                newTrafficLight.setPoint( trafficLightPoint );
                newTrafficLight.setColor( color );
                newTrafficLights.push_back( newTrafficLight );
            }
    }
    Designer::roadNetwork->setTrafficLights( newTrafficLights );
}

void Designer::createTrafficLightDirections()
{
    std::vector<Road> roads =  Designer::roadNetwork->getRoads();
    std::vector<TrafficLight> trafficLights = Designer::roadNetwork->getTrafficLights();
    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        std::vector<Lane> lanes;

        for ( std::vector<Road>::iterator jt = roads.begin();
              jt != roads.end(); ++jt )
        {
            std::vector<QLineF> lines = jt->getLines();
            for ( std::vector<QLineF>::iterator kt = lines.begin();
                  kt != lines.end(); ++kt )
            {
                if ( ( floor( kt->p1().x() + 0.5 ) == floor( it->getPoint().x() + 0.5 ) &&
                       floor( kt->p1().y() + 0.5 ) == floor( it->getPoint().y() + 0.5 ) ) ||
                     ( floor( kt->p2().x() + 0.5 ) == floor( it->getPoint().x() + 0.5 ) &&
                       floor( kt->p2().y() + 0.5 ) == floor( it->getPoint().y() + 0.5 ) )
                   )
                {

                    if ( ( kt->p1().x() - it->getPoint().x() ) == 0  && ( kt->p1().y() - it->getPoint().y() ) == 0 )
                    {
                        Lane lane;
                        lane.x = kt->p2().x() - it->getPoint().x();
                        lane.y = kt->p2().y() - it->getPoint().y();

                        lane.red = jt->getColor().red();
                        lane.green = jt->getColor().green();
                        lane.blue = jt->getColor().blue();

                        lanes.push_back( lane );
                    } else if ( ( kt->p2().x() - it->getPoint().x() ) == 0  && ( kt->p2().y() - it->getPoint().y() ) == 0 ) {
                        Lane lane;
                        lane.x = kt->p1().x() - it->getPoint().x();
                        lane.y = kt->p1().y() - it->getPoint().y();

                        lane.red = jt->getColor().red();
                        lane.green = jt->getColor().green();
                        lane.blue = jt->getColor().blue();

                        lanes.push_back( lane );
                    }

                    std::cout << std::endl;
                }
            }
        }

        double highest = 0;
        std::vector<Lane> switch1;
        std::vector<Lane> switch2;
        for ( std::vector<Lane>::iterator lt = lanes.begin();
              lt != lanes.end(); ++lt )
        {
            double temp;
            std::vector<Lane> tempSwitch1;
            Lane lane;
            tempSwitch1.push_back( lane );
            tempSwitch1.push_back( lane );

            if ( lt+1 == lanes.end() )
            {
                //std::cout << QPointF::dotProduct( *lt, *points.begin() ) << std::endl << std::endl;
                tempSwitch1[0] = *lt;
                tempSwitch1[1] = *lanes.begin();

                temp = QPointF::dotProduct( QPointF( lt->x, lt->y ), QPointF( lanes.begin()->x, lanes.begin()->y ) );
            }
            else
            {
                //std::cout << QPointF::dotProduct( *lt, *(lt+1) ) << std::endl;
                tempSwitch1[0] = *lt;
                tempSwitch1[1] = *(lt+1);

                temp = QPointF::dotProduct( QPointF( lt->x, lt->y ), QPointF( (lt+1)->x, (lt+1)->y ) );
            }

            if ( highest > temp )
            {
                highest = temp;
                switch1 = tempSwitch1;
            }
        }

        for ( std::vector<Lane>::iterator lt = lanes.begin();
              lt != lanes.end(); ++lt )
        {
            if ( switch1[0].x != lt->x && switch1[0].y != lt->y &&
                 switch1[1].x != lt->x && switch1[1].y != lt->y )
            {
                switch2.push_back( *lt );
            }
        }

        it->setSwitch1( switch1 );
        it->setSwitch2( switch2 );
    }
    Designer::roadNetwork->setTrafficLights( trafficLights );
}

double Designer::latLon2Length( double endLatitude, double endLongitude,
                               double beginLatitude, double beginLongitude )
{
    double theta, dist;
    theta = endLongitude - beginLongitude;
    dist = sin( Designer::deg2Rad( endLatitude ) ) *
           sin( Designer::deg2Rad( beginLatitude ) ) +
           cos( Designer::deg2Rad( endLatitude ) ) *
           cos( Designer::deg2Rad( beginLatitude ) ) *
           cos( Designer::deg2Rad( theta ) );
    dist = acos( dist );
    dist = Designer::rad2Deg( dist );
    dist = dist * 60 * 1.1515 * 1.609344; //M nothing, KM 1.609344
    dist = dist * MAP_SCALE;

    return dist;
}

double Designer::deg2Rad(double deg )
{
    return ( deg * M_PI / 180 );
}

double Designer::rad2Deg( double rad )
{
    return ( rad * 180 / M_PI );
}

QPointF Designer::circle2CircleIntersect( double latitude, double longitude )
{
    double dx, dy, dist, adist, r1, r2, r3, hdist, dist1, dist2;
    QPointF temp(0,0), point1(0,0), point2(0,0), result(0,0);

    dx   = Designer::a.p2().x() - Designer::a.p1().x();
    dy   = Designer::a.p2().y() - Designer::a.p1().y();
    dist = sqrt( (dy * dy) + (dx * dx) );

    r1 = latLon2Length( latitude, longitude,
                        Designer::roadNetwork->getBoundary().minLatitude,
                        Designer::roadNetwork->getBoundary().minLongitude );
    r2 = latLon2Length( latitude, longitude,
                        Designer::roadNetwork->getBoundary().minLatitude,
                        Designer::roadNetwork->getBoundary().maxLongitude );
    r3 = latLon2Length( latitude, longitude,
                        Designer::roadNetwork->getBoundary().maxLatitude,
                        Designer::roadNetwork->getBoundary().minLongitude );

    adist = ( (r1 * r1) - (r2 * r2) + (dist * dist) ) / (2.0 * dist);

    temp.setX( Designer::a.p1().x() + (dx * adist / dist) );
    temp.setY( Designer::a.p1().y() + (dy * adist / dist) );

    hdist = sqrt( (r1 * r1) - (adist * adist) );

    result.setX( (0 - dy) * (hdist / dist) );
    result.setY( dx * (hdist / dist) );

    point1.setX( temp.x() + result.x() );
    point1.setY( temp.y() + result.y() );

    point2.setX( temp.x() - result.x() );
    point2.setY( temp.y() - result.y() );

    dist1 = sqrt( ( (point1.x() - Designer::d.p1().x()) *
                    (point1.x() - Designer::d.p1().x()) ) +
                  ( (point1.y() - Designer::d.p1().y()) *
                    (point1.y() - Designer::d.p1().y()) ) );

    dist2 = sqrt( ( (point2.x() - Designer::d.p1().x()) *
                    (point2.x() - Designer::d.p1().x()) ) +
                  ( (point2.y() - Designer::d.p1().y()) *
                    (point2.y() - Designer::d.p1().y()) ) );

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

void Designer::circle2CircleIntersect1()
{
    double x1 = 0;
    double y1 = 0;
    double r1 = 4;
    double x2 = 8;
    double y2 = 0;
    double r2 = 5;

    double dx = x2 - x1;
    double dy = y2 - y1;

    double cdist = sqrt( dx*dx + dy*dy );
    double adist = ( cdist*cdist + r1*r1 - r2*r2 ) / ( 2*cdist );

    double x = x1 + dx*adist/cdist + ( dy/cdist )*sqrt( r1*r1 - adist*adist );
    double y = y1 + dy*adist/cdist - ( dx/cdist )*sqrt( r1*r1 - adist*adist );
    double dist = sqrt( (0 - x)*(0 - x) + (4 - y)*(4 - y) );
    std::cout << x << " , " << y << " , " << dist << std::endl;

    x = x1 + dx*adist/cdist - ( dy/cdist )*sqrt( r1*r1 - adist*adist );
    y = y1 + dy*adist/cdist + ( dx/cdist )*sqrt( r1*r1 - adist*adist );
    dist = sqrt( (0 - x)*(0 - x) + (4 - y)*(4 - y) );
    std::cout << x << " , " << y << " , " << dist << std::endl;
}

QColor Designer::createColorScheme( double totalNumber, double number )
{
    double value = COLOR_SCHEME / totalNumber * number;

    double thress = value / COLOR_SCHEME;

    uchar r = 0;
    uchar g = 0;
    uchar b = 0;
    int sat = 255;

    if ( thress <= 0.2 )
    {
        b = sat;
        g = uchar( thress / 0.2 * sat );

        return QColor( r, g, b );
    }
    else if ( thress < 0.399 )
    {
        g = sat;
        b = uchar( sat - (thress-0.4) / 0.2 * sat );

        return QColor( r, g, b);
    }
    else if ( thress < 0.599 )
    {
        g = sat;
        r = uchar( (thress-0.6) / 0.2 * sat );

        return QColor( r, g, b );
    }
    else if ( thress < 0.799 )
    {
        r = sat;
        g = sat - uchar( (thress-0.8) / 0.2 * sat );

        return QColor( r, g, b );
    }
    else if ( thress < 0.999 )
    {
        r = sat;
        b = uchar( (thress-1) / 0.2 * sat );

        return QColor( r, g, b );
    }
    else if ( thress >= 1 )
    {
        r = sat;
        b = sat;

        return QColor( r, g, b );
    } else return QColor( 255, 255, 255 );
}
