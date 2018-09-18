#include <stdio.h>
#include <math.h>

float average(float *arr, int size){
	int i;
	float sum = 0;
	for (i = 0; i < size; i++){
		sum = sum + arr[i];
	}
	return sum/size;
}

void variance(float *arr, int size, float *result){
	int i;
	float ave = average(arr, size);
	float sum = 0;
	for (i = 0; i < size; i++){
		sum = sum + pow((arr[i] - ave), 2);
	}
	
	*result = sum/size;
}