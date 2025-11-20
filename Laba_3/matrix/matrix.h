#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "buffered_channel.h"

struct MatrixBlock {
    int start_row;
    int start_col;
    int block_size;
    
    MatrixBlock(int row = 0, int col = 0, int size = 0) 
        : start_row(row), start_col(col), block_size(size) {}
};

class MatrixMultiplier {
public:
    explicit MatrixMultiplier(int matrix_size = 100);
    
    void generateRandomMatrices();
    void multiplySingleThread();
    void multiplyWithChannels(int num_workers, int block_size);
    
    const std::vector<std::vector<long long>>& getResult() const;
    const std::vector<std::vector<int>>& getMatrixA() const;
    const std::vector<std::vector<int>>& getMatrixB() const;
    int getSize() const;

    static bool validateResults(const MatrixMultiplier& m1, const MatrixMultiplier& m2);

private:
    int size;
    std::vector<std::vector<int>> matrix_A;
    std::vector<std::vector<int>> matrix_B;  
    std::vector<std::vector<long long>> result_matrix;
    
    void workerProcess(BufferedChannel<MatrixBlock>* task_channel);
    void multiplyBlock(const MatrixBlock& block);
    
    void multiplyBlockOptimized(const MatrixBlock& block, 
                               const std::vector<std::vector<int>>& b_transposed);
    
    std::vector<std::vector<int>> transposeMatrixB() const;
};

#endif // MATRIX_H