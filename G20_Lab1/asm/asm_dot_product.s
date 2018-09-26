	AREA test, CODE, READONLY
	export asm_dot_product				; label must be exported if it is to be used as a function in C
		
asm_dot_product
	VLDR.f32 S0, [R0]
	VLDR.f32 S1, [R1]

LOOP
	SUBS R2, R2, #1
	BLT DONE
	VMUL.f32 S2, S0, S1
	VADD.f32 S3, S3, S2
	VLDR.f32 S0, R0, R2, LSL #2
	VLDR.f32 S1, R1, R2, LSL #2
	B LOOP

DONE
	VSTR.f32 S3, [R3]

	BX LR
	END