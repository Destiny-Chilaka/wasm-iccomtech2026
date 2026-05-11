#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 256

float A[N][N], B[N][N], C[N][N];

void matrix_multiply() {
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++)
            for (int j = 0; j < N; j++)
                C[i][j] += A[i][k] * B[k][j];
}

int main() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = (float)(i + j) / N;
            B[i][j] = (float)(i * j) / N;
        }

    int RUNS = 100;
    struct timespec start, end;
    volatile float sink = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int r = 0; r < RUNS; r++) {
        matrix_multiply();
        sink += C[0][0];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ms = ((end.tv_sec - start.tv_sec)*1e9 + (end.tv_nsec - start.tv_nsec)) / 1e6 / RUNS;
    printf("Matrix Multiply mean: %.2f ms (sink: %.4f)\n", ms, sink);
    return 0;
}
