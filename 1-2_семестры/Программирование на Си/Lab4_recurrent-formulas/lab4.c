#include <stdio.h>
#include <math.h>

int main(void) {
	int n, i;
	double x, z, p, const_p;
	char item;

	printf("x: "); 		scanf("%lf", &x);
	printf("n: "); 		scanf("%d", &n);

	const_p = exp(-pow(x, 1.2)) + x;
	while (1) {
		p = 1; z = 0;
		
		printf("symbol ('e' = finish the program): ");  scanf(" %c", &item);

		switch(item) {
		case '1':
		case 'f':
		case 'F':
			for (i = 3; i <= n; i++){
				p *= const_p + pow(i, (double)1/3);
			}
			z = 49 * x + 4.9 * p;
			break;
		case '2':
		case 'w':
		case 'W':
			i = 3;
			while (i <= n){
				p *= const_p + pow(i, (double)1/3);
				i++;
			}
			z = 49 * x + 4.9 * p;
			break;
		case '3':
		case 'd':
		case 'D':
			i = 3;
			do {
				p *= const_p + pow(i, (double)1/3);
				i++;
			} while (i <= n);
			z = 49 * x + 4.9 * p;
			break;
		case 'e':
			return 0;
		default:
			printf("INVALID CHARACTER\n");
			continue;
		}
		printf("z: %.2lf\n", z);
	}
}