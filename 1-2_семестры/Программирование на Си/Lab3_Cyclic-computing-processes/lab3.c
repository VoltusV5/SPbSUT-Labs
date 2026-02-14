#include <stdio.h>
#include <math.h>

int main(void) {
	double n, a, x1, x2, x, y, dx;

	printf("n: "); scanf("%lf", &n);
	printf("a: "); scanf("%lf", &a);
	printf("x начальное: "); scanf("%lf", &x1);
	printf("x конечное: "); scanf("%lf", &x2);

	x = x1;
	dx = (x2 - x1) / (n - 1);
	
	printf("   i   |        x         |        y        \n");
	printf("--------------------------------------------\n");
	for (int i = 1; i <= n; i++) {
		y = 5 * sqrt(a + log(a) + log(x));
		printf("%6.d | %16.8lf | %16.8lf\n", i, x, y);
		x += dx;
	}

	return 0;
}
