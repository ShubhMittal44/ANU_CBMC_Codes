#include <stdio.h>
#include <stdlib.h>

#define size_t unsigned long

double**
TwoDim(int nrow, int ncol) {
    double** m;
    int i;

    m = (double**)malloc((size_t)(nrow * sizeof(double*)));
    m[0] = (double*)malloc((size_t)((nrow * ncol) * sizeof(double)));

    for (i = 1; i < nrow; i++)
        m[i] = m[i - 1] + ncol;

    return m;
}

void freeTwoDim(double** m, int nrow) {
    free(m[0]);
    free(m);
}

int main() {
    int nrow = 3;
    int ncol = 4;
    double** matrix = TwoDim(nrow, ncol);

    // Assertion
    if (matrix != NULL) {
        printf("Matrix allocation successful\n");
    }
    else {
        printf("Matrix allocation failed\n");
        return 1;
    }

    // Assertion
    int i;
    for (i = 0; i < nrow; i++) {
        if (matrix[i] != matrix[0] + (i * ncol)) {
            printf("Row alignment check failed\n");
            freeTwoDim(matrix, nrow);
            return 1;
        }
    }

    printf("Matrix creation and alignment successful\n");

    // Clean up memory
    freeTwoDim(matrix, nrow);

    return 0;
}
