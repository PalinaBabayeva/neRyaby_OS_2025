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
    const int MATRIX_SIZE = 100;
    std::cout << "Тестирование умножения матриц " << MATRIX_SIZE << "x" << MATRIX_SIZE << " (pthread)" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    MatrixMultiplier multiplier(MATRIX_SIZE);
    
    std::cout << "Генерация случайных матриц..." << std::endl;
    multiplier.generateRandomMatrices();
    
    std::cout << "\n--- ОДНОПОТОЧНОЕ УМНОЖЕНИЕ ---" << std::endl;
    double single_thread_time = measureExecutionTime([&]() {
        multiplier.multiplySingleThread();
    });
    std::cout << "Время выполнения: " << single_thread_time << " мс" << std::endl;
    
    std::cout << "\n--- МНОГОПОТОЧНОЕ УМНОЖЕНИЕ (pthread) ---" << std::endl;
    
    std::vector<int> block_sizes = {1, 2, 5, 10, 20, 25, 50, MATRIX_SIZE};
    
    for (int block_size : block_sizes) {
        int blocks_per_dimension = (MATRIX_SIZE + block_size - 1) / block_size;
        int total_threads = blocks_per_dimension * blocks_per_dimension;
        
        double multi_thread_time = measureExecutionTime([&]() {
            multiplier.multiplyMultiThread(block_size);
        });
        
        std::cout << "Размер блока: " << block_size 
                  << " | Потоков: " << total_threads 
                  << " | Время: " << multi_thread_time << " мс"
                  << " | Ускорение: " << (single_thread_time / multi_thread_time) << "x"
                  << std::endl;
    }
    
    std::cout << "\n==========================================" << std::endl;
    std::cout << "Тестирование завершено" << std::endl;
    
    return 0;
}