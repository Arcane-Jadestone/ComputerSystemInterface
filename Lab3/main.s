

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT Start
	IMPORT PortF_Init
	IMPORT exp
	IMPORT delay
	IMPORT delayVari
	IMPORT mulsum4
	IMPORT blue_led_on
	IMPORT blue_led_off
	IMPORT red_led_on
	IMPORT red_led_off
	IMPORT green_led_on
	IMPORT green_led_off
	IMPORT leds_off
	
	;Be sure to import your created functions


		
Start
	BL PortF_Init
loop

	;Comment and uncomment the appropriate branch instructions to demo the different parts of the lab
	;B part1
	;B part2
	;B part3
	B part4
	
	B loop
	
	
part1
	
	;Toggle an LED every 25ms using your created 25ms delay subroutine
	
	;Turn on LED
	;Call 25 ms delay subroutine
	;Turn off LED
	;Call 25 ms delay subroutine
	BL red_led_on
	BL delay   
	BL red_led_off
	BL delay
	
	B loop
	
	
part2

	;Toggle an LED every 500ms using your created variable delay subroutine

	;Turn on LED
	BL red_led_on
	MOV R0, #20
	BL delayVari   
	BL red_led_off
	MOV R0, #20
	BL delayVari	
	
	
	;Call variable delay subroutine
	;Turn off LED
	;MOV R0, #20
	;Call variable delay subroutine

	B loop
	
	
part3
	
	B testexp ;Comment out to do your own testing
	
	;You can use this section to test your "exp" function
	;Example of self testing
	;MOV #3
	;MOV #2
	;BL exp
	;NOP ;Check debugger value to make sure result is 9	
	
	
	B loop
	
	
part4

	B testmulsum4 ;Comment out to do your own testing
	
	B loop



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Code for testing Parts 3 and 4
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Tests exp subroutine which performs (A+B) to the power of A
;Turns on green LED if exp is correct
;Turns on red LED if exp is incorrect
testexp
	BL leds_off
	MOV R0, #1
	MOV R1, #2
	MOV R4, #3
	BL exp
	CMP R0, R4
	BNE testexpfail
	MOV R0, #2
	MOV R1, #3
	MOV R5, #25
	BL exp
	CMP R0, R5
	BNE testexpfail
	MOV R0, #3
	MOV R1, #7
	MOV R6, #1000
	BL exp
	CMP R0, R6
	BNE testexpfail
	
	MOV R0, #2
	MOV R1, #2
	MOV R7, #16
	BL exp
	CMP R0, R7
	BNE testexpfail
	
	MOV R0, #3
	MOV R1, #3
	MOV R8, #216
	BL exp
	CMP R0, R8
	BNE testexpfail
	
	MOV R0, #4
	MOV R1, #4
	MOV R9, #4096
	BL exp
	CMP R0, R9
	BNE testexpfail
	
	MOV R0, #2
	MOV R1, #6
	MOV R10, #64
	BL exp
	CMP R0, R10
	BNE testexpfail
	
	BL green_led_on	;All tests passed
	B testexpend	
testexpfail
	BL red_led_on
testexpend	
	B loop
	

;Tests mulsum4 which performs ((A+B)*(C+D)*3)+D+(B*C*4)
;	(A+B+C)*(B+C+D)*2
;Turns on blue LED if func4 is correct
;Turns on red LED if func4 is incorrect
testmulsum4
	BL leds_off
	MOV R0, #1
	MOV R1, #1
	MOV R2, #1
	MOV R3, #1
	MOV R4, #18
	BL mulsum4
	CMP R0, R4
	BNE testmulsum4fail
	MOV R0, #2
	MOV R1, #3
	MOV R2, #4
	MOV R3, #5
	MOV R5, #216
	BL mulsum4
	CMP R0, R5
	BNE testmulsum4fail
	MOV R0, #3
	MOV R1, #3
	MOV R2, #5
	MOV R3, #5
	MOV R6, #286
	BL mulsum4
	CMP R0, R6
	BNE testmulsum4fail
	
	MOV R0,#2
	MOV R1,#2
	MOV R2,#2
	MOV R3,#2
	MOV R7,#36
	MOV R8,#36
	BL mulsum4
	ADD R7,R7,R8
	CMP R0,R7
	BNE testmulsum4fail
	
	MOV R0,#3
	MOV R1,#4
	MOV R2,#1
	MOV R3,#3
	MOV R9,#64
	MOV R10,#64
	BL mulsum4
	ADD R9,R9,R10
	CMP R0,R9
	BNE testmulsum4fail
	
	BL blue_led_on ;All tests passed
	B testmulsum4end
testmulsum4fail
	BL red_led_on
testmulsum4end
	B loop


	ALIGN
	END