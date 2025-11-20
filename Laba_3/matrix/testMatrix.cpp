#include "matrix.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>

template<typename Func>
double measureExecutionTime(Func&& func) {
    auto start_time = std::chrono::high_resolution_clock::now();
    func();
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}

void printMatrixInfo(const MatrixMultiplier& multiplier, const std::string& name) {
    std::cout << name << " matrix:" << std::endl;
    auto& result = multiplier.getResult();
    
    int show_size = std::min(4, multiplier.getSize());
    
    for (int i = 0; i < show_size; ++i) {
        for (int j = 0; j < show_size; ++j) {
            std::cout << std::setw(8) << result[i][j] << " ";
        }
        if (show_size < multiplier.getSize()) {
            std::cout << "...";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    const int MATRIX_SIZE = 100;
    
    std::cout << "==========================================" << std::endl;
    std::cout << "Testing matrix multiplication " << MATRIX_SIZE << "x" << MATRIX_SIZE << std::endl;
    std::cout << "Using channels" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    try {
        MatrixMultiplier single_thread(MATRIX_SIZE);
        single_thread.generateRandomMatrices();
        
        std::cout << "\n--- SINGLE-THREADED MULTIPLICATION ---" << std::endl;
        double single_thread_time = measureExecutionTime([&]() {
            single_thread.multiplySingleThread();
        });
        std::cout << "Execution time: " << single_thread_time << " ms" << std::endl;
        
        std::cout << "\n--- CHANNEL-BASED MULTIPLICATION ---" << std::endl;
        
        std::vector<int> worker_counts = {2, 4, 8};
        std::vector<int> block_sizes = {10, 25, 50};
        
        bool validation_passed = true;
        
        for (int workers : worker_counts) {
            for (int block_size : block_sizes) {
                MatrixMultiplier channel_multiplier(MATRIX_SIZE);
                
                channel_multiplier = single_thread;
                
                double channel_time = measureExecutionTime([&]() {
                    channel_multiplier.multiplyWithChannels(workers, block_size);
                });
                
                bool valid = MatrixMultiplier::validateResults(single_thread, channel_multiplier);
                validation_passed &= valid;
                
                std::cout << "Workers: " << std::setw(2) << workers 
                          << " | Block: " << std::setw(3) << block_size 
                          << " | Time: " << std::setw(8) << std::fixed 
                          << std::setprecision(2) << channel_time << " ms"
                          << " | Speedup: " << std::setw(6) 
                          << std::setprecision(2) << (single_thread_time / channel_time) << "x"
                          << " | Correct: " << (valid ? "+" : "-")
                          << std::endl;
            }
            std::cout << "---" << std::endl;
        }
        
        std::cout << "\n--- VALIDATION RESULTS ---" << std::endl;
        if (validation_passed) {
            std::cout << "All results are correct!" << std::endl;
        } else {
            std::cout << "Discrepancies found in results!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n==========================================" << std::endl;
    std::cout << "Testing completed" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    return 0;
}