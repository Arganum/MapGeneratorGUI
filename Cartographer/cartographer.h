#ifndef CARTOGRAPHER_H
#define CARTOGRAPHER_H

#include <QFile>
#include <QTextStream>

#include "utility.h"
#include "roadnetwork.h"
#include "Graph/graph.h"

class RoadNetwork;
class Cartographer
{
public:
    Cartographer( RoadNetwork* roadNetwork );
    ~Cartographer();

    void createVertices();
    void createEdges();
    void printGraph();
    void createLuaFile();

private:
    RoadNetwork* roadNetwork;

    Graph roadMap;

    double distVertex2Vertex( Vertex vertex1, Vertex vertex2 );
    bool isEdgeUnique(std::vector<Edge> edges, Edge edge );
};

#endif // CARTOGRAPHER_H
