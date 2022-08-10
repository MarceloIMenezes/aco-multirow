#include "Graph.hpp"

Graph::Graph(std::string instanceName, size_t nVertices) {
    this->instanceName = instanceName;

    this->vertices.resize(nVertices);

    this->pesos.reserve(nVertices);
    this->feromonios.reserve(nVertices);
    this->influencia.resize(nVertices, 1);
    for (size_t i=0; i<nVertices; i++) {
        std::vector<float> aux;
        aux.resize(nVertices);
        this->pesos.push_back(aux);
        aux.clear();
        aux.resize(nVertices, 1);
        this->feromonios.push_back(aux);
    }

    this->comprimentos.resize(nVertices);
}

void Graph::initializeInfluencia() {
    for (size_t i=0; i<this->vertices.size(); ++i) {
        size_t w = 0;
        for (size_t j=0; j<this->vertices.size(); ++j) {
            w += this->pesos[i][j];
        }
        this->influencia.at(i) = w;
    }
}

void Graph::setVertice(size_t id, size_t new_id) {
    this->vertices.at(id) = new_id;
}

Graph Graph::readFile(std::istream& file, std::string instanceName)
{
    std::string line;
    size_t nVertices;

    getline(file, line);
    nVertices = stol(line);

    Graph g(instanceName, nVertices);

    std::vector<std::string> lb;

    getline(file, line);
    lb = split(line, ' ');

    for (size_t i=0; i<nVertices; ++i) {
        g.setVertice(i, i);
        g.setComprimento(i, stol(lb.at(i)));
    }
    
    getline(file, line);
    
    size_t i=0;
    while (getline(file, line)) {
        lb = split(line, ' ');
        
        for (size_t j=i; j<nVertices; j++) {
            g.setPesos(i, j, stol(lb.at(j)));
            g.setPesos(j, i, stol(lb.at(j)));
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
        if (s.solucao.at(i).size() <= col) {
            break;
        }
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

    for (size_t i=0; i<nAnts; i++) {
        s.push_back(Solution(nRows));
    }
    this->initializeInfluencia();

    size_t it = 0;
    size_t nIt = 20;
    while (it < nIt) {
        //std::cout << "\nit: " << it;
        for (size_t ant=0; ant<nAnts; ant++) {
            //std::cout << "\n\tant: " << ant;
            s.at(ant) = Solution(nRows);

            std::vector<size_t> facilidades = this->vertices;
            std::vector<size_t> colunas;
            colunas.resize(nRows, 0);

            std::vector<prob_fac> prob;

            while (!facilidades.empty()) {
                prob.clear();
                prob.resize(nRows, prob_fac{-1,0});
                
                for (size_t linha = 0; linha < nRows; linha++) {
                    for (size_t id_f = 0; id_f < facilidades.size(); id_f++) {
                        float influencia_aux;
                        if (colunas.at(linha) == 0) {
                            influencia_aux = (float) 1/ ((float) this->influencia.at(facilidades.at(id_f)));
                        } else {
                            influencia_aux = 1/(this->influencia.at(facilidades.at(id_f)) + this->pesos[facilidades.at(id_f)][s.at(ant).solucao[linha][colunas.at(linha)-1]]);
                        }
                        float p = this->getProbabilidade(facilidades.at(id_f), colunas.at(linha), influencia_aux);
                        if (p > prob.at(linha).prob) {
                            prob.at(linha).id = facilidades.at(id_f);
                            prob.at(linha).prob = p;
                        }
                    }
                }

                size_t linha_insert = this->getFacilidade(prob);
                s.at(ant).solucao[linha_insert].push_back(prob.at(linha_insert).id);
                colunas.at(linha_insert) += 1;
                for (size_t iter = 0; iter<facilidades.size(); ++iter) {
                    if (facilidades.at(iter) == prob.at(linha_insert).id) {
                        facilidades.erase(facilidades.begin() + iter);
                        break;
                    }
                }

            }
            s.at(ant).recalcularCusto(this->pesos, this->comprimentos);
            if (best.getCost() == 0 || best.getCost() > s.at(ant).getCost()) {
                best = s.at(ant);
            }
        }
        this->atualizaFeromonios(s);

        it++;
    }
    //best.triangSup(this->pesos, this->comprimentos);
    return best;
}

void Graph::outFile(std::ostream& file, Solution& s, long seed) {
    file << "INSTÂNCIA: " << this->instanceName << "\n";
    file << "SEED: " << seed << "\n";
    file << "\nCUSTO: " << s.getCost() << "\n";
    for (size_t row=0; row < s.solucao.size(); row++) {
        file << "Coluna " << row + 1 << ": ";
        for (size_t col=0; col<s.solucao.at(row).size(); col++) {
            file << s.solucao[row][col] << " ";
        }
        file << "\n";
    }
}