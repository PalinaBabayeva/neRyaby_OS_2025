#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class MatrixMultiplier {
public:
    MatrixMultiplier(int matrix_size = 100);
    
    void generateRandomMatrices();
    
    void multiplySingleThread();
    
    void multiplyMultiThread(int block_size);
    
    const std::vector<std::vector<long long>>& getResult() const;
    
    int getSize() const;

private:
    int size;
    std::vector<std::vector<int>> matrix_A;    // Исходная матрица A
    std::vector<std::vector<int>> matrix_B;    // Исходная матрица B  
    std::vector<std::vector<long long>> result_matrix; // Результирующая матрица C
    
    void multiplyBlock(int start_row, int start_col, int block_size);
};

#endif