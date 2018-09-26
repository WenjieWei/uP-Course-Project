#include <stdio.h>

/* Filter function*/
void filter(float *original_sig, float *filtered_sig, int N, int D){
	//This function takes four inputs, with N representing number of elements in the array.
	//D represents the filter depth defined by the user.
	
	int i, j, odd;
	float sum, result;
	
	printf("You have entered %d raw data, and the filter depth is %d.\n", N, D);
	
	//examine if D is even or odd.
	if(D % 2){
		//odd
		odd = 1;
	} else {
		odd = 0;
	}
	
	
	for(i = 0; i < N; i++){
		sum = 0;
		result = 0;
		if(odd){
			//range is x[n-D/2] ~ x[n+D/2]
			for(j = (i - D/2); j <= (i + D/2); j++){
				if(j < 0){
					sum += 0;
				} 
				else if (j > (N - 1)) {
					sum += 0;
				} 
				else {
					sum += original_sig[j];
				}
			}
		} else {
			for (j = (i - D/2); j < (i + D/2); j++){
				if(j < 0){
					sum += 0;
				} 
				else if (j > (N - 1)) {
					sum += 0;
				} 
				else {
					sum += original_sig[j];
				}
			}
		}
		result = sum / D;
		filtered_sig[i] = result;
		
		printf("%f, ", filtered_sig[i]);
	}
	printf("\nFiltering Complete.\n");
}