#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

using matrix_t = std::vector<std::vector<size_t>>;

class Graph
{
private:
    std::string instanceName;
    std::vector<size_t> vertices; // vetor de facilidades
    matrix_t pesos; // matriz de pesos entre facilidades
    std::vector<size_t> comprimentos; // vetor de comprimentos das facilidades

public:

    Graph(size_t nVertices);
    ~Graph();

};



#endif // GRAPH_HPP