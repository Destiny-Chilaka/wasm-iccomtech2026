#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  1024
#define HEIGHT 1024

unsigned char input[HEIGHT][WIDTH][3];
unsigned char output[HEIGHT][WIDTH][3];

// 3x3 Gaussian blur kernel
void gaussian_blur() {
    int kernel[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
    int ksum = 16;
    for (int y = 1; y < HEIGHT-1; y++) {
        for (int x = 1; x < WIDTH-1; x++) {
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                for (int ky = -1; ky <= 1; ky++)
                    for (int kx = -1; kx <= 1; kx++)
                        sum += input[y+ky][x+kx][c] * kernel[ky+1][kx+1];
                output[y][x][c] = (unsigned char)(sum / ksum);
            }
        }
    }
}

int main() {
    // Fill with synthetic image data
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++) {
            input[y][x][0] = (unsigned char)((x + y) % 256);
            input[y][x][1] = (unsigned char)((x * 2) % 256);
            input[y][x][2] = (unsigned char)((y * 3) % 256);
        }

    int RUNS = 20;
    struct timespec start, end;
    volatile int sink = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int r = 0; r < RUNS; r++) {
        gaussian_blur();
        sink += output[512][512][0];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ms = ((end.tv_sec - start.tv_sec)*1e9 + (end.tv_nsec - start.tv_nsec)) / 1e6 / RUNS;
    printf("Image Filter mean: %.2f ms (sink: %d)\n", ms, sink);
    return 0;
}
