#include "Solution.hpp"

Solution::Solution(size_t nRows) {
    this->nRows = nRows;
    this->cost = 0;
    this->solucao.reserve(nRows);
}

float Solution::distanciaHorizontal(size_t id, size_t row, std::vector<size_t>& comprimentos) {
    float distancia = comprimentos.at(id) / 2;
    for (size_t i=0; i<this->solucao.at(row).size(); ++i) {
        if (this->solucao[row][i] == id) {
            return distancia;
        }
        distancia += comprimentos.at(this->solucao[row][i]);
    }
    return -1;
}

void Solution::recalcularCusto(matrix_t& pesos, std::vector<size_t>& comprimentos) {
    float cost = 0;
    for (size_t row=0; row<this->nRows; ++row) {
        for (size_t i=0; i<this->solucao.at(row).size(); i++) {
            for (size_t j=0; j<this->solucao.at(row).size(); j++) {
                cost += pesos[this->solucao[row][i]][this->solucao[row][j]] * std::abs(this->distanciaHorizontal(i, row, comprimentos) - this->distanciaHorizontal(j, row, comprimentos));
            }
        }
    }
    this->cost = cost;
}