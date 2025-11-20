#!/bin/bash

echo "Компиляция программы..."
g++ -std=c++17 -pthread testMatrix.cpp matrix.cpp -o matrix_test

if [ $? -eq 0 ]; then
    echo "Программа успешно собрана"
    echo "Запуск тестирования..."
    echo "=========================================="
    ./matrix_test
else
    echo "Ошибка компиляции"
    exit 1
fi