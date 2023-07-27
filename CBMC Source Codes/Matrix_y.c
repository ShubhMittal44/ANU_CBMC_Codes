#include <yices.h>

#define SIZE 3

void matrix_multiply(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int A[SIZE][SIZE] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int B[SIZE][SIZE] = {
        {10, 11, 12},
        {13, 14, 15},
        {16, 17, 18}
    };

    int C[SIZE][SIZE];

    matrix_multiply(A, B, C);

    // Assert correctness properties
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            assert(C[i][j] == A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j]);
        }
    }

    return 0;
}
