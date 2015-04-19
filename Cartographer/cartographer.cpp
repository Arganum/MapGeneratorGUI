#include "cartographer.h"

Cartographer::Cartographer(RoadNetwork *roadNetwork)
{
    Cartographer::roadNetwork = roadNetwork;
}

Cartographer::~Cartographer()
{

}

void Cartographer::createVertices()
{  
    std::vector<TrafficLight> trafficLights = Cartographer::roadNetwork->getTrafficLights();
    std::vector<Intersection> intersections = Cartographer::roadNetwork->getIntersections();

    int vertexIndex = 0;
    for ( std::vector<TrafficLight>::iterator it = trafficLights.begin();
          it != trafficLights.end(); ++it )
    {
        std::vector<Lane> lanes = it->getLanes();

        ++vertexIndex;
        Vertex vertex;
        vertex.setIndex( vertexIndex );
        vertex.setPoint( it->getPoint() );
        vertex.setColor( it->getColor() );
        vertex.setSpeedLimit( it->getSpeedLimit() );
        for ( std::vector<Lane>::iterator jt = lanes.begin();
              jt != lanes.end(); ++jt )
        {
            Edge edge;
            edge.setLength( jt->getLength() );
            edge.setPixelLength( jt->getPixelLength() );
            edge.setColor( jt->getColor() );
            edge.setSpeedLimit( jt->getSpeedLimit() );
            edge.setIsOneWay( jt->getIsOneWay() );
            vertex.addEdge( edge );
        }

        Cartographer::roadMap.addVertex( vertex );
    }

    for ( std::vector<Intersection>::iterator it = intersections.begin();
          it != intersections.end(); ++it )
    {
        std::vector<Lane> lanes = it->getLanes();

        ++vertexIndex;
        Vertex vertex;
        vertex.setIndex( vertexIndex );
        vertex.setPoint( it->getPoint() );
        vertex.setColor( it->getColor() );
        vertex.setSpeedLimit( it->getSpeedLimit() );
        for ( std::vector<Lane>::iterator jt = lanes.begin();
              jt != lanes.end(); ++jt )
        {
            Edge edge;
            edge.setLength( jt->getLength() );
            edge.setPixelLength( jt->getPixelLength() );
            edge.setColor( jt->getColor() );
            edge.setSpeedLimit( jt->getSpeedLimit() );
            edge.setIsOneWay( jt->getIsOneWay() );
            vertex.addEdge( edge );
        }

        Cartographer::roadMap.addVertex( vertex );
    }
}

void Cartographer::createEdges()
{
    std::vector<Vertex> vertices = Cartographer::roadMap.getVertices();
    std::vector<Vertex> newVertices;

    for ( std::vector<Vertex>::iterator it = vertices.begin();
          it != vertices.end(); ++it )
    {
        std::vector<Edge> origEdges = it->getEdges();
        std::vector<Edge> newEdges;

        for ( std::vector<Edge>::iterator jt = origEdges.begin();
              jt != origEdges.end(); ++jt )
        {
            double closestVertex = 0;
            double secondClosestVertex = 0;
            double closestIndex = 0;
            double secondClosestIndex = 0;

            for ( std::vector<Vertex>::iterator kt = vertices.begin();
                  kt != vertices.end(); ++ kt )
            {
                if ( it - vertices.begin() == kt - vertices.begin() )
                    continue;

                std::vector<Edge> testEdges = kt->getEdges();
                for ( std::vector<Edge>::iterator lt = testEdges.begin();
                      lt != testEdges.end(); ++lt )
                {
                    if ( jt->getColor() == lt->getColor() )
                    {
                        double tempDist = Cartographer::distVertex2Vertex( *it, *kt );

                        if ( closestVertex == 0 ) {
                            closestVertex = tempDist;
                            closestIndex = kt->getIndex();
                        } else if ( secondClosestVertex == 0 && tempDist > closestVertex ) {
                            secondClosestVertex = tempDist;
                            secondClosestIndex = kt->getIndex();
                        } else if ( secondClosestVertex == 0 && tempDist < closestVertex ) {
                            secondClosestVertex = closestVertex;
                            closestVertex = tempDist;

                            secondClosestIndex = closestIndex;
                            closestIndex = kt->getIndex();
                        } else if ( tempDist < closestVertex ){
                            secondClosestVertex = closestVertex;
                            closestVertex = tempDist;

                            secondClosestIndex = closestIndex;
                            closestIndex = kt->getIndex();
                        } else if ( tempDist > closestVertex && tempDist < secondClosestVertex ) {
                            secondClosestVertex = tempDist;
                            secondClosestIndex = kt->getIndex();
                        }
                    }
                }
            }

            Edge edge = *jt;
            edge.setTo( closestIndex );
            if ( Cartographer::isEdgeUnique( origEdges, edge ) == true ) {
                edge.setFrom( it->getIndex() );
                if ( edge.getIsOneWay() == "FromTo" || edge.getIsOneWay() == "From" )
                    newEdges.push_back( edge );
            } else {
                edge.setTo( secondClosestIndex );
                if ( Cartographer::isEdgeUnique( origEdges, edge ) == true ) {
                    edge.setFrom( it->getIndex() );
                    if ( edge.getIsOneWay() == "FromTo" || edge.getIsOneWay() == "From" )
                        newEdges.push_back( edge );
                }
            }
        }

        Vertex tempVertex = *it;
        tempVertex.setEdges( newEdges );
        newVertices.push_back( tempVertex );
    }

    Cartographer::roadMap.setVertices( newVertices );
}

double Cartographer::distVertex2Vertex( Vertex vertex1, Vertex vertex2 )
{
    double dist;

    dist = sqrt( ( vertex2.getPoint().x() - vertex1.getPoint().x() ) * ( vertex2.getPoint().x() - vertex1.getPoint().x() ) +
                 ( vertex2.getPoint().y() - vertex1.getPoint().y() ) * ( vertex2.getPoint().y() - vertex1.getPoint().y() ) );

    return dist;
}

bool Cartographer::isEdgeUnique( std::vector<Edge> edges, Edge edge )
{
    bool answer = true;

    for ( std::vector<Edge>::iterator it = edges.begin();
          it != edges.end(); ++it )
    {
        if ( edge.getTo() == it->getTo() && edge.getTo() != 0 )
            answer = false;
    }

    return answer;
}

void Cartographer::printGraph()
{
    std::cout << "Road Graph: " << std::endl;
    std::vector<Vertex> vertices = Cartographer::roadMap.getVertices();

    for ( std::vector<Vertex>::iterator it = vertices.begin();
          it != vertices.end(); ++it )
    {
        std::vector<Edge> edges = it->getEdges();

        std::cout << "  Vertex: " << it->getIndex() << std::endl;
        std::cout << "    Point: " << std::endl;
        std::cout << "      x: " << it->getPoint().x() << std::endl;
        std::cout << "      y: " << it->getPoint().y() <<std::endl;
        std::cout << "    SpeedLimit: " << it->getSpeedLimit() <<std::endl;
        std::cout << "    Color: " << std::endl;
        std::cout << "      Red   : " << it->getColor().red() << std::endl;
        std::cout << "      Green : " << it->getColor().green() << std::endl;
        std::cout << "      Blue  : " << it->getColor().blue() << std::endl;
        std::cout << "    Edges: " << edges.size() << std::endl;

        for ( std::vector<Edge>::iterator jt = edges.begin();
              jt != edges.end(); ++jt )
        {
            std::cout << "      Edge: " << std::endl;
            std::cout << "        Length: " << jt->getLength() << std::endl;
            std::cout << "        PixelLength: " << jt->getPixelLength() << std::endl;
            std::cout << "        SpeedLimit: " << jt->getSpeedLimit() << std::endl;
            std::cout << "        Color : " << std::endl;
            std::cout << "          Red   : " << jt->getColor().red() << std::endl;
            std::cout << "          Green : " << jt->getColor().green() << std::endl;
            std::cout << "          Blue  : " << jt->getColor().blue() << std::endl;
            std::cout << "        From  : " << jt->getFrom() << std::endl;
            std::cout << "        To    : " << jt->getTo() << std::endl;
        }
    }

    std::cout << std::endl;
}

void Cartographer::createLuaFile()
{
    QFile* graphLuaFile = new QFile( "roadMap.lua" );
    if ( !graphLuaFile->open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;
    QTextStream out( graphLuaFile );

    std::vector<Vertex> vertices = Cartographer::roadMap.getVertices();

    out << "roadMap = { " << "\n" << "\n";
    for ( std::vector<Vertex>::iterator it = vertices.begin();
          it != vertices.end(); ++it )
    {
        out << "  [" << it->getIndex() << "] = { " << "\n";
        out << "    x = " << floor( it->getPoint().x() + 0.5 ) << ",\n";
        out << "    y = " << floor( ( Cartographer::roadNetwork->getBoundary().height - it->getPoint().y() ) + 0.5 ) << ",\n";
        out << "    speedLimit = " << it->getSpeedLimit() << ",\n";
        out << "    color = { ";
        out << "red = " << it->getColor().red() << ", ";
        out << "green = " << it->getColor().green() << ", ";
        out << "blue = " << it->getColor().blue();
        out << " },\n";

        std::vector<Edge> edges = it->getEdges();
        out << "    edges = { " << "\n";
        int index = 0;
        for ( std::vector<Edge>::iterator jt = edges.begin();
              jt != edges.end(); ++jt )
        {
            ++index;
            out << "      [" << index << "] = { ";
            out << "length = " << jt->getLength() << ", ";
            out << "pixelLength = " << jt->getPixelLength() << ", ";
            out << "speedLimit = " << jt->getSpeedLimit() << ", ";
            out << "color = { ";
            out << "red = " << jt->getColor().red() << ", ";
            out << "green = " << jt->getColor().green() << ", ";
            out << "blue = " << jt->getColor().blue() << " }, ";
            out << "from = " << jt->getFrom() << ", ";
            out << "to = " << jt->getTo();
            if ( jt == --edges.end() )
                out << " }\n";
            else
                out << " },\n";
        }
        out << "    } \n";
        if ( it == --vertices.end() )
            out << "  }" << "\n" << "\n";
        else
            out << "  }," << "\n" << "\n";
    }
    out << "}";

    graphLuaFile->close();
    delete graphLuaFile;
}
