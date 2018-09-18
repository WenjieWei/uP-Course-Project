#include <stdio.h>

void Dot_Production(float* arr_a, float* arr_b, int size, float *result){
	int i;
	float temp_result = 0;
	for(i = 0; i < size; i++){
		temp_result += (arr_a[i] * arr_b[i]);
	}
	
	*result = temp_result;
}