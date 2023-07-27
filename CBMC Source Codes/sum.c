#include <assert.h>

int add(int a, int b) {
	return a + b;
}

int main() {
	int x = 2;
	int y = 3;
	int z = add(x, y);

	assert(z == 5);

	return 0;
}