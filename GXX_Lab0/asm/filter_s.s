	AREA text, CODE, READONLY
	export filter_s
filter_s
	; This is the subroutine for calculating the filtered value.
	; There are four input arguments:
	; R1 for the location of array OriginalSignal
	; R2 for the location of array FilteredSignal
	; R3 for the number of N
	; R4 for the number of D
	
	AND R7, R4, #1		; R7 stores if D is odd. 1 for odd, 0 for even.
	
	MOVS R5, #0x00		; i is the counter of the outer loop. Stored in R5.
	SUBS R5, R5, #0x04	; Decrement the counter by one for ease of use in the future loops.
	
	
	BX LR
	
	END