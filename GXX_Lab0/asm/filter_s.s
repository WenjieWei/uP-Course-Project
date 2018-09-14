	AREA text, CODE, READONLY
	export FILTER_S
FILTER_S
	; This is the subroutine for calculating the filtered value.
	; There are four input arguments:
	; R1 for the location of array OriginalSignal
	; R2 for the location of array FilteredSignal
	; R3 for the number of N
	; R4 for the number of D
	; R0 holds the return value, in this case, the average result
	; R5 holds the temporary sum.
	; R6 - R12 are general purpose reg. All used later.
	CMP R4, #0
	BEQ FINISH				; Quit immediately for 0 depth
	
	MOV R6, #0				; R6 holds the counter i of the outer loop, which iterates through the original data array
	MOV R5, #0				; R5 holds the temporary sum. Initialize it to 0.
	SUB R6, #4				; Decrement R6 once, for ease of use in the loop.
	
ORI_SIG_LOOP
	ADD R6, R6, #4			; Increment the counter.
	CMP R6, R3				
	BGE FINISH				; Branch to FINISH if (i >= N)
	
	SUB R8, R6, R4, LSR #1	; R8 holds the value of (i - D/2)
	ADD R9, R6, R4, LSR #1	; R9 holds the value of (i + D/2)
	LDR R7, [R8, #-4]		; R7 holds the counter j of the inner loop.
	
	AND R12, R4, #1			; Check if D is odd. 1 for odd, 0 for even.	
	BEQ EVEN_DEPTH			; Branch to EVEN_DEPTH if D is even (R12 == 0)
	B ODD_DEPTH				; Otherwise branch to ODD_DEPTH
	
ODD_DEPTH
	ADD R7, R7, #4			; Increment the counter
	CMP R7, R9				; If j > (i + D/2)
	BGT AVERAGE				; Jump out from the inner loop
	
	CMP R7, #0				; If j < 0
	BLT ODD_DEPTH			; Do not change the sum
	SUB R10, R3, #1			; R10 = N - 1
	CMP R7, R10				; If j > (N-1)
	BLT ODD_DEPTH			; Do not change the sum
	
	ADD R5, R5, R1			; sum += original signal
	ADD R1, R1, #4			; Now R1 points to the next number in the original signal array.
	B ODD_DEPTH
	
EVEN_DEPTH

AVERAGE
	SDIV R0, R5, R4			; average = sum / D
	LDR R2, [R0]			; Load the average into corresponding index in filtered array.
	ADD R2, R2, #4			; Now R2 points to the next element in Filtered array.
	B ORI_SIG_LOOP

FINISH
	BX LR	
	END
