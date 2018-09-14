#ifndef _FILTER_H
#define _FILTER_H

	extern float FILTER_S(float *original_sig, float *filtered_sig, int N, int D);
	void filter(float *a, float *b, int num, int depth);
	
#endif