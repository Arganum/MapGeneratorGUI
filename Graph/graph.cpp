#include "graph.h"

Graph::Graph()
{

}

Graph::~Graph()
{

}

std::vector<Vertex> Graph::getVertices()
{
    return Graph::vertices;
}

void Graph::setVertices( std::vector<Vertex> vertices )
{
    Graph::vertices = vertices;
}

void Graph::addVertex( Vertex vertex )
{
    Graph::vertices.push_back( vertex );
}
