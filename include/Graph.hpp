#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "utils.hpp"
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

    Graph(std::string instanceName, size_t nVertices);
    ~Graph();

    Graph readFile(std::istream& file, std::string instanceName);
    void setComprimento(size_t id, size_t comprimento);
    void setPesos(size_t id_i, size_t id_j, size_t peso);
};



#endif // GRAPH_HPP