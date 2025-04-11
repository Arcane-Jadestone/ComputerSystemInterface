
GPIO_PORTF_DATA_R  EQU 0x400253FC

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT  Start
	EXPORT 	Example_Function
	EXPORT 	Part3_Function
	EXPORT	Part4_Function
	EXPORT	Part5_Function
	IMPORT	delay
	IMPORT	leds_off
	IMPORT  PortF_Init
	IMPORT	Example
	IMPORT	Part3
	IMPORT  Part4
	IMPORT	Part5
		
Start
	BL PortF_Init 	; Initialize the LEDs and Pushbuttons
	BL debug		; Useful for parts 2 and 3
loop
	LDR R1, =GPIO_PORTF_DATA_R
	LDR R0, [R1]
	AND R0, #0x11	;Get just the pushbutton values
	CMP R0, #0x11	;No buttons pressed?
	BNE checkSW1
	BL Example
	B loop
checkSW1
	CMP R0, #0x01 	;SW1 pressed?
	BNE checkSW2
	BL Part3
	B blink
checkSW2
	CMP R0, #0x10 	;SW2 pressed?
	BNE both
	BL Part4
	B blink
both
	BL Part5		;Both must be pressed
	
	;Create blinking effect
blink
	BL delay
	BL leds_off
	BL delay

	B loop



debug
	;Place parts 1 and 2 here
	MOV R1, #0x00000008
	MOV R2, #0xFFFFFFFD
	MOV R3, #0x7FFFFFFF
	ADDS R0, R1, R2   ; 0x0000 0005| C
	SUBS R0, R1, R1   ; 0x0000 0000| ZC
	ADDS R0, R1, R3   ; 0x8000 0007| NV
	SUBS R0, R2, R3   ; 0x7fff fffe| CV
	ADDS R0, R2, R3   ; 0x7fff fffc| C
	
	;Flag Meanings
;N = Negative
;Z = Zero
;C = Cary (Unsigned)
;V = Overflow (Signed)	
	
	
;	First run:
	MOV R7, #0x20000000
;Then:
	LDR R6, [R7]
;R6 =A07C F8DF  | Reading from 0x2000003 reading backwards to 0x20000000
	LDRH R6, [R7]
;R6 =0000 F8DF  | Reads first half of [R7] into R6 (Remainder being read as 00 | Unsigned)
	LDRB R6, [R7]
;R6 =0000 00DF  | Reads first byte of [R7] into R6 (Remainder being read as 00 | Unsigned)
	LDRSH R6, [R7]
;R6 =FFFF F8DF  | Reads first half of [R7] into R6 (Remainder being read as FF | Signed)
	LDRSB R6,[R7]
;R6 =FFFF FFDF  | Reads first byte of [R7] into R6 (Remainder being read as FF | Signed)

;Signed means that if the furthest read bit is a '1' the the unfilled spaces will be set to 'F'

;First run:
	MOV R7, #0x20000000
	ADD R7, #2
; Must use add because 0x20000002 cannot be represented as
; an immediate value
;Then:
	LDR R6, [R7]
;R6 = 0xF8DF A07C
	LDRH R6, [R7]
;R6 = 0x0000 A07C
	LDRB R6, [R7]
;R6 = 0x0000 007C
	LDRSH R6, [R7]
;R6 = 0xFFFF A07C
	LDRSB R6,[R7]
;R6 = 0x0000 007c

;See above section for explinations

	BX LR ;Returns to main function



; Returns Z = A+B
; A and B are in R0 and R1, respectively
; Z should be placed in R0
Example_Function
	ADD R0, R0, R1 ;Comment out this instruction to see the Example fail
	BX LR


; Should return Z = (A << 2)|(B & 15)
; Assume A and B are in R0 and R1, respectively
; The value of Z should be placed in R0 at the end
Part3_Function
	LSLS R0, R0, #2
	ANDS R1, R1, #15
	ORRS R0, R0, R1
	
	BX LR
	
	
; Should return Z = ((A + B) + (A - B)) | (A << B)
; Assume A and B are in R0 and R1, respectively
; The value of Z should be placed in R0 at the end
Part4_Function
	ADDS R2, R0, R1
	SUBS R3, R0, R1
	ADDS R2, R2, R3
	LSLS R0, R0, R1
	ORRS R0, R2, R0

	BX LR


; Should return Z = X + Y
; Where X = ((A*8)|(B*4)|(C*2))  -> R0 = ( (R0=((R0=R0*8)|(R3=R1*4))) | (R3=R2*2) )
; and Y = ((B xor 2)|(C xor 4)  -> R3 = ( (R1=R1 xor #2) | (R2= R2 xor #4))
; Assume A, B, and C are in R0, R1, and R2, respectively
; The value of Z should be placed in R0 at the end
Part5_Function
	MOVS R4, #8
	MUL R0, R0, R4
	MOVS R4, #4
	MUL R3, R1, R4
	
	ORRS R0, R0, R3
	MOVS R4, #2
	MUL R3, R2, R4
	ORRS R0, R0, R3
	
	MOVS R4, #2
	EORS R1, R1, R4
	MOVS R4, #4
	EORS R2, R2, R4
	ORRS R3, R1, R2

	ADDS R0, R0, R3
	BX LR



	ALIGN        ; make sure the end of this section is aligned
	END          ; end of file