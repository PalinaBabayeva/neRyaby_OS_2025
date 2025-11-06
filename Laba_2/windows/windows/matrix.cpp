#include "matrix.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

int A[N][N];
int B[N][N];
long long C[N][N];

static DWORD WINAPI ThreadMultiply(LPVOID lpParam) {
    BlockParam* param = (BlockParam*)lpParam;
    int r0 = param->row_start;
    int c0 = param->col_start;
    int blockSize = param->size;
    int rEnd = (r0 + blockSize < N) ? r0 + blockSize : N;
    int cEnd = (c0 + blockSize < N) ? c0 + blockSize : N;

    for (int i = r0; i < rEnd; ++i) {
        for (int j = c0; j < cEnd; ++j) {
            C[i][j] = 0;
            for (int p = 0; p < N; ++p) {
                C[i][j] += (long long)A[i][p] * B[p][j];
            }
        }
    }
    return 0;
}

void generateRandomMatrices() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
}

void singleThreadMultiplication() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            long long sum = 0;
            for (int p = 0; p < N; ++p) {
                sum += (long long)A[i][p] * B[p][j];
            }
            C[i][j] = sum;
        }
    }
}

void multiThreadMultiplication(int k) {
    int blocks_per_dim = (N + k - 1) / k;
    int total_threads = blocks_per_dim * blocks_per_dim;

    std::vector<HANDLE> threads;
    threads.reserve(total_threads);
    std::vector<BlockParam> params;
    params.reserve(total_threads);

    for (int i = 0; i < N; i += k) {
        for (int j = 0; j < N; j += k) {
            BlockParam param = { i, j, k };
            params.push_back(param);

            HANDLE hThread = CreateThread(
                NULL,
                0,
                ThreadMultiply,
                &params.back(),
                0,
                NULL
            );

            if (hThread == NULL) {
                std::cerr << "Ошибка CreateThread, код: " << GetLastError() << std::endl;
            }
            else {
                threads.push_back(hThread);
            }
        }
    }

    for (HANDLE hThread : threads) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}