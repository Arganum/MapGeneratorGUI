#ifndef VERTEX_H
#define VERTEX_H

#include <QPointF>

#include "edge.h"

class Vertex
{
public:
    Vertex();
    ~Vertex();

    int getIndex();
    void setIndex( int index );

    QPointF getPoint();
    void setPoint( QPointF point );

    QColor getColor();
    void setColor( QColor color );

    double getSpeedLimit();
    void setSpeedLimit( double speedLimit );

    std::vector<Edge> getEdges();
    void setEdges( std::vector<Edge> edges );
    void addEdge( Edge edge );

private:
    int index;
    QPointF point;
    QColor color;

    double speedLimit;

    std::vector<Edge> edges;
};

#endif // VERTEX_H
