#include "Graph.hpp"

Graph::Graph(std::string instanceName, size_t nVertices) {
    this->instanceName = instanceName;

    this->vertices.resize(nVertices);

    this->pesos.reserve(nVertices);
    for (size_t i=0; i<nVertices; i++) {
        this->pesos.at(i).resize(nVertices);
    }

    this->comprimentos.resize(nVertices);
}

Graph Graph::readFile(std::istream& file, std::string instanceName)
{
    std::string line;
    size_t nVertices;

    getline(file, line);
    nVertices = stol(line);

    Graph g(this->instanceName, nVertices);

    std::vector<std::string> lb;

    getline(file, line);
    lb = split(line, ' ');

    for (size_t i=0; i<nVertices; ++i) {
        g.setComprimento(i, stol(lb.at(i)));
    }
    
    getline(file, line);
    
    size_t i=0;
    while (getline(file, line)) {
        lb = split(line, ' ');
        
        for (size_t j=0; j<nVertices; j++) {
            g.setPesos(i, j, stol(lb.at(j)));
        }

        i++;
    }
    return g;
}

void Graph::setComprimento(size_t id, size_t comprimento) {
    this->comprimentos.at(id) = comprimento;
}

void Graph::setPesos(size_t id_i, size_t id_j, size_t peso) {
    this->pesos[id_i][id_j] = peso;
}

