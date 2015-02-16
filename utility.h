#ifndef UTILITY_H
#define UTILITY_H

#define FIRST_PARSE 0
#define SECOND_PARSE 1
#define MAP_SCALE 1000

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
    double minLatitude;
    double minLongitude;
    double maxLatitude;
    double maxLongitude;
};

#endif // UTILITY_H

