#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <cmath>

using matrix_t = std::vector<std::vector<size_t>>;

class Solution
{
private:
    
    float cost;
    size_t nRows;

public:

    Solution(size_t nRows);
    ~Solution();

    float getCost();
    matrix_t solucao;
    float distanciaHorizontal(size_t id, size_t row, std::vector<size_t>& comprimentos);
    void recalcularCusto(matrix_t& pesos, std::vector<size_t>& comprimentos);

};

#endif // SOLUTION_HPP