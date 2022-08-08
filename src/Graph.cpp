#include "Graph.hpp"

Graph::Graph(std::string instanceName, size_t nVertices) {
    this->instanceName = instanceName;

    this->vertices.resize(nVertices);

    this->pesos.reserve(nVertices);
    this->feromonios.reserve(nVertices);
    this->influencia.reserve(nVertices);
    for (size_t i=0; i<nVertices; i++) {
        this->pesos.at(i).resize(nVertices);
        this->feromonios.at(i).resize(nVertices, 1);
    }

    this->comprimentos.resize(nVertices);
}

void Graph::initializeInfluencia() {
    for (size_t i=0; i<this->vertices.size(); ++i) {
        size_t w = 0;
        for (size_t j=0; i<this->vertices.size(); ++j) {
            w += this->pesos[i][j];
        }
        this->influencia.at(i) = w;
    }
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

float Graph::getProbabilidade(size_t id_i, size_t coluna, float influencia) {
    float denominador = 0;
    for (size_t i=0; i<this->vertices.size(); i++) {
        denominador += influencia * this->feromonios[id_i][i];
    }
    return influencia * this->feromonios[id_i][coluna] / denominador;
}

float Graph::evaporacao(size_t id_i, size_t coluna) {
    float rho = 0.05;
    return (1-rho)*this->feromonios[id_i][coluna];
}

bool Graph::presenteNaCol(size_t id, size_t col, Solution& s) {
    for (size_t i=0; i<s.solucao.size(); ++i) {
        if (s.solucao[i][col] == id) {
            return true;
        }
    }
    return false;
}

float Graph::influenciaSobreSol(size_t id, size_t col, std::vector<Solution&>& s) {
    float influencia = 0;
    for (size_t i=0; i<s.size(); i++) {
        influencia += this->presenteNaCol(id, col, s.at(i)) ? 1/s.at(i).getCost() : 0;
    }
    return influencia;
}

void Graph::atualizaFeromonios(std::vector<Solution&>& s) {
    for (size_t id=0; id<this->vertices.size(); id++) {
        for (size_t col=0; col<this->vertices.size(); col++) {
            this->feromonios[id][col] = this->evaporacao(id, col) + this->influenciaSobreSol(id, col, s);
        }
    }
}

Solution Graph::aco() {
    size_t nRows = 2;
    size_t nAnts = this->vertices.size();
    std::vector<Solution*> s;
    s.reserve(nAnts);
    for (size_t i=0; i<nAnts; i++) {
        s.at(i) = new Solution(nRows);
    }
    this->initializeInfluencia();

    size_t it = 0;
    size_t nIt = 1;
    while (it < nIt) {

        for (size_t ant=0; ant<nAnts; ant++) {

            std::vector<size_t> facilidades = this->vertices;
            std::vector<size_t> colunas;
            colunas.resize(nRows, 0);

            while (!facilidades.empty()) {

            }

        }

        it++;
    }

}