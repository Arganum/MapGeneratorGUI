#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::~Vertex()
{

}

int Vertex::getIndex()
{
    return Vertex::index;
}

void Vertex::setIndex( int index )
{
    Vertex::index = index;
}

QPointF Vertex::getPoint()
{
    return Vertex::point;
}

void Vertex::setPoint( QPointF point )
{
    Vertex::point = point;
}

QColor Vertex::getColor()
{
    return Vertex::color;
}

void Vertex::setColor( QColor color )
{
    Vertex::color = color;
}

double Vertex::getSpeedLimit()
{
    return Vertex::speedLimit;
}

void Vertex::setSpeedLimit( double speedLimit )
{
    Vertex::speedLimit = speedLimit;
}

std::vector<Edge> Vertex::getEdges()
{
    return Vertex::edges;
}

void Vertex::setEdges( std::vector<Edge> edges )
{
    Vertex::edges = edges;
}

void Vertex::addEdge( Edge edge )
{
    Vertex::edges.push_back( edge );
}
