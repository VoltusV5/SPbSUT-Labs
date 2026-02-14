#include <stdio.h>

int matrix_input(int rows, int columns, double arr[rows][columns]){
	for (int i = 1; i <= rows; i++){
		for (int j = 1; j <= columns; j++) {
			scanf("%lf", &arr[i][j]);
		}
	}
	return 0;
}



int matrix_process(int rows, int columns, double* mult_arr, double arr[rows][columns], double paramb) {
	double mult;
	char changes = 'F';
	for (int i = 1; i <= columns; i++){
		mult = 1;
		for (int j = 1; j <= rows; j++) {
			if (arr[j][i] < paramb) {
				mult *= arr[j][i];
				changes = 'T';
			}
		mult_arr[i] = mult;
		}
	}
	if (changes != 'T') 
		printf("all values > b\n");
	
	return 0;
}


int matrix_output(int rows, int columns, double* mult_arr, double arr[rows][columns]) {
	for (int i = 1; i <= columns; i++) {
		printf("%10.2lf ", mult_arr[i]);
	}
	printf("\n");
	printf("--------------------------------------------------------\n");
	for (int i = 1; i <= rows; i++){
		for (int j = 1; j <= columns; j++) {
			printf("%10.2lf ", arr[i][j]);
		}
	printf("\n");
	}
	
	return 0;
}

