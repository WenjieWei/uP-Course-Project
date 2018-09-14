	AREA text, CODE, READONLY
	export FILTER_S
FILTER_S
	; This is the subroutine for calculating the filtered value.
	; There are four input arguments:
	; R0 for the location of array OriginalSignal
	; R1 for the location of array FilteredSignal
	; R2 for the number of N
	; R3 for the number of D
	; R4 for the counter i
	; R5 for the counter j
	
	CMP R3, #0					; if (D = 0).
	BLE FINISH					; Quit immediately if D <= 0.
	
	MOV R4, #0					; R5 holds the counter i of the outer loop.
	SUBS R4, #4					; Decrement once for easier later application.
	
ORI_SIG_LOOP
	ADD R4, R4, #4				; Increment the loop counter.
	CMP R4, R2					; if (i >= N).
	BGE FINISH					; Quit the loop and finish.
	
	SUBS R6, R4, R3, LSR #1		; R6 holds the value of (i - D / 2).
	LSL R6, R6, #2				; Multiply R6 by 4 to make it a word size.
	SUBS R5, R6, #4				; R5 holds the inner loop counter j.
	ADDS R6, R4, R3, LSR #1		; Now R6 holds the value of (i + D / 2), the upper limit of j.
	LSL R6, R6, #4
	
	MOV R8, #0					; R8 is a temporary random register
	VMOV.f32 S1, R8				; S1 holds the temporary sum, initialized to 0.
	VCVT.f32.u32 S1, S1			; Convert to float
	
	AND R7, R3, #1				; if (D % 2 == 0)
	BEQ EVEN_DEPTH				; Branch to even if true
	B ODD_DEPTH					; Otherwise to odd.
	
ODD_DEPTH
	ADDS R5, R5, #4				; Increment the counter
	CMP R5, R6					
	BGT AVERAGE					; Branch if (j > i+D/2)
	
	CMP R5, #0
	BLT ODD_DEPTH				; Return to beginning of the loop and do nothing if j<0 (Out of bounds)
	CMP R5, R2			
	BGE ODD_DEPTH				; Return to beginning of the loop and do nothing if j >= N (Out of bounds)
	B AVERAGE					; Otherwise branch to AVERAGE and perform calculations for an average.
	
EVEN_DEPTH
	
AVERAGE
	VMOV.f32 S3, R3				; S3 holds the value of D.
	VCVT.f32.u32 S3, S3			; Convert the value held by S3 to float
	
	VLDR.f32 S2, [R0]			; S2 holds the value of the current original signal

	VADD.F32 S1, S1, S2			; Update the sum
	VDIV.F32 S0, S1, S3			; Calculation for average
	ADDS R0, R0, #4				; R0 points to the next number in the array
	VSTR.F32 S0, [R1]			; Store the average result to array filtered
	ADDS R1, R1, #4				; R1 points to the next number in the array.
	B ORI_SIG_LOOP
	
FINISH
	BX LR	
	END
