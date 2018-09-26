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
