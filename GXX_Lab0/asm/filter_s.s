	AREA text, CODE, READONLY
	export FILTER_S
FILTER_S
	; This is the subroutine for calculating the filtered value.
	; There are four input arguments:
	; R0 for the location of array OriginalSignal
	; R1 for the location of array FilteredSignal
	; R2 for the number of N
	; R3 for the number of D
	; R4 holds the temporary sum.

	CMP R3, #0
	BEQ FINISH				; Quit immediately for 0 depth
	
	MOV R5, #0			; R5 holds the counter i of the outer loop, which iterates through the original data array
	SUB R5, #4			; Decrement R5 once, for ease of use in the loop.
	
ORI_SIG_LOOP
	ADD R5, R5, #4		; Increment the counter.
	CMP R5, R2				
	BGE FINISH				; Branch to FINISH if (i >= N)
	
	SUB R8, R5, R3, LSR #1	; R8 holds the value of (i - D/2)
	LDR R7, [R8, #-4]		; R7 holds the counter j of the inner loop.
	
	AND R12, R3, #1			; Check if D is odd. 1 for odd, 0 for even.	
	BEQ EVEN_DEPTH			; Branch to EVEN_DEPTH if D is even (R12 == 0)
	B ODD_DEPTH				; Otherwise branch to ODD_DEPTH
	
ODD_DEPTH
	ADD R7, R7, #4		; Increment the counter
	ADD R8, R5, R3, LSR #1	; R8 now holds the value of (i + D/2)
	CMP R7, R8				; If j > (i + D/2)
	BGT AVERAGE				; Jump out from the inner loop
	
	CMP R7, #0				; If j < 0
	BLT ODD_DEPTH			; Do not change the sum
	SUB R10, R2, #1			; R10 = N - 1
	CMP R7, R10				; If j > (N-1)
	BLT ODD_DEPTH			; Do not change the sum
	
	;ADD R4, R4, R0			; sum += original signal
	;ADD R0, R0, #4			; Now R0 points to the next number in the original signal array.
	VMOV S0, #0x00			; Use S0 to hold temp sum.
	VLDR S1, [R0]			; Load the first float value into S1
	VADD S0, S0, S1			; sum += original signal
	B ODD_DEPTH
	
EVEN_DEPTH

AVERAGE
	; Division

FINISH
	BX LR	
	END
