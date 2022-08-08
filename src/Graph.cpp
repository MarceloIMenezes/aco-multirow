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

float Graph::influenciaSobreSol(size_t id, size_t col, std::vector<Solution>& s) {
    float influencia = 0;
    for (size_t i=0; i<s.size(); i++) {
        influencia += this->presenteNaCol(id, col, s.at(i)) ? 1/s.at(i).getCost() : 0;
    }
    return influencia;
}

void Graph::atualizaFeromonios(std::vector<Solution>& s) {
    for (size_t id=0; id<this->vertices.size(); id++) {
        for (size_t col=0; col<this->vertices.size(); col++) {
            this->feromonios[id][col] = this->evaporacao(id, col) + this->influenciaSobreSol(id, col, s);
        }
    }
}

size_t Graph::getFacilidade(std::vector<prob_fac>& prob) {
    double rng = (double) (rand() % 1000) / 1000;
    float aux = 0;
    float prob_sum = 0;
    for (size_t i=0; i<prob.size(); i++) {
        prob_sum += prob.at(i).prob;
    }

    for (size_t i=0; i<prob.size()-1; i++) {
        aux += prob.at(i).prob / prob_sum;
        if (rng <= aux) {
            return i;
        }
    }
    return prob.size()-1;
}

Solution Graph::aco() {
    size_t nRows = 2;
    size_t nAnts = this->vertices.size();
    std::vector<Solution> s;
    Solution best = Solution(nRows);
    s.reserve(nAnts);
    this->initializeInfluencia();

    size_t it = 0;
    size_t nIt = 200;
    while (it < nIt) {

        for (size_t ant=0; ant<nAnts; ant++) {

            s.at(ant) = Solution(nRows);

            std::vector<size_t> facilidades = this->vertices;
            std::vector<size_t> colunas;
            colunas.resize(nRows, 0);

            std::vector<prob_fac> prob;
            prob.resize(nRows, prob_fac{-1,0});

            while (!facilidades.empty()) {
                
                for (size_t linha = 0; linha < nRows; linha++) {
                    for (size_t id_f = 0; id_f < this->vertices.size(); id_f++) {
                        float influencia = colunas.at(linha) == 0 ? 1/this->influencia.at(id_f) : 1/(this->influencia.at(id_f) + this->pesos[id_f][s.at(ant).solucao[linha][colunas.at(linha)-1]]);
                        size_t p = this->getProbabilidade(id_f, colunas.at(linha), influencia);
                        if (p > prob.at(linha).prob) {
                            prob.at(linha).id = id_f;
                            prob.at(linha).prob = p;
                        }
                    }
                }

                size_t linha_insert = this->getFacilidade(prob);
                s.at(ant).solucao[linha_insert][colunas.at(linha_insert)] = prob.at(linha_insert).id;
                facilidades.erase(facilidades.begin() + prob.at(linha_insert).id);

            }
            s.at(ant).recalcularCusto(this->pesos, this->comprimentos);
            if (best.getCost() == 0 || best.getCost() > s.at(ant).getCost()) {
                best = s.at(ant);
            }
        }
        this->atualizaFeromonios(s);

        it++;
    }

    return best;
}