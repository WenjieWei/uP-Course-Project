#include <stdio.h>
#include <stdlib.h>
#include <filter.h>

int main(){
	float signal[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
	float *filtered;
	filtered = malloc(6 * sizeof(float));
	
	//filter(signal, filtered, 6, 4);
	FILTER_S(signal, filtered, 6, 3);
	
	return 0;
}