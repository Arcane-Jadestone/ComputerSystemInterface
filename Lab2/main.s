



	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT  Start
	IMPORT  PortF_Init;
	IMPORT  delay
	IMPORT	delay_dim
	IMPORT  blue_led_on
	IMPORT  blue_led_off
	IMPORT  red_led_on
	IMPORT  red_led_off
	IMPORT  green_led_on
	IMPORT  green_led_off

Start
	BL PortF_Init; ; Initialize the LEDs and Pushbuttons

loop

	B parts34 ;Uncomment this line to skip the toggling routine used in Parts 1 and 2

	BL red_led_on
	BL delay   ;delay or delay_dim
	BL red_led_off
	BL delay
	B loop


parts34


	;B red_start
	MOV R2, #4
red_loop
		BL red_led_on
		BL delay   ;delay or delay_dim
		BL red_led_off
		BL delay
		SUBS R2, R2, #1
		BNE red_loop
	
	;B blue_start
	MOV R3, #3
blue_green_loop
	
	
	MOV R2, #3
blue_loop
		BL blue_led_on
		BL delay   ;delay or delay_dim
		BL blue_led_off
		BL delay
		SUBS R2, R2, #1
		BNE blue_loop
	
	;B green_start
	MOV R2, #2
green_loop
		BL green_led_on
		BL delay   ;delay or delay_dim
		BL green_led_off
		BL delay
		SUBS R2, R2, #1
		BNE green_loop
		
	SUBS R3, R3, #1
	BNE blue_green_loop
	
	B parts34
	
	        ; make sure the end of this section is aligned



REDLOOP			EQU 4 
red_startsdsd
	ADD R3, LR, #1
	LDR R2, = REDLOOP			; put the value of REDLOOP into R2	
red_loopsdsd
	BL red_led_on
	BL delay
	BL red_led_off
	BL delay
	
	SUBS R2, R2, #1				; R2 = R2 - 1
	
	BNE red_loop 			; repeat until R2 == 0
	MOV LR , R3
	BLX LR				; return
	
	                       ; make sure the end of this section is aligned


BLUELOOP			EQU 3
blue_startsds
	MOV R3, LR
	LDR R2, = BLUELOOP 			; put the value of REDLOOP into R2
blue_loopsdsds
	SUBS R2, R2, #1				; R2 = R2 - 1
	BL blue_led_on
	BL delay
	BL blue_led_off
	BL delay
	
	BNE blue_loop 			; repeat until R2 == 0
	MOV LR, R3
	BLX R3						; return
	
	                       ; make sure the end of this section is aligned

GREENLOOP			EQU 2
green_startsdsdd
	MOV R3, LR
	LDR R2, = GREENLOOP 			; put the value of REDLOOP into R2
green_loopsdssd
	SUBS R2, R2, #1				; R2 = R2 - 1
	
	BL green_led_on
	BL delay
	BL green_led_off
	BL delay
	
	BNE green_loop 			; repeat until R2 == 0
	MOV LR, R3
	BLX R3						; return
	
	                       ; make sure the end of this section is aligned
      
		
		
		
		
		
	ALIGN
	END          ; end of file