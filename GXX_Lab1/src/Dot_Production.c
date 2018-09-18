#include <stdio.h>

float Dot_Production(float *arr_a, float *arr_b, int size, float result){
	int i;
	for(i = 0; i < size; i++){
		result = result + arr_a[i] * arr_b[i];
	}

	return result;
}