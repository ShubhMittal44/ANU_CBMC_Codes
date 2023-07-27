#include <assert.h>

int factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

int main() {
    int input;
    int result = factorial(input);

    return 0;
}
