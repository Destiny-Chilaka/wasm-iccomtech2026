#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100000

int original[SIZE];
int arr[SIZE];

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    srand(42);
    for (int i = 0; i < SIZE; i++)
        original[i] = rand() % 1000000;

    int RUNS = 100;
    struct timespec start, end;
    volatile int sink = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int r = 0; r < RUNS; r++) {
        memcpy(arr, original, SIZE * sizeof(int));
        qsort(arr, SIZE, sizeof(int), cmp);
        sink += arr[0];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ms = ((end.tv_sec - start.tv_sec)*1e9 + (end.tv_nsec - start.tv_nsec)) / 1e6 / RUNS;
    printf("Integer Sort mean: %.2f ms (sink: %d)\n", ms, sink);
    return 0;
}
