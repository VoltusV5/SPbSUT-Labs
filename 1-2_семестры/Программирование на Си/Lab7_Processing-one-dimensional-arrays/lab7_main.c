#include <stdio.h>

int arr_input(double arr[], int length);
int arr_processing(double arr[], int length);
int final_arr_processing(double arr1[], double arr2[], double arrFinal[], int length);
int arr_output(double arr[], int length);

int main(void) {
	int n;
	
	printf("n: "); scanf("%d", &n);
	
	double a[n];
	double b[n];
	double c[n];
	double x[n];
	double y[n];

	// функции ввода
	printf("Введите массив a: ");
	arr_input(a, n);
	printf("Введите массив b: ");
	arr_input(b, n);
	printf("Введите массив c: ");
	arr_input(c, n);

	// функции обрaботки
	arr_processing(a, n);
	arr_processing(b, n);
	arr_processing(c, n);
	final_arr_processing(a, b, x, n);
	final_arr_processing(b, c, y, n);

	// функции Bыводa a' b' c'
	printf("Вывод массива a': \n");
	arr_output(a, n);
	printf("Вывод массива b': \n");
	arr_output(b, n);
	printf("Вывод массива c': \n");
	arr_output(c, n);
	

	// функции Bыводa x y
	printf("Вывод массива x: \n");
	arr_output(x, n);
	printf("Вывод массива y: \n");
	arr_output(y, n);


	return 0;
}