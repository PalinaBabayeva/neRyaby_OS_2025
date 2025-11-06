#ifndef MATRIX_H
#define MATRIX_H

const int N = 100;

extern int A[N][N];
extern int B[N][N];
extern long long C[N][N];

struct BlockParam {
    int row_start;
    int col_start;
    int size;
};

void generateRandomMatrices();
void singleThreadMultiplication();
void multiThreadMultiplication(int k);

#endif