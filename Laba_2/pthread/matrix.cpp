#include "matrix.h"
#include <pthread.h>
#include <vector>
#include <random>

struct BlockParam {
    int row_start;
    int col_start;
    int size;
    std::vector<std::vector<int>>* matrix_A;
    std::vector<std::vector<int>>* matrix_B;
    std::vector<std::vector<long long>>* result_matrix;
};

void* threadMultiply(void* arg) {
    BlockParam* param = (BlockParam*) arg;
    int r0 = param->row_start;
    int c0 = param->col_start;
    int blockSize = param->size;
    int N = param->matrix_A->size();
    
    int rEnd = (r0 + blockSize < N) ? r0 + blockSize : N;
    int cEnd = (c0 + blockSize < N) ? c0 + blockSize : N;
    
    for (int i = r0; i < rEnd; ++i) {
        for (int j = c0; j < cEnd; ++j) {
            long long sum = 0;
            for (int p = 0; p < N; ++p) {
                sum += (long long)(*param->matrix_A)[i][p] * (*param->matrix_B)[p][j];
            }
            (*param->result_matrix)[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}

MatrixMultiplier::MatrixMultiplier(int matrix_size) 
    : size(matrix_size) {
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

void MatrixMultiplier::multiplyMultiThread(int block_size) {
    int blocks_per_dim = (size + block_size - 1) / block_size;
    int total_threads = blocks_per_dim * blocks_per_dim;
    
    std::vector<pthread_t> threads(total_threads);
    std::vector<BlockParam> params(total_threads);
    
    int t = 0;
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            params[t] = {i, j, block_size, &matrix_A, &matrix_B, &result_matrix};
            pthread_create(&threads[t], NULL, threadMultiply, &params[t]);
            ++t;
        }
    }
    
    for (int t = 0; t < total_threads; ++t) {
        pthread_join(threads[t], NULL);
    }
}

const std::vector<std::vector<long long>>& MatrixMultiplier::getResult() const {
    return result_matrix;
}

int MatrixMultiplier::getSize() const {
    return size;
}

const std::vector<std::vector<int>>& MatrixMultiplier::getMatrixA() const {
    return matrix_A;
}

const std::vector<std::vector<int>>& MatrixMultiplier::getMatrixB() const {
    return matrix_B;
}

extern "C" {
    MatrixMultiplier* create_matrix_multiplier(int size) {
        return new MatrixMultiplier(size);
    }
    
    void delete_matrix_multiplier(MatrixMultiplier* obj) {
        delete obj;
    }
    
    void generate_matrices(MatrixMultiplier* obj) {
        obj->generateRandomMatrices();
    }
    
    void multiply_single_thread(MatrixMultiplier* obj) {
        obj->multiplySingleThread();
    }
    
    void multiply_multi_thread(MatrixMultiplier* obj, int block_size) {
        obj->multiplyMultiThread(block_size);
    }
    
    long long get_result_value(MatrixMultiplier* obj, int i, int j) {
        return obj->getResult()[i][j];
    }
    
    int get_matrix_size(MatrixMultiplier* obj) {
        return obj->getSize();
    }
}