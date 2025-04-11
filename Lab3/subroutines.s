

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	export exp
	EXPORT mulsum4
	IMPORT green_led_on
	IMPORT red_led_on
	IMPORT leds_off
	;Be sure to export created functions
	
	
	

		

;Should perform the operation (R0+R1)^R0
;Takes the sum of R0 and R1 and multiplies it by itself R0 times (i.e. raises R0+R1 to the power of R0)
;Assume R0 is greater than 0
;Should return the result in R0
;Inputs: R0 and R1
;Outputs: R0
exp
	
	PUSH{LR}
	
	ADD R2, R0, R1
	MOV R3, R0
	MOV R0, #1
	
loop2
	CMP R3, #0
	BEQ finish
	MUL R0, R0, R2
	SUBS R3, R3, #1
	B loop2
	
finish
	POP {LR}
	BX LR


;Should perform the operation (A+B+C)*(B+C+D)*2	
;A, B, C, and D are passed in order in R0-R3
;Should return the result in R0
;Inputs: R0, R1, R2, and R3
;Outputs: R0
mulsum4
	PUSH{LR}

	
	ADD R0, R0, R1
	ADD R0, R0, R2
	
	ADD R1, R1, R2
	ADD R1, R1, R3

	MUL R0, R0, R1
	ADD R0, R0, R0
	
	
	POP {LR}
	BX LR
	
	
	ALIGN
	END