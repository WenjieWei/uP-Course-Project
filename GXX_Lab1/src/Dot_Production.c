#include <stdio.h>

int i;

float Dot_Production(float *arr_a, float *arr_b, int size, float result){
  for(i = 0; i < size; i++){
    result = result + arr_a[i] * arr_b[i];
  }

  return result;
}
