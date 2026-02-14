#include <stdio.h>

int arr_input(double arr[], int length) {
	for (int i = 1; i <= length; i++)
		scanf("%lf", &arr[i]);
	return 0;
}

int arr_processing(double arr[], int length) {
	double sum = 0;
	for (int i = 1; i <= length; i++) {
		sum += arr[i];
	}
	for (int i = 1; i <= length; i++) {
		arr[i] = arr[i] / sum;
	}
	return 0;
}

int final_arr_processing(double arr1[], double arr2[], double arrFinal[], int length) {
	for (int i = 1; i <= length; i++) {
		arrFinal[i] = arr1[i] + arr2[i];
	}
	return 0;
}

int arr_output(double arr[], int length) {
	for (int i = 1; i <= length; i++)
		printf("%.3lf ", arr[i]);
	printf("\n");
	return 0;
}