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

    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        std::vector<Node> nodes = it->getNodes();

        for ( std::vector<Node>::iterator jt = nodes.begin();
              jt < (nodes.end() - 1); ++jt )
        {
            beginPoint = Designer::circle2CircleIntersect( jt->latitude, jt->longitude );
            endPoint   = Designer::circle2CircleIntersect( (jt+1)->latitude, (jt+1)->longitude );
            if ( ( beginPoint.x() > Designer::A.x() &&
                   beginPoint.y() > Designer::A.y() &&
                   beginPoint.x() < Designer::C.x() &&
                   beginPoint.y() < Designer::C.y() ) ||
                 ( endPoint.x() > Designer::A.x() &&
                   endPoint.y() > Designer::A.y() &&
                   endPoint.x() < Designer::C.x() &&
                   endPoint.y() < Designer::C.y() ) )
            {
                tempLine.setPoints( beginPoint, endPoint );
                it->addLine( tempLine );
            }
        }
    }

    roads = Designer::mergeRoads( roads );

    Designer::roadNetwork->setRoads( roads );
}

void Designer::createTrafficLightCoordinates()
{
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
                newTrafficLight.setSpeedLimit( TRAFFICLIGHT_SPEED );
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

                    if ( ( floor( kt->p1().x() + 0.5 ) - floor( it->getPoint().x() + 0.5 ) ) == 0  &&
                         ( floor( kt->p1().y() + 0.5 ) - floor( it->getPoint().y() + 0.5 ) ) == 0 )
                    {
                        Lane lane;
                        lane.x = kt->p2().x() - it->getPoint().x();
                        lane.y = kt->p2().y() - it->getPoint().y();

                        lane.speedLimit = jt->getSpeedLimit();

                        lane.red = jt->getColor().red();
                        lane.green = jt->getColor().green();
                        lane.blue = jt->getColor().blue();

                        lanes.push_back( lane );
                    }
                    else if ( ( floor( kt->p2().x() + 0.5 ) - floor( it->getPoint().x() + 0.5 ) ) == 0  &&
                                ( floor( kt->p2().y() + 0.5 ) - floor( it->getPoint().y() + 0.5 ) ) == 0 )
                    {
                        Lane lane;
                        lane.x = kt->p1().x() - it->getPoint().x();
                        lane.y = kt->p1().y() - it->getPoint().y();

                        lane.speedLimit = jt->getSpeedLimit();

                        lane.red = jt->getColor().red();
                        lane.green = jt->getColor().green();
                        lane.blue = jt->getColor().blue();

                        lanes.push_back( lane );
                    }

                    std::cout << std::endl;
                }
            }
        }

        it->setLanes( lanes );

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

            for ( std::vector<Lane>::iterator mt = lanes.begin();
                  mt != lanes.end(); ++mt )
            {
                if ( lt - lanes.begin() == mt - lanes.begin() )
                    continue;

                tempSwitch1[0] = *lt;
                tempSwitch1[1] = *mt;

                temp = QPointF::dotProduct( QPointF( lt->x, lt->y ), QPointF( mt->x, mt->y ) );


                if ( highest > temp )
                {
                    //std::cout <<  lt->x << " , " << lt->y << " : " << mt->x << " , " << mt->y << std::endl;
                    //std::cout << QPointF::dotProduct( QPointF( lt->x, lt->y ), QPointF( mt->x, mt->y ) ) << std::endl << std::endl;
                    highest = temp;
                    switch1 = tempSwitch1;
                }
            }
        }

        for ( std::vector<Lane>::iterator lt = lanes.begin();
              lt != lanes.end(); ++lt )
        {
            if ( floor( switch1[0].x + 0.5 ) != floor( lt->x + 0.5 ) && floor( switch1[0].y + 0.5 ) != floor( lt->y + 0.5 ) &&
                 floor( switch1[1].x + 0.5 ) != floor( lt->x + 0.5 ) && floor( switch1[1].y + 0.5 ) != floor( lt->y + 0.5 ) )
            {
                switch2.push_back( *lt );
            }
        }

        it->setSwitch1( switch1 );
        it->setSwitch2( switch2 );
    }
    Designer::roadNetwork->setTrafficLights( trafficLights );
}

void Designer::createIntersectionCoordinates()
{
    std::vector<QPointF> intersectPoints;
    std::vector<Road> roads =  Designer::roadNetwork->getRoads();
    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        std::vector<QLineF> origLines = it->getLines();

        for ( std::vector<Road>::iterator jt = roads.begin();
              jt != roads.end(); ++jt )
        {
            std::vector<QLineF> testLines = jt->getLines();

            for ( std::vector<QLineF>::iterator kt = origLines.begin();
                  kt != origLines.end(); ++kt )
            {
                QPointF tempPoint;

                for ( std::vector<QLineF>::iterator lt = testLines.begin();
                      lt != testLines.end(); ++lt )
                {
                    if ( floor( kt->p1().x() + 0.5 ) == floor( lt->p1().x() + 0.5 ) &&
                         floor( kt->p1().y() + 0.5 ) == floor( lt->p1().y() + 0.5 ) )
                    {
                        tempPoint = kt->p1();
                        intersectPoints.push_back( tempPoint );
                    }
                    else if ( floor( kt->p2().x() + 0.5 ) == floor( lt->p2().x() + 0.5 ) &&
                              floor( kt->p2().y() + 0.5 ) == floor( lt->p2().y() + 0.5 ) )
                    {
                        tempPoint = kt->p2();
                        intersectPoints.push_back( tempPoint );
                    }
                }
            }
        }
    }

    intersectPoints = Designer::sortIntersectionCoordinates( intersectPoints );

    Intersection tempIntersection;
    for ( std::vector<QPointF>::iterator mt = intersectPoints.begin();
          mt != intersectPoints.end(); ++mt )
    {
        tempIntersection.setPoint( *mt );
        tempIntersection.setSpeedLimit( INTERSECTION_SPEED );
        //std::cout << mt->x() << " , " << mt->y() << std::endl;
        Designer::roadNetwork->addIntersection( tempIntersection );
    }
}

void Designer::createIntersectionLanes()
{
    std::vector<Intersection> intersections = Designer::roadNetwork->getIntersections();
    std::vector<Road> roads = Designer::roadNetwork->getRoads();

    for ( std::vector<Intersection>::iterator it = intersections.begin();
          it != intersections.end(); ++it )
    {
        for ( std::vector<Road>::iterator jt = roads.begin();
              jt != roads.end(); ++jt )
        {
            std::vector<QLineF> lines = jt->getLines();
            Lane tempLane;

            for ( std::vector<QLineF>::iterator kt = lines.begin();
                  kt != lines.end(); ++kt )
            {
                if ( floor( kt->p1().x() + 0.5 ) == floor( it->getPoint().x() + 0.5 ) &&
                     floor( kt->p1().y() + 0.5 ) == floor( it->getPoint().y() + 0.5 ) )
                {
                    tempLane.speedLimit = jt->getSpeedLimit();

                    tempLane.red = jt->getColor().red();
                    tempLane.green = jt->getColor().green();
                    tempLane.blue = jt->getColor().blue();
                    //std::cout << tempLane.red << " , " << tempLane.green << " , " << tempLane.blue << std::endl;
                    it->addLane( tempLane );
                }
                else if ( floor( kt->p2().x() + 0.5 ) == floor( it->getPoint().x() + 0.5 ) &&
                          floor( kt->p2().y() + 0.5 ) == floor( it->getPoint().y() + 0.5 ) )
                {
                    tempLane.speedLimit = jt->getSpeedLimit();

                    tempLane.red = jt->getColor().red();
                    tempLane.green = jt->getColor().green();
                    tempLane.blue = jt->getColor().blue();
                    //std::cout << tempLane.red << " , " << tempLane.green << " , " << tempLane.blue << std::endl;
                    it->addLane( tempLane );
                }
            }
        }
    }

    Designer::roadNetwork->setIntersections( intersections );
}

void Designer::createColorScheme()
{
    std::vector<Road> roads = Designer::roadNetwork->getRoads();
    std::vector<TrafficLight> trafficLights = Designer::roadNetwork->getTrafficLights();
    std::vector<Intersection> intersections = Designer::roadNetwork->getIntersections();

    double totalNumber = roads.size() + trafficLights.size() + intersections.size();
    //std::cout << totalNumber << std::endl;
    int indexNumber = 0;

    for ( std::vector<Road>::iterator it = roads.begin();
              it != roads.end(); ++it )
    {
        ++indexNumber;
        QColor color;
        color = Designer::createColorScheme( totalNumber, indexNumber );
        it->setColor ( color );
    }

    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
              it != trafficLights.end(); ++it )
    {
        ++indexNumber;
        QColor color;
        color = Designer::createColorScheme( totalNumber, indexNumber );
        it->setColor ( color );
    }

    for ( std::vector<Intersection>::iterator it = intersections.begin();
              it != intersections.end(); ++it )
    {
        ++indexNumber;
        QColor color;
        color = Designer::createColorScheme( totalNumber, indexNumber );
        it->setColor ( color );
    }

    Designer::roadNetwork->setRoads( roads );
    Designer::roadNetwork->setTrafficLights( trafficLights );
    Designer::roadNetwork->setIntersections( intersections );
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

    uchar r = 30;
    uchar g = 30;
    uchar b = 30;
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

// Some roads in the osm file are divided into two ways. so in this code they would get a different color.
// Even though it is the same road. This will create a problem with the graph creation.
// Since a road might change color without an intersection or trafficlight.
// And this in turn does so that the Route behavior of the Car agents breaks down.
// Lines række følgen vil ikke længere være seqventiel efter denne operation.
std::vector<Road> Designer::mergeRoads( std::vector<Road> roads )
{
    std::vector<Road> newRoads;
    std::unordered_set<std::string> uniqueness;

    //Evt return type se sortIntersectionCoordinates
    for ( std::vector<Road>::iterator it = roads.begin();
          it != roads.end(); ++it )
    {
        std::vector<QLineF> origlines = it->getLines();

        for ( std::vector<Road>::iterator jt = roads.begin();
              jt != roads.end(); ++jt )
        {
            std::vector<QLineF> lines = jt->getLines();

            if ( it - roads.begin() == jt - roads.begin() )
                continue;

            if ( it->getName() == jt->getName() && it->getType() == jt->getType() )
            {
                origlines.insert( origlines.end(), lines.begin(), lines.end() );
            }
        }

        if ( uniqueness.count( it->getName() + it->getType() ) == 0 )
        {
            uniqueness.insert( it->getName() + it->getType() );
            it->setLines( origlines );
            newRoads.push_back( *it );
        }
    }

    return newRoads;
}

std::vector<QPointF> Designer::sortIntersectionCoordinates( std::vector<QPointF> coordinates )
{
    std::vector<QPointF> intersectionPoints;
    QPointF tempPoint;

    for ( std::vector<QPointF>::iterator it = coordinates.begin();
          it != coordinates.end(); ++it )
    {
        int count = 0;
        tempPoint = *it;
        for ( std::vector<QPointF>::iterator jt = coordinates.begin();
              jt != coordinates.end(); ++jt )
        {
            if ( floor( it->x() + 0.5 ) == floor( jt->x() + 0.5 ) &&
                 floor( it->y() + 0.5 ) == floor( jt->y() + 0.5 ) )
            {
                count++;
            }
        }

        if ( tempPoint.x() > Designer::A.x() && tempPoint.y() > Designer::A.y() &&
             tempPoint.x() < Designer::C.x() && tempPoint.y() < Designer::C.y() && count > 2 )
        {
            bool isDuplicate = false;

            for ( std::vector<QPointF>::iterator kt = intersectionPoints.begin();
                  kt != intersectionPoints.end(); ++kt )
            {
                if ( floor( tempPoint.x() + 0.5 ) == floor( kt->x() + 0.5 ) &&
                     floor( tempPoint.y() + 0.5 ) == floor( kt->y() + 0.5 ) )
                {
                    isDuplicate = true;
                    break;
                }
            }

            //Test if the Intersect is a trafficLight.
            std::vector<TrafficLight> trafficLights = Designer::roadNetwork->getTrafficLights();
            for ( std::vector<TrafficLight>::iterator lt = trafficLights.begin();
                  lt != trafficLights.end(); ++lt )
            {
                if ( floor( tempPoint.x() + 0.5 ) == floor( lt->getPoint().x() + 0.5 ) &&
                     floor( tempPoint.y() + 0.5 ) == floor( lt->getPoint().y() + 0.5 ) )
                {
                    isDuplicate = true;
                    break;
                }
            }

            if ( isDuplicate == false )
            {
                intersectionPoints.push_back( tempPoint );
                //std::cout << tempPoint.x() << " , " << tempPoint.y() << " , " << count << std::endl;
            }
        }
    }

    return intersectionPoints;
}
