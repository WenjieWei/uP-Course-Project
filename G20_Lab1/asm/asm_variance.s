<<<<<<< HEAD:G20_Lab1/asm/asm_variance.s
AREA test, CODE, READONLY

export asm_variance                  ; label must be exported if it is to be used as a function in C
asm_variance

VLDR.f32 S0, [R0]
VLDR.f32 S1, [R0]

STAR
  LDR R4, [R1]
  LDR R5, [R1]

AVERAGE
  SUBS R4, R4, #1
  BLT VARIANCE_PRE
  VADD.f32 S4, S4, S0               ; If there are anything wrong, just try VLDR.f32 S4, S4, =0 first
  VLDR.f32 S0, R0, R4, LSL #2
  B AVERAGE

VARIANCE_PRE
  VDIV.f32 S4, S4, R1

VARIANCE
  SUBS R5, R5, #1
  BLT VARIANCE_POST
  VSUBS.f32 S6, S1, S4
  VMUL.f32 S6, S6, S6
  VADD.f32 S7, S7, S6
  VLDR.f32 S1, R0, R5, LSL #2
  B VARIANCE

VARIANCE_POST
  VDIV.f32 S7, S7, R1

DONE
  VSTR S7, [R2]

  BX LR

  END
=======
	AREA text, CODE, READONLY
	export asm_variance					; label must be exported if it is to be used as a function in C
		
	; The subroutine for variance calculation
	; R0 holds the base address of the vector
	; R1 holds the size of the vector, which is used as the counter
	; R2 holds the address of the result
	
asm_variance	
	MOV R3, R1							; R3 holds the value of the counter, as a backup
	PUSH {R1}
	PUSH {R0}							; Back up the base address
	VLDR.f32 S0, [R0]
	VLDR.f32 S1, [R0]
	LSL R1, R1, #2						; Convert the counter to word size
	MOV R4, #0
	VMOV.f32 S4, R4						; Initialize S4 to hold the temporary sum
	VCVT.f32.u32 S4, S4					; Convert it to float 

AVERAGE
	SUBS R1, R1, #4						; Decrement the counter
	BLT VARIANCE_PRE					; Branch out if counter value drops below 0
	VADD.f32 S4, S4, S0					; Sum up
	ADD R0, R0, #4
	VLDR.f32 S0, [R0]					; Now S0 holds the value of the next number in the array
	B AVERAGE

VARIANCE_PRE
	VMOV.f32 S5, R3						; Initialize S5 to hold the size of the array
	VCVT.f32.u32 S5, S5					; Convert it to float
	VDIV.f32 S4, S4, S5					; S4 holds the average
	LSL R3, R3, #2
	POP{R0}

VARIANCE
	SUBS R3, R3, #4
	BLT VARIANCE_POST
	VSUB.f32 S6, S1, S4
	VMUL.f32 S6, S6, S6
	VADD.f32 S7, S7, S6
	
	ADD R0, R0, #4
	VLDR.f32 S1, [R0]
	B VARIANCE

VARIANCE_POST
	POP {R1}
	SUB R1, R1, #1
	VMOV.f32 S0, R1
	VCVT.f32.u32 S0, S0
	VDIV.f32 S7, S7, S0

DONE
	VSTR S7, [R2]

	BX LR

	END
>>>>>>> 8dc26f5c929b3f252ac9cf31c2c6e3adf89bfb89:GXX_Lab1/asm/asm_variance.s
