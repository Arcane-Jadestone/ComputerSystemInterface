
	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT   delay
	EXPORT delayVari

		
	;Be sure to export your created functions
	
;133333.33 ;25 ms seconds per loop 	;Change to calculated value
delay
	MOV R1, #57142 			; put the value of DIMSEC into R0   | 
delay_loop
	MOV R0, R0 
	MOV R0, R0
	MOV R0, R0
	MOV R0 , R0
	SUBS R1, R1, #1				
	BNE delay_loop 			; repeat until R1 == 0
	BX LR
	
	
delayVari
	MOV R1, #57142 			; put the value of DIMSEC into R0   | 
delayVari_loop
	MOV R0, R0 
	MOV R0, R0
	MOV R0, R0
	MOV R0 , R0
	SUBS R1, R1, #1				
	BNE delayVari_loop 			; repeat until R1 == 0
	
	SUBS R0, R0, #1
	BNE delayVari
	BX LR

	
	ALIGN
	END