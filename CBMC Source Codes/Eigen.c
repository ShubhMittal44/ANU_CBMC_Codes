#include <stdio.h>
#include <math.h>

#define SIZE 2

// Function to calculate the dot product of two vectors
double dotProduct(double v1[], double v2[]) {
    double result = 0;
    for (int i = 0; i < SIZE; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

// Function to normalize a vector
void normalize(double vector[]) {
    double magnitude = sqrt(dotProduct(vector, vector));
    for (int i = 0; i < SIZE; i++) {
        vector[i] /= magnitude;
    }
}

// Function to calculate the eigenvalues and eigenvectors of a matrix
void calculateEigen(double matrix[][SIZE], double eigenvalues[], double eigenvectors[][SIZE]) {
    // Initialize the eigenvectors to the identity matrix
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            eigenvectors[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Power iteration method to calculate the dominant eigenvalue and eigenvector
    int maxIterations = 100;
    double epsilon = 1e-6; // convergence threshold
    double prevEigenvalue = 0.0;

    for (int iter = 0; iter < maxIterations; iter++) {
        // Multiply the matrix with the current eigenvector
        double tmp[SIZE];
        for (int i = 0; i < SIZE; i++) {
            tmp[i] = 0;
            for (int j = 0; j < SIZE; j++) {
                tmp[i] += matrix[i][j] * eigenvectors[j][0];
            }
        }

        // Calculate the eigenvalue using Rayleigh quotient
        double eigenvalue = dotProduct(tmp, eigenvectors[0]);

        // Check convergence
        if (fabs(eigenvalue - prevEigenvalue) < epsilon) {
            break;
        }

        // Update the eigenvector
        normalize(tmp);
        for (int i = 0; i < SIZE; i++) {
            eigenvectors[i][0] = tmp[i];
        }

        // Update the eigenvalue
        prevEigenvalue = eigenvalue;
    }

    // Assign the eigenvalue and normalize the final eigenvector
    eigenvalues[0] = prevEigenvalue;
    normalize(eigenvectors[0]);
}

int main() {
    double matrix[SIZE][SIZE] = {
        {2, 1},
        {1, 3}
    };

    double eigenvalues[SIZE];
    double eigenvectors[SIZE][SIZE];

    calculateEigen(matrix, eigenvalues, eigenvectors);

    printf("Eigenvalues: %.6f, %.6f\n", eigenvalues[0], eigenvalues[1]);
    printf("Eigenvectors: [%.6f, %.6f], [%.6f, %.6f]\n",
        eigenvectors[0][0], eigenvectors[1][0],
        eigenvectors[0][1], eigenvectors[1][1]);

    return 0;
}
