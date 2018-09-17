#include <stdio.h>

int i;

float variance(float *arr, int size, result){
  float ave = average(arr, size);
  float sum;
  for (i = 0; i < size; i++){
    sum = sum + (arr[i] - ave)^2;
  }
  return sum/size;
}

float average(float *arr, int size){
  float sum;
  for (i = 0; i < size; i++){
    sum = sum + arr[i];
  }
  return sum/size;
}
