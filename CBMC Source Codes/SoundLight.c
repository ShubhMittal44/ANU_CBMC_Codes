
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SPEED_OF_LIGHT 299792458.0

double custom_pow(double base, double exponent) {
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

double nondet_double(double lower_bound, double upper_bound) {
    double value;
    do {
        value = (double)rand() / RAND_MAX; 
        value = lower_bound + value * (upper_bound - lower_bound); 
    } while (isinf(value) || isnan(value));
    return value;
}

bool is_nan(double value) {
    return isnan(value);
}

double solveMHDdispersionRelation(double v_A, double k, double c) {
    double omega_squared = custom_pow(v_A, 2) * custom_pow(k, 2) + custom_pow(k, 2) * custom_pow(c, 2);
    if (is_nan(omega_squared) || omega_squared < 0.0) {
        return 0.0;
    }
    return sqrt(omega_squared);
}

int main() {
    double v_A, k, c;
    v_A = nondet_double(0.0, SPEED_OF_LIGHT); 
    k = nondet_double(0.0, SPEED_OF_LIGHT); 
    c = SPEED_OF_LIGHT;

    if (v_A < 0.0 || v_A >= c || k < 0.0 || k >= c) {
        printf("v_A and k must be between 0 and SPEED_OF_LIGHT\n");
        exit(1);
    }

    double omega = solveMHDdispersionRelation(v_A, k, c);

    double v_s = omega / k;

    if (is_nan(v_s) || isinf(v_s)) {
        printf("Invalid v_s\n");
        exit(1);
    }

    if (v_s > c) {
        printf("Assertion failed: v_s is greater than SPEED_OF_LIGHT\n");
        exit(1);
    }

    assert(v_s <= c);
    printf("Assertion succeeded: v_s is less than or equal to SPEED_OF_LIGHT\n");

    return 0;
}
