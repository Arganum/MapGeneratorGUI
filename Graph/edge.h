#ifndef EDGE_H
#define EDGE_H

#include <string>
#include <QColor>

class Edge
{
public:
    Edge();
    ~Edge();

    double getLength();
    void setLength( double length );

    double getPixelLength();
    void setPixelLength( double pixelLength );

    double getSpeedLimit();
    void setSpeedLimit( double speedLimit );

    QColor getColor();
    void setColor( QColor color );

    int getFrom();
    void setFrom( int from );

    int getTo();
    void setTo( int to );

    std::string getIsOneWay();
    void setIsOneWay( std::string isOneWay );

private:
    double length;
    double pixelLength;
    double speedLimit;
    QColor color;

    int from;
    int to;

    std::string isOneWay;
};

#endif // EDGE_H
