#include <stdio.h>

void void_summa(int start, int finish, int coefficient1, int coefficient2, int *psm) 
{
	int s = 0;
	for (int i = start; i <= finish; i++) {
		s += coefficient1 * i * i * i + coefficient2;
	}
	*psm = s;
}

int summa(int start, int finish, int coefficient1, int coefficient2) {
	int s = 0;
	for (int i = start; i <= finish; i++) {
		s += coefficient1 * i * i * i + coefficient2;
	}
	return s;
}

int main(void) {
	int n, m;
	double y;

	printf("m: "); scanf("%d", &m);
	printf("n: "); scanf("%d", &n);

	// Способ через return
	y = (double)(5 + summa(1, m, 2, 1)) / (3 + summa(3, n, 1, 2));
	printf("return y: %.3lf\n", y);
	int sm1 = 0;
	int sm2 = 0;
	// Способ через функцию void
	void_summa(1, m, 2, 1, &sm1);
	void_summa(3, n, 1, 2, &sm2);
	printf("void y: %.3lf\n", (double)(5 + sm1) / (3 + sm2));
	
}



// Способ print
	// print_summa(m, n);

/*
// print через void
void print_summa(int x, int y) {
	printf("print y: %.3lf\n", (double)(5 + summa(1, x, 2, 1)) / (3 + summa(3, y, 1, 2)));
}
*/
