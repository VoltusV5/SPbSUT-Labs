#include <stdio.h>
#include <math.h>

int main(void) {
	double x, y;

	printf("x: "); scanf("%lf", &x);

	if (x > 2) 
		y = 4 + (x - 2) * (x - 2);
	else if (x > 1) 
		y = pow(2, x);
	else if (x > 0)
		y = x++;
	else 
		y = exp(x);

	printf("y: %.3lf\n", y);
	return 0;
}
