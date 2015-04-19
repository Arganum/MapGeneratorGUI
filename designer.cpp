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

    /*
    width  = Designer::latLon2Length( boundary.minLatitude,
                                      boundary.maxLongitude,
                                      boundary.minLatitude,
                                      boundary.minLongitude );
    height = Designer::latLon2Length( boundary.maxLatitude,
                                      boundary.minLongitude,
                                      boundary.minLatitude,
                                      boundary.minLongitude );
    */

    width  = Designer::haversine( boundary.minLatitude,
                                  boundary.maxLongitude,
                                  boundary.minLatitude,
                                  boundary.minLongitude );
    height = Designer::haversine( boundary.maxLatitude,
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
                if ( beginPoint.x() > A.x() && endPoint.x() > A.x() &&
                     beginPoint.y() > A.y() && endPoint.y() > A.y() &&
                     beginPoint.x() < C.x() && endPoint.x() < C.x() &&
                     beginPoint.y() < C.y() && endPoint.y() < C.y() )
                {
                    it->addLine( tempLine );
                }
                else if ( boundaryAdjustedLine( tempLine, a ) )
                {
                    it->addLine( tempLine );
                }
                else if ( boundaryAdjustedLine( tempLine, b ) )
                {
                    it->addLine( tempLine );
                }
                else if ( boundaryAdjustedLine( tempLine, c ) )
                {
                    it->addLine( tempLine );
                }
                else if ( boundaryAdjustedLine( tempLine, d ) )
                {
                    it->addLine( tempLine );
                }
            }
        }
    }

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
    std::vector<TrafficLight> newTrafficLights;
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

                        QPointF point;
                        point.setX( kt->p2().x() - it->getPoint().x() );
                        point.setY( kt->p2().y() - it->getPoint().y() );

                        lane.setPoint( point );
                        lane.setSpeedLimit( jt->getSpeedLimit() );
                        lane.setColor( jt->getColor() );
                        lane.setLength( jt->getLength() );
                        lane.setPixelLength( jt->getPixelLength() );

                        if ( jt->getIsOneWay()=="yes" )
                        {
                            lane.setIsOneWay( "To" );
                            //std::cout << "To: " << lane.getColor().red() << " , " << lane.getColor().green() << " , " << lane.getColor().blue() << std::endl;
                            //std::cout << "To: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                        } else if ( jt->getIsOneWay()=="-1" ) {
                            lane.setIsOneWay( "From" );
                            //std::cout << "From: " << lane.getColor().red() << " , " << lane.getColor().green() << " , " << lane.getColor().blue() << std::endl;
                            //std::cout << "From: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                        } else {
                            lane.setIsOneWay( "FromTo" );
                        }

                        lanes.push_back( lane );
                    }
                    else if ( ( floor( kt->p2().x() + 0.5 ) - floor( it->getPoint().x() + 0.5 ) ) == 0  &&
                                ( floor( kt->p2().y() + 0.5 ) - floor( it->getPoint().y() + 0.5 ) ) == 0 )
                    {
                        Lane lane;

                        QPointF point;
                        point.setX( kt->p1().x() - it->getPoint().x() );
                        point.setY( kt->p1().y() - it->getPoint().y() );

                        lane.setPoint( point );
                        lane.setSpeedLimit( jt->getSpeedLimit() );
                        lane.setColor( jt->getColor() );
                        lane.setLength( jt->getLength() );
                        lane.setPixelLength( jt->getPixelLength() );

                        if ( jt->getIsOneWay()=="yes" )
                        {
                            lane.setIsOneWay( "To" );
                            //std::cout << "To: " << lane.getColor().red() << " , " << lane.getColor().green() << " , " << lane.getColor().blue() << std::endl;
                            //std::cout << "To: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                        } else if ( jt->getIsOneWay()=="-1" ) {
                            lane.setIsOneWay( "From" );
                            //std::cout << "From: " << lane.getColor().red() << " , " << lane.getColor().green() << " , " << lane.getColor().blue() << std::endl;
                            //std::cout << "From: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                        } else {
                            lane.setIsOneWay( "FromTo" );
                        }

                        lanes.push_back( lane );
                    }
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

                temp = QPointF::dotProduct( lt->getPoint(), mt->getPoint() );

                if ( highest > temp )
                {
                    highest = temp;
                    switch1 = tempSwitch1;
                }
            }
        }

        if ( switch1.size()>=2 )
        {
            for ( std::vector<Lane>::iterator lt = lanes.begin();
                  lt != lanes.end(); ++lt )
            {
                if ( floor( switch1[0].getPoint().x() + 0.5 ) != floor( lt->getPoint().x() + 0.5 ) && floor( switch1[0].getPoint().y() + 0.5 ) != floor( lt->getPoint().y() + 0.5 ) &&
                     floor( switch1[1].getPoint().x() + 0.5 ) != floor( lt->getPoint().x() + 0.5 ) && floor( switch1[1].getPoint().y() + 0.5 ) != floor( lt->getPoint().y() + 0.5 ) )
                {
                    switch2.push_back( *lt );
                }
            }

            TrafficLight tempTrafficLight = *it;
            tempTrafficLight.setSwitch1( switch1 );
            tempTrafficLight.setSwitch2( switch2 );
            newTrafficLights.push_back( tempTrafficLight );
        }
    }
    Designer::roadNetwork->setTrafficLights( newTrafficLights );
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
            if ( it - roads.begin() == jt - roads.begin() )
                continue;

            std::vector<QLineF> testLines = jt->getLines();

            for ( std::vector<QLineF>::iterator kt = origLines.begin();
                  kt != origLines.end(); ++kt )
            {
                QPointF tempPoint;

                for ( std::vector<QLineF>::iterator lt = testLines.begin();
                      lt != testLines.end(); ++lt )
                {
                    if ( ( floor( kt->p1().x() + 0.5 ) == floor( lt->p1().x() + 0.5 ) &&
                           floor( kt->p1().y() + 0.5 ) == floor( lt->p1().y() + 0.5 ) ) ||
                         ( floor( kt->p1().x() + 0.5 ) == floor( lt->p2().x() + 0.5 ) &&
                           floor( kt->p1().y() + 0.5 ) == floor( lt->p2().y() + 0.5 ) ) )
                    {
                        tempPoint = kt->p1();
                        //std::cout << tempPoint.x() << " , " << tempPoint.y() << std::endl;
                        intersectPoints.push_back( tempPoint );
                    }
                    else if ( ( floor( kt->p2().x() + 0.5 ) == floor( lt->p2().x() + 0.5 ) &&
                                floor( kt->p2().y() + 0.5 ) == floor( lt->p2().y() + 0.5 ) ) ||
                              ( floor( kt->p2().x() + 0.5 ) == floor( lt->p1().x() + 0.5 ) &&
                                floor( kt->p2().y() + 0.5 ) == floor( lt->p1().y() + 0.5 ) ) )
                    {
                        tempPoint = kt->p2();
                        //std::cout << tempPoint.x() << " , " << tempPoint.y() << std::endl;
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
                    QPointF point;
                    point.setX( kt->p1().x() - it->getPoint().x() );
                    point.setY( kt->p1().y() - it->getPoint().y() );

                    tempLane.setPoint( point );
                    tempLane.setSpeedLimit( jt->getSpeedLimit() );
                    tempLane.setColor( jt->getColor() );
                    tempLane.setLength( jt->getLength() );
                    tempLane.setPixelLength( jt->getPixelLength() );

                    if ( jt->getIsOneWay()=="yes" )
                    {
                        tempLane.setIsOneWay( "From" );
                        //std::cout << "From: " << tempLane.getColor().red() << " , " << tempLane.getColor().green() << " , " << tempLane.getColor().blue() << std::endl;
                        //std::cout << "From: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                    } else if ( jt->getIsOneWay()=="-1" ) {
                        tempLane.setIsOneWay( "To" );
                        //std::cout << "To: " << tempLane.getColor().red() << " , " << tempLane.getColor().green() << " , " << tempLane.getColor().blue() << std::endl;
                        //std::cout << "To: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                    } else {
                        tempLane.setIsOneWay( "FromTo" );
                    }
                    it->addLane( tempLane );
                }
                else if ( floor( kt->p2().x() + 0.5 ) == floor( it->getPoint().x() + 0.5 ) &&
                          floor( kt->p2().y() + 0.5 ) == floor( it->getPoint().y() + 0.5 ) )
                {
                    QPointF point;
                    point.setX( kt->p2().x() - it->getPoint().x() );
                    point.setY( kt->p2().y() - it->getPoint().y() );

                    tempLane.setPoint( point );
                    tempLane.setSpeedLimit( jt->getSpeedLimit() );
                    tempLane.setColor( jt->getColor() );
                    tempLane.setLength( jt->getLength() );
                    tempLane.setPixelLength( jt->getPixelLength() );

                    if ( jt->getIsOneWay()=="yes" )
                    {
                        tempLane.setIsOneWay( "To" );
                        //std::cout << "To: " << tempLane.getColor().red() << " , " << tempLane.getColor().green() << " , " << tempLane.getColor().blue() << std::endl;
                        //std::cout << "To: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                    } else if ( jt->getIsOneWay()=="-1" ) {
                        tempLane.setIsOneWay( "From" );
                        //std::cout << "From: " << tempLane.getColor().red() << " , " << tempLane.getColor().green() << " , " << tempLane.getColor().blue() << std::endl;
                        //std::cout << "From: " << floor( it->getPoint().x() + 0.5 ) << " , " << floor( it->getPoint().y() + 0.5 ) << std::endl;
                    } else {
                        tempLane.setIsOneWay( "FromTo" );
                    }
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
    roads = Designer::splitRoads( roads );
    roads = Designer::calculateRoadsLengths( roads );
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

double Designer::latLon2Length( double endLatitude,   double endLongitude,
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

double Designer::haversine( double endLatitude,   double endLongitude,
                            double beginLatitude, double beginLongitude )
{
    double R = 6371000; //Earth radius in meters.
    double lat1 = Designer::deg2Rad( endLatitude );
    double lat2 = Designer::deg2Rad( beginLatitude );
    double deltaLat = Designer::deg2Rad( beginLatitude - endLatitude );
    double deltaLon = Designer::deg2Rad( beginLongitude - endLongitude );

    double aa = sin( deltaLat/2 ) * sin( deltaLat/2 ) +
                cos( lat1 ) * cos( lat2 ) *
                sin( deltaLon/2 ) * sin( deltaLon/2 );
    double cc = 2 * atan2( sqrt( aa ), sqrt( 1 - aa ) );
    double dist = R * cc * MAP_SCALE;

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

    /*
    r1 = latLon2Length( latitude, longitude,
                        Designer::roadNetwork->getBoundary().minLatitude,
                        Designer::roadNetwork->getBoundary().minLongitude );
    r2 = latLon2Length( latitude, longitude,
                        Designer::roadNetwork->getBoundary().minLatitude,
                        Designer::roadNetwork->getBoundary().maxLongitude );
    r3 = latLon2Length( latitude, longitude,
                        Designer::roadNetwork->getBoundary().maxLatitude,
                        Designer::roadNetwork->getBoundary().minLongitude );
    */

    r1 = Designer::haversine( latitude, longitude,
                              Designer::roadNetwork->getBoundary().minLatitude,
                              Designer::roadNetwork->getBoundary().minLongitude );
    r2 = Designer::haversine( latitude, longitude,
                              Designer::roadNetwork->getBoundary().minLatitude,
                              Designer::roadNetwork->getBoundary().maxLongitude );
    r3 = Designer::haversine( latitude, longitude,
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

std::vector<Road> Designer::splitRoads( std::vector<Road> roads )
{
    std::vector<Road> newRoads = roads;
    std::vector<TrafficLight> trafficLights = Designer::roadNetwork->getTrafficLights();
    std::vector<Intersection> intersections = Designer::roadNetwork->getIntersections();

    double splitCount = Designer::splitRoadsCount( roads );

    for ( int i = 0; i != splitCount; ++i )
    {
        std::vector<Road> tempRoads;

        for ( std::vector<Road>::iterator it = newRoads.begin();
              it != newRoads.end(); ++it )
        {

            bool isSplit = true;
            std::vector<QLineF> lines = it->getLines();

            for ( std::vector<QLineF>::iterator jt = lines.begin();
                  jt < (lines.end() - 1); ++ jt )
            {
                for ( std::vector<TrafficLight>::iterator kt = trafficLights.begin();
                      kt != trafficLights.end(); ++kt )
                {
                    if ( floor( jt->p2().x() + 0.5 ) == floor( kt->getPoint().x() + 0.5 ) &&
                         floor( jt->p2().y() + 0.5 ) == floor( kt->getPoint().y() + 0.5 ) &&
                         floor( (jt+1)->p1().x() + 0.5 ) == floor( kt->getPoint().x() + 0.5 ) &&
                         floor( (jt+1)->p1().y() + 0.5 ) == floor( kt->getPoint().y() + 0.5 ) &&
                         isSplit == true )
                    {
                        Road split1;
                        split1.setName( it->getName() );
                        split1.setType( it->getType() );
                        split1.setIsOneWay( it->getIsOneWay() );
                        split1.setColor( it->getColor() );
                        split1.setSpeedLimit( it->getSpeedLimit() );
                        std::vector<QLineF> tempLines1;
                        tempLines1.insert( tempLines1.begin(), lines.begin(), (jt+1) ); // (jt+1)
                        split1.setLines( tempLines1 );
                        tempRoads.push_back( split1 );

                        Road split2;
                        split2.setName( it->getName() );
                        split2.setType( it->getType() );
                        split2.setIsOneWay( it->getIsOneWay() );
                        split2.setColor( it->getColor() );
                        split2.setSpeedLimit( it->getSpeedLimit() );
                        std::vector<QLineF> tempLines2;
                        tempLines2.insert( tempLines2.begin(), (jt+1), lines.end() );
                        split2.setLines( tempLines2 );
                        tempRoads.push_back( split2 );

                        isSplit = false;
                    }
                }

                for ( std::vector<Intersection>::iterator lt = intersections.begin();
                      lt != intersections.end(); ++lt )
                {
                    if ( floor( jt->p2().x() + 0.5 ) == floor( lt->getPoint().x() + 0.5 ) &&
                         floor( jt->p2().y() + 0.5 ) == floor( lt->getPoint().y() + 0.5 ) &&
                         floor( (jt+1)->p1().x() + 0.5 ) == floor( lt->getPoint().x() + 0.5 ) &&
                         floor( (jt+1)->p1().y() + 0.5 ) == floor( lt->getPoint().y() + 0.5 ) &&
                         isSplit == true )
                    {
                        Road split1;
                        split1.setName( it->getName() );
                        split1.setType( it->getType() );
                        split1.setIsOneWay( it->getIsOneWay() );
                        split1.setColor( it->getColor() );
                        split1.setSpeedLimit( it->getSpeedLimit() );
                        std::vector<QLineF> tempLines1;
                        tempLines1.insert( tempLines1.begin(), lines.begin(), (jt+1) );
                        split1.setLines( tempLines1 );
                        tempRoads.push_back( split1 );

                        Road split2;
                        split2.setName( it->getName() );
                        split2.setType( it->getType() );
                        split2.setIsOneWay( it->getIsOneWay() );
                        split2.setColor( it->getColor() );
                        split2.setSpeedLimit( it->getSpeedLimit() );
                        std::vector<QLineF> tempLines2;
                        tempLines2.insert( tempLines2.begin(), (jt+1), lines.end() );
                        split2.setLines( tempLines2 );
                        tempRoads.push_back( split2 );

                        isSplit = false;
                    }
                }
            }

            if ( isSplit == true )
            {
                Road tempRoad;
                tempRoad.setName( it->getName() );
                tempRoad.setType( it->getType() );
                tempRoad.setIsOneWay( it->getIsOneWay() );
                tempRoad.setColor( it->getColor() );
                tempRoad.setSpeedLimit( it->getSpeedLimit() );
                tempRoad.setLines( it->getLines() );
                tempRoads.push_back( tempRoad );
            }
        }

        newRoads = tempRoads;
    }

    return newRoads;
}

double Designer::splitRoadsCount( std::vector<Road> roads )
{
    std::vector<TrafficLight> trafficLights = Designer::roadNetwork->getTrafficLights();
    std::vector<Intersection> intersections = Designer::roadNetwork->getIntersections();

    double count = 0;
    for ( std::vector<Road>::iterator it = roads.begin();
              it != roads.end(); ++it )
        {

        std::vector<QLineF> lines = it->getLines();

        for ( std::vector<QLineF>::iterator jt = lines.begin();
              jt < (lines.end() - 1); ++ jt )
        {
            for ( std::vector<TrafficLight>::iterator kt = trafficLights.begin();
                  kt != trafficLights.end(); ++kt )
            {
                if ( floor( jt->p2().x() + 0.5 ) == floor( kt->getPoint().x() + 0.5 ) &&
                     floor( jt->p2().y() + 0.5 ) == floor( kt->getPoint().y() + 0.5 ) &&
                     floor( (jt+1)->p1().x() + 0.5 ) == floor( kt->getPoint().x() + 0.5 ) &&
                     floor( (jt+1)->p1().y() + 0.5 ) == floor( kt->getPoint().y() + 0.5 ) )
                {
                    count++;
                }
            }

            for ( std::vector<Intersection>::iterator lt = intersections.begin();
                  lt != intersections.end(); ++lt )
            {
                if ( floor( jt->p2().x() + 0.5 ) == floor( lt->getPoint().x() + 0.5 ) &&
                     floor( jt->p2().y() + 0.5 ) == floor( lt->getPoint().y() + 0.5 ) &&
                     floor( (jt+1)->p1().x() + 0.5 ) == floor( lt->getPoint().x() + 0.5 ) &&
                     floor( (jt+1)->p1().y() + 0.5 ) == floor( lt->getPoint().y() + 0.5 ) )
                {
                    count++;
                }
            }
        }
    }

    return count;
}

std::vector<Road> Designer::calculateRoadsLengths( std::vector<Road> roads )
{
    std::vector<Road> newRoads = roads;

    for ( std::vector<Road>::iterator it = newRoads.begin();
          it != newRoads.end(); ++it )
    {
        double length = 0;
        std::vector<QLineF> lines = it->getLines();

        for ( std::vector<QLineF>::iterator jt = lines.begin();
              jt != lines.end(); ++ jt )
        {
            length = length + jt->length();
        }

        it->setLength( length/MAP_SCALE );
        it->setPixelLength( length );
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
             tempPoint.x() < Designer::C.x() && tempPoint.y() < Designer::C.y() ) //&& count > 2
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

bool Designer::boundaryAdjustedLine( QLineF & line, QLineF intersectLine )
{
    bool temp = false;
    QPointF intersectionPoint;
    int intersect = line.intersect( intersectLine, &intersectionPoint );

    if ( intersect == QLineF::BoundedIntersection )
    {
        if ( !pointWithInBoundary( line.p1() ) )
        {
            line.setP1( intersectionPoint );
            temp = true;
        } else {
            line.setP2( intersectionPoint );
            temp = true;
        }
    }
    return temp;
}

bool Designer::pointWithInBoundary( QPointF point )
{
    bool temp = false;
    bool withInBoundary =   point.x() > A.x() && point.y() > A.y() &&
                            point.x() < C.x() && point.y() < C.y();

    if ( withInBoundary )
        temp = true;

    return temp;
}
