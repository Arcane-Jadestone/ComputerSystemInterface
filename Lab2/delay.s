; Credit: Based on software by Valvano

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT   delay
	EXPORT	 delay_dim



;------------delay------------
; Delay function for testing
; Input: none
; Output: none	
DELAYVAL		EQU 2700000 ;16000*500 = 8000000 1.5 seconds per loop 	;Change to calculated value

delay
	LDR R0, =DELAYVAL 			; put the value of DIMSEC into R0
delay_loop
	SUBS R0, R0, #1				; R0 = R0 - 1
	BNE delay_loop 			; repeat until R0 == 0
	BX LR						; return

	
	
DIMSEC			EQU 16000 		; delay_dim_loop will run this many time to create a delay

delay_dim
	LDR R0, =DIMSEC 			; put the value of DIMSEC into R0
delay_dim_loop
	SUBS R0, R0, #1				; R0 = R0 - 1
	BNE delay_dim_loop 			; repeat until R0 == 0
	BX LR						; return
	
	ALIGN                       ; make sure the end of this section is aligned
    END                         ; end of file
