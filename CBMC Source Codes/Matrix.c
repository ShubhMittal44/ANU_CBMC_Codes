#include <assert.h>

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
    int A[SIZE][SIZE] = { {1, 2, 1}, {4, 5, 6}, {7, 8, 9} };
    int B[SIZE][SIZE] = { {9, 8, 7}, {6, 5, 4}, {3, 2, 1} };
    int C[SIZE][SIZE];

    matrix_multiply(A, B, C);

    // Verify correctness properties
    assert(C[0][0] == 30);
    assert(C[0][1] == 24);
    assert(C[0][2] == 18);
    assert(C[1][0] == 84);
    assert(C[1][1] == 69);
    assert(C[1][2] == 54);
    assert(C[2][0] == 138);
    assert(C[2][1] == 114);
    assert(C[2][2] == 90);

    return 0;
}
