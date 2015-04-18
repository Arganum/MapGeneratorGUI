#ifndef ROAD_H
#define ROAD_H

#include <string>
#include <vector>
#include <QLine>
#include <QColor>

#include "utility.h"

class Road
{
public:
    Road();
    ~Road();

    std::string getName();
    void setName( std::string name );

    std::string getType();
    void setType( std::string type );

    std::string getIsOneWay();
    void setIsOneWay( std::string isOneWay );

    double getSpeedLimit();
    void setSpeedLimit( double speedLimit );

    std::vector<Node> getNodes();
    void setNodes( std::vector<Node> nodes );
    void addNode( Node node );
    void clearNodes();

    std::vector<unsigned long> getNodeIDs();
    void setNodeIDs( std::vector<unsigned long> nodeIDs );
    void addNodeID( unsigned long nodeID );
    void clearNodeIDs();

    std::vector<QLineF> getLines();
    void setLines( std::vector<QLineF> lines );
    void addLine( QLineF line );
    void clearLines();

    QColor getColor();
    void setColor( QColor color );

    double getLength();
    void setLength( double length );

private:
    std::string name;
    std::string type;
    std::string isOneWay;

    double speedLimit;

    std::vector<Node> nodes;
    std::vector<unsigned long> nodeIDs;

    std::vector<QLineF> lines;
    QColor color;
    double length;
};

#endif // ROAD_H
