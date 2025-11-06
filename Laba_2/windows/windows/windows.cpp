#include "matrix.h"
#include <iostream>
#include <chrono>
#include <vector>

template<typename Func>
double measureExecutionTime(Func&& func) {
    auto start_time = std::chrono::high_resolution_clock::now();
    func();
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}

int main() {
    const int MATRIX_SIZE = N;

    std::cout << "Testing matrix multiplication " << MATRIX_SIZE << "x" << MATRIX_SIZE << std::endl;
    std::cout << "==========================================" << std::endl;

    std::cout << "Generating random matrices..." << std::endl;
    generateRandomMatrices();

    std::cout << "\n--- SINGLE THREAD MULTIPLICATION ---" << std::endl;
    double single_thread_time = measureExecutionTime([]() {
        singleThreadMultiplication();
        });
    std::cout << "Execution time: " << single_thread_time << " ms" << std::endl;

    std::cout << "\n--- MULTI-THREAD MULTIPLICATION (WinAPI) ---" << std::endl;

    std::vector<int> block_sizes = { 1, 2, 5, 10, 20, 50, N };

    for (int block_size : block_sizes) {
        int blocks_per_dimension = (MATRIX_SIZE + block_size - 1) / block_size;
        int total_threads = blocks_per_dimension * blocks_per_dimension;

        double multi_thread_time = measureExecutionTime([&]() {
            multiThreadMultiplication(block_size);
            });

        std::cout << "Block size: " << block_size
            << " | Threads: " << total_threads
            << " | Time: " << multi_thread_time << " ms"
            << " | Speedup: " << (single_thread_time / multi_thread_time) << "x"
            << std::endl;
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "Testing completed" << std::endl;

    return 0;
}