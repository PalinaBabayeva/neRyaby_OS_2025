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
    const std::vector<std::vector<int>>& getMatrixA() const;
    const std::vector<std::vector<int>>& getMatrixB() const;

private:
    int size;
    std::vector<std::vector<int>> matrix_A;
    std::vector<std::vector<int>> matrix_B;  
    std::vector<std::vector<long long>> result_matrix;
    
    void multiplyBlock(int start_row, int start_col, int block_size);
};

extern "C" {
    MatrixMultiplier* create_matrix_multiplier(int size);
    void delete_matrix_multiplier(MatrixMultiplier* obj);
    
    void generate_matrices(MatrixMultiplier* obj);
    void multiply_single_thread(MatrixMultiplier* obj);
    void multiply_multi_thread(MatrixMultiplier* obj, int block_size);
    
    long long get_result_value(MatrixMultiplier* obj, int i, int j);
    int get_matrix_size(MatrixMultiplier* obj);
}

#endif