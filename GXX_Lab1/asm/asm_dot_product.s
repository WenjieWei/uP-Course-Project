	AREA test, CODE, READONLY
	export asm_dot_product				; label must be exported if it is to be used as a function in C
		
	; Subroutine for calculation of the dot product.
	; R0 holds the base address of the first vector
	; R1 holds the base address of the second vector.
	; R2 holds the size of the two vectors.
	; R3 holds the address of the result.
	
asm_dot_product
	VLDR.f32 S0, [R0]					; 
	VLDR.f32 S1, [R1]
	LSL R2, R2, #2						; Change the counter to word size
	MOV R4, #0
	VMOV S3, R4
	VCVT.f32.u32 S3, S3

LOOP
	SUBS R2, R2, #4						; Decrement the counter
	BLT DONE							; Branch to finish if counter drops below 0.
	VMUL.f32 S2, S0, S1					; Multiply corresponding entries
	VADD.f32 S3, S3, S2					; Add up with the previous result
	
	ADD R0, R0, #4						; Now R0 points to the next number in the first array
	ADD R1, R1, #4						; Now R1 points to the next number in the second array
	VLDR.f32 S0, [R0]
	VLDR.f32 S1, [R1]
	B LOOP

DONE
	VSTR.f32 S3, [R3]

	BX LR
	END