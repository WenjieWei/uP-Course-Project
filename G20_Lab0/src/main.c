<<<<<<< HEAD:G20_Lab0/src/main.c
#include <stdio.h>
#include <stdlib.h>
#include <filter.h>

int main(){
	float signal[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
	float *filtered;
	filtered = malloc(6 * sizeof(float));
	
	filter(signal, filtered, 6, 4);
	//FILTER_S(signal, filtered, 6, 3);
	
	return 0;
=======
#include <stdio.h>
#include <stdlib.h>
#include <filter.h>

int main(){
	float signal[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
	float *filtered;
	filtered = malloc(6 * sizeof(float));
	
	filter(signal, filtered, 6, 4);
<<<<<<< HEAD
	FILTER_S(signal, filtered, 6, 3);
=======
	//FILTER_S(signal, filtered, 6, 3);
>>>>>>> cd1c05d8b86ea319d0470f3417a0da1a2916e52f
	
	return 0;
>>>>>>> 8dc26f5c929b3f252ac9cf31c2c6e3adf89bfb89:GXX_Lab0/src/main.c
}