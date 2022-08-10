#include "Solution.hpp"

Solution::Solution(size_t nRows) {
    this->nRows = nRows;
    this->cost = 0;
    this->solucao.reserve(nRows);
    for (size_t i=0; i<nRows; i++) {
        std::vector<size_t> aux;
        this->solucao.push_back(aux);
    }
}

float Solution::getCost() {
    return this->cost;
}

float Solution::distanciaHorizontal(size_t id, size_t row, std::vector<size_t>& comprimentos) {
    float distancia = ((float) comprimentos.at(id)) / 2;
    for (size_t i=0; i<this->solucao.at(row).size(); ++i) {
        if (this->solucao[row][i] == id) {
            return distancia;
        }
        distancia += (float) comprimentos.at(this->solucao[row][i]);
    }
    return 0;
}

void Solution::recalcularCusto(matrix_t& pesos, std::vector<size_t>& comprimentos) {
    std::vector<float> abscissas;
    abscissas.resize(comprimentos.size(), 0);
    for (size_t row=0; row<this->nRows; ++row) {
        for (size_t i=0; i<this->solucao.at(row).size(); i++) {
            abscissas.at(this->solucao[row][i]) = this->distanciaHorizontal(this->solucao[row][i], row, comprimentos);
        }
    }

    float cost = 0;
    
    for (size_t i=0; i<abscissas.size()-1; i++) {
        for (size_t j=i+1; j<abscissas.size(); j++) {
            cost += ((float) pesos[i][j]) * std::abs(abscissas.at(i) - abscissas.at(j));
        }
    }
    this->cost = cost;
}

void Solution::triangSup(matrix_t& pesos, std::vector<size_t>& comprimentos) {
    std::vector<matrix_t> triangularSup;
    triangularSup.reserve(this->nRows);
    for (size_t j=0; j<this->nRows; j++) {
        matrix_t aux2;
        aux2.reserve(pesos.size());
        triangularSup.push_back(aux2);
        for (size_t i=0; i<pesos.size(); i++) {
            std::vector<float> aux;
            aux.resize(pesos.size(), 0);
            triangularSup.at(j).push_back(aux);
        }

    }
    float cost = 0;
    for (size_t row=0; row<this->nRows; ++row) {
        for (size_t i=0; i<this->solucao.at(row).size(); i++) {
            
            size_t id_i = this->solucao[row][i];
            float d_i = this->distanciaHorizontal(id_i, row, comprimentos);
            
            for (size_t j=i+1; j<this->solucao.at(row).size(); j++) {
                
                size_t id_j = this->solucao[row][j];
                float d_j = this->distanciaHorizontal(id_j, row, comprimentos);
                
                cost = ((float) pesos[id_i][id_j]) * std::abs(d_i - d_j);
                triangularSup[row][id_i][id_j] = cost;
                triangularSup[row][id_j][id_i] = cost;
            }
        }
    }
    for (size_t row=0; row<this->nRows; ++row) {
        std::cout << "\nLINHA " << row + 1 << "\n";
        for (size_t i=0; i<triangularSup[row].size(); i++) {
            std::cout << "\t";
            for (size_t j=0; j<triangularSup[row][i].size(); j++) {
                if (triangularSup[row][i][j] < 10) {
                    std::cout << " " << triangularSup[row][i][j] << " ";    
                } else {
                    std::cout << triangularSup[row][i][j] << " ";
                }
            }
            std::cout << "\n";
        }
    }
}