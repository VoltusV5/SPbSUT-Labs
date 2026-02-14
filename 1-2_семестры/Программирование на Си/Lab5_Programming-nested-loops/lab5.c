#include <stdio.h>
#include <math.h>

int main(void) {
	double x, Sum = 0, Sum1, Sum2 = 0, Mult;
	int m, n;

	printf("x: "); scanf("%lf", &x);
	printf("m: "); scanf("%d", &m);
	printf("n: "); scanf("%d", &n);

	for (int k = 1; k <= m; k++)
		Sum2 += k * k;


	for (int i = 1; i <= n; i++) {
		Sum1 = 0; Mult = 1;
		
		for (int j = 1; j <= m; j++){
			double ij = i - j;
			Sum1 += ij * ij;
			Mult *= sqrt(fabs(ij));
		}
		Sum += (x + Sum1 + Sum2) / (i + Mult);
	}

	printf("%6.3lf\n", 2 * x + Sum);
	return 0;
}