
#!/bin/bash

g++ -std=c++11 -pthread testMatrix.cpp matrix.cpp -o matrix_test
if [ $? -eq 0 ]; then
    echo "Программа собрана"
    ./matrix_test
else
    echo "Ошибка компиляции"
    exit 1
fi