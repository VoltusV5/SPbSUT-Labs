#include <stdio.h>

int main(void) {
	int n, m;
	double b;


	printf("n: "); scanf("%d", &n);
	printf("m: "); scanf("%d", &m);
	printf("b: "); scanf("%lf", &b);

	double matrix[n][m];
	double mult[m];

	int matrix_input(int rows, int columns, double arr[rows][columns]);
	int matrix_output(int rows, int columns, double* mult_arr, double arr[rows][columns]);
	int matrix_process(int rows, int columns, double* mult_arr, double arr[rows][columns], double paramb);

	printf("Введите матрицу, состоящую из %d строк и %d столбцов: \n", n, m);
	matrix_input(n, m, matrix);

	// Обработка
	matrix_process(n, m, mult, matrix, b);

	printf("Итоговая матрица: \n");
	matrix_output(n, m, mult, matrix);
	return 0;
}