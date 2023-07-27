#include <stdio.h>
#include <z3.h>

#define SIZE 3

void matrix_multiply(Z3_context ctx, Z3_ast A[SIZE][SIZE], Z3_ast B[SIZE][SIZE], Z3_ast C[SIZE][SIZE]) {
    Z3_sort int_sort = Z3_mk_int_sort(ctx);
    Z3_ast zero = Z3_mk_int(ctx, 0, int_sort);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = zero;
            for (int k = 0; k < SIZE; k++) {
                Z3_ast args[2] = { A[i][k], B[k][j] };
                Z3_ast mul_result = Z3_mk_mul(ctx, 2, args);
                Z3_ast add_args[2] = { C[i][j], mul_result };
                C[i][j] = Z3_mk_add(ctx, 2, add_args);
            }
        }
    }
}

int main() {
    Z3_config cfg = Z3_mk_config();
    Z3_context ctx = Z3_mk_context(cfg);

    Z3_ast A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

    // Create Z3 integer matrices
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = Z3_mk_int(ctx, i * SIZE + j + 1, Z3_mk_int_sort(ctx));
            B[i][j] = Z3_mk_int(ctx, SIZE * SIZE - (i * SIZE + j), Z3_mk_int_sort(ctx));
            C[i][j] = Z3_mk_int(ctx, 0, Z3_mk_int_sort(ctx));
        }
    }

    matrix_multiply(ctx, A, B, C);

    // Verify correctness properties
    Z3_solver solver = Z3_mk_solver(ctx);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Z3_solver_assert(ctx, solver, Z3_mk_eq(ctx, C[i][j], Z3_mk_int(ctx, (i + 1) * (j + 1), Z3_mk_int_sort(ctx))));
        }
    }

    Z3_lbool result = Z3_solver_check(ctx, solver);

    if (result == Z3_L_TRUE) {
        printf("Matrix multiplication is correct.\n");
    }
    else if (result == Z3_L_FALSE) {
        printf("Matrix multiplication is incorrect.\n");
    }
    else {
        printf("Unable to determine the correctness of matrix multiplication.\n");
    }

    Z3_del_solver(ctx, solver);
    Z3_del_context(ctx);
    Z3_del_config(cfg);

    return 0;
}
