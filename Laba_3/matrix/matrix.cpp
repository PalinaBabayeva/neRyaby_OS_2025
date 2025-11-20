#include "matrix.h"
#include <thread>
#include <vector>
#include <random>
#include <stdexcept>
#include <iostream>

MatrixMultiplier::MatrixMultiplier(int matrix_size) 
    : size(matrix_size) {
    if (size <= 0) {
        throw std::invalid_argument("Matrix size must be positive");
    }
    
    matrix_A.resize(size, std::vector<int>(size, 0));
    matrix_B.resize(size, std::vector<int>(size, 0));
    result_matrix.resize(size, std::vector<long long>(size, 0));
}

void MatrixMultiplier::generateRandomMatrices() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9);
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix_A[i][j] = distrib(gen);
            matrix_B[i][j] = distrib(gen);
        }
    }
    
    result_matrix.assign(size, std::vector<long long>(size, 0));
}

void MatrixMultiplier::multiplySingleThread() {
    result_matrix.assign(size, std::vector<long long>(size, 0));
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            long long sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += static_cast<long long>(matrix_A[i][k]) * matrix_B[k][j];
            }
            result_matrix[i][j] = sum;
        }
    }
}

std::vector<std::vector<int>> MatrixMultiplier::transposeMatrixB() const {
    std::vector<std::vector<int>> transposed(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            transposed[j][i] = matrix_B[i][j];
        }
    }
    return transposed;
}

void MatrixMultiplier::multiplyBlock(const MatrixBlock& block) {
    int end_row = std::min(block.start_row + block.block_size, size);
    int end_col = std::min(block.start_col + block.block_size, size);
    
    for (int i = block.start_row; i < end_row; ++i) {
        for (int j = block.start_col; j < end_col; ++j) {
            long long sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += static_cast<long long>(matrix_A[i][k]) * matrix_B[k][j];
            }
            result_matrix[i][j] = sum;
        }
    }
}

void MatrixMultiplier::multiplyBlockOptimized(const MatrixBlock& block, 
                                            const std::vector<std::vector<int>>& b_transposed) {
    int end_row = std::min(block.start_row + block.block_size, size);
    int end_col = std::min(block.start_col + block.block_size, size);
    
    for (int i = block.start_row; i < end_row; ++i) {
        for (int j = block.start_col; j < end_col; ++j) {
            long long sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += static_cast<long long>(matrix_A[i][k]) * b_transposed[j][k];
            }
            result_matrix[i][j] = sum;
        }
    }
}

void MatrixMultiplier::workerProcess(BufferedChannel<MatrixBlock>* task_channel) {
    auto b_transposed = transposeMatrixB();
    
    while (true) {
        auto [block, valid] = task_channel->Recv();
        
        if (!valid) {
            break;
        }
        
        multiplyBlockOptimized(block, b_transposed);
    }
}

void MatrixMultiplier::multiplyWithChannels(int num_workers, int block_size) {
    if (num_workers <= 0) {
        throw std::invalid_argument("Number of workers must be positive");
    }
    if (block_size <= 0 || block_size > size) {
        throw std::invalid_argument("Invalid block size");
    }
    
    result_matrix.assign(size, std::vector<long long>(size, 0));
    
    BufferedChannel<MatrixBlock> task_channel(num_workers * 2);
    
    std::vector<std::thread> workers;
    
    try {
        for (int i = 0; i < num_workers; ++i) {
            workers.emplace_back(&MatrixMultiplier::workerProcess, this, &task_channel);
        }
        
        for (int i = 0; i < size; i += block_size) {
            for (int j = 0; j < size; j += block_size) {
                MatrixBlock block(i, j, block_size);
                task_channel.Send(block);
            }
        }
        
    } catch (...) {
        task_channel.Close();
        for (auto& worker : workers) {
            if (worker.joinable()) worker.join();
        }
        throw;
    }
    
    task_channel.Close();
    
    for (auto& worker : workers) {
        if (worker.joinable()) worker.join();
    }
}

const std::vector<std::vector<long long>>& MatrixMultiplier::getResult() const {
    return result_matrix;
}

const std::vector<std::vector<int>>& MatrixMultiplier::getMatrixA() const {
    return matrix_A;
}

const std::vector<std::vector<int>>& MatrixMultiplier::getMatrixB() const {
    return matrix_B;
}

int MatrixMultiplier::getSize() const {
    return size;
}

bool MatrixMultiplier::validateResults(const MatrixMultiplier& m1, const MatrixMultiplier& m2) {
    if (m1.size != m2.size) {
        return false;
    }
    
    auto& r1 = m1.getResult();
    auto& r2 = m2.getResult();
    
    for (int i = 0; i < m1.size; ++i) {
        for (int j = 0; j < m1.size; ++j) {
            if (r1[i][j] != r2[i][j]) {
                std::cout << "Mismatch at [" << i << "][" << j << "]: " 
                          << r1[i][j] << " != " << r2[i][j] << std::endl;
                return false;
            }
        }
    }
    return true;
}