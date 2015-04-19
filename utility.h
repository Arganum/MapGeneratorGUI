#ifndef UTILITY_H
#define UTILITY_H

#define FIRST_PARSE 0
#define SECOND_PARSE 1
//#define MAP_SCALE 2000 // 1000 = 1 pixel = 1 meter
#define MAP_SCALE 2 // 1 = 1 pixel = 1 meter, 2 = 1 pixel = 0.5 meter.

#define COLOR_SCHEME 11390625 // 225^3

//What Road Type to Paint
#define MOTORWAY     1
#define TRUNK        1
#define PRIMARY      1
#define SECONDARY    1 //1 for trafficLight Test
#define TERTIARY     1
#define UNCLASSIFIED 1
#define RESIDENTIAL  1
#define SERVICE      0

//Speed Limit of the different Road Types in km/h
#define MOTORWAY_SPEED     130 // 110
#define TRUNK_SPEED         90
#define PRIMARY_SPEED       60
#define SECONDARY_SPEED     60
#define TERTIARY_SPEED      50
#define UNCLASSIFIED_SPEED  50
#define RESIDENTIAL_SPEED   50
#define SERVICE_SPEED       30

//Speed Limit of Traffic Lights and intersections in km/h
#define TRAFFICLIGHT_SPEED  20
#define INTERSECTION_SPEED  20

struct Node
{
    unsigned int number;
    unsigned long ID;
    double latitude;
    double longitude;

    bool operator < ( const Node & a ) const
    {
        return number < a.number;
    }
};

struct Bounds
{
    double width;
    double height;

    double minLatitude;
    double minLongitude;
    double maxLatitude;
    double maxLongitude;
};

/*
struct Lane
{
    double x;
    double y;

    double speedLimit;

    double red;
    double green;
    double blue;
};*/

#endif // UTILITY_H

