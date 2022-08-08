#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "utils.hpp"
#include "Solution.hpp"
#include <vector>
#include <string>

using matrix_t = std::vector<std::vector<float>>;

class Graph
{
private:
    std::string instanceName;
    std::vector<size_t> vertices; // vetor de facilidades
    matrix_t pesos; // matriz de pesos entre facilidades
    std::vector<size_t> comprimentos; // vetor de comprimentos das facilidades

    matrix_t feromonios;
    std::vector<size_t> influencia;

    void initializeInfluencia();

    bool presenteNaCol(size_t id, size_t col, Solution& s);
    float getProbabilidade(size_t id_i, size_t coluna, float influencia);
    float evaporacao(size_t id_i, size_t coluna);
    float influenciaSobreSol(size_t id, size_t col, std::vector<Solution&>& s);
    void atualizaFeromonios(std::vector<Solution&>& s);

public:

    Graph(std::string instanceName, size_t nVertices);
    ~Graph();

    Graph readFile(std::istream& file, std::string instanceName);
    void setComprimento(size_t id, size_t comprimento);
    void setPesos(size_t id_i, size_t id_j, size_t peso);

    Solution aco();
};



#endif // GRAPH_HPP