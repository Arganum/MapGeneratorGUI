#ifndef UTILITY_H
#define UTILITY_H

#define FIRST_PARSE 0
#define SECOND_PARSE 1
#define MAP_SCALE 1000 // 1 pixel = 1 meter

//What Road Type to Paint
#define MOTORWAY     0
#define TRUNK        0
#define PRIMARY      0
#define SECONDARY    1
#define TERTIARY     0
#define UNCLASSIFIED 1
#define RESIDENTIAL  1
#define SERVICE      1

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
    double imageWidth;
    double imageHeight;

    double minLatitude;
    double minLongitude;
    double maxLatitude;
    double maxLongitude;
};

#endif // UTILITY_H

