#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"

class Graph
{
public:
    Graph();
    ~Graph();

    std::vector<Vertex> getVertices();
    void setVertices( std::vector<Vertex> vertices );
    void addVertex( Vertex vertex );

private:
    std::vector<Vertex> vertices;
};

#endif // GRAPH_H
