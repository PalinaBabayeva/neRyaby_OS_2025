#include "matrix.h"
#include <thread>
#include <vector>
#include <random>

MatrixMultiplier::MatrixMultiplier(int matrix_size) 
    : size(matrix_size) {
    // Инициализация матриц с заданным размером
    matrix_A.resize(size, std::vector<int>(size, 0));
    matrix_B.resize(size, std::vector<int>(size, 0));
    result_matrix.resize(size, std::vector<long long>(size, 0));
}

void MatrixMultiplier::generateRandomMatrices() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9); // Числа от 0 до 9
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix_A[i][j] = distrib(gen);
            matrix_B[i][j] = distrib(gen);
        }
    }
}

void MatrixMultiplier::multiplySingleThread() {
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

void MatrixMultiplier::multiplyBlock(int start_row, int start_col, int block_size) {
    int end_row = (start_row + block_size < size) ? start_row + block_size : size;
    int end_col = (start_col + block_size < size) ? start_col + block_size : size;
    
    for (int i = start_row; i < end_row; ++i) {
        for (int j = start_col; j < end_col; ++j) {
            long long sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += static_cast<long long>(matrix_A[i][k]) * matrix_B[k][j];
            }
            result_matrix[i][j] = sum;
        }
    }
}

void MatrixMultiplier::multiplyMultiThread(int block_size) {
    std::vector<std::thread> threads;
    
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            threads.emplace_back(&MatrixMultiplier::multiplyBlock, this, 
                               i, j, block_size);
        }
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

const std::vector<std::vector<long long>>& MatrixMultiplier::getResult() const {
    return result_matrix;
}

int MatrixMultiplier::getSize() const {
    return size;
}