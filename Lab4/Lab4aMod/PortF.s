GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
GPIO_PORTF_LOCK_R  EQU 0x40025520
GPIO_PORTF_CR_R    EQU 0x40025524
GPIO_PORTF_AMSEL_R EQU 0x40025528
GPIO_PORTF_PCTL_R  EQU 0x4002552C
GPIO_LOCK_KEY      EQU 0x4C4F434B  ; Unlocks the GPIO_CR register
SYSCTL_RCGC2_R     EQU 0x400FE108
SYSCTL_RCGC2_GPIOF EQU 0x00000020  ; port F Clock Gating Control
LEDS			   EQU 0x40025038


RED       		   EQU 0x02
BLUE      		   EQU 0x04
GREEN     		   EQU 0x08
PF1				   EQU 0x40025008 ; RED LED PORT
PF2				   EQU 0x40025010 ; Blue LED PORT
PF3				   EQU 0x40025020 ; Green LED PORT

	

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT PortF_Init
	EXPORT LEDs_on
	EXPORT red_on
	EXPORT blue_on
	EXPORT green_toggle
	EXPORT all_off
	EXPORT buttons
	EXPORT delay
	
	;REMOVE BELOW
	EXPORT   blue_led_on
	EXPORT   blue_led_off
	EXPORT   red_led_on
	EXPORT   red_led_off
	EXPORT   green_led_on
	EXPORT   green_led_off
		
	
	;Be sure to export your created I/O functions
PART3			   EQU 0x00
PART7			   EQU 0xFF

		
PortF_Init
	;Do not use MOV in the code you write

	; 1) activate clock for Port F
    ; allow time for clock to finish
    LDR R1, =SYSCTL_RCGC2_R         
    LDR R0, [R1]                 
    ORR R0, R0, #0x20               
	STR R0, [R1]                  
    NOP
    NOP  
							 
    LDR R1, =GPIO_PORTF_LOCK_R      ; 2) unlock the lock register
    LDR R0, =0x4C4F434B             ; unlock GPIO Port F Commit Register
    STR R0, [R1]     
    LDR R1, =GPIO_PORTF_CR_R        ; enable commit for Port F
    MOV  R0, #0xFF                   ; 1 means allow access
    STR R0, [R1]    
	
	; 3) disable analog functionality (Our code)
    ; 0 means analog is off
	LDR R1, =GPIO_PORTF_AMSEL_R ; Load address of amsel
    ADD R0, #0x00
    STR R0, [R1]
    
	
    LDR R1, =GPIO_PORTF_PCTL_R      ; 4) configure as GPIO
    MOV R0, #0x00000000             ; 0 means configure Port F as GPIO
    STR R0, [R1]                  

	; 5) set direction register
	LDR R1, =GPIO_PORTF_DIR_R   ; Load address of Direction Register
    LDR R0, =0x0E               ; 0x0E = 00001110 (PF1, PF2, PF3 as output; PF0, PF4 as input)
    STR R0, [R1]                	

	LDR R1, =GPIO_PORTF_AFSEL_R     ; 6) regular port function
    MOV R0, #0                      ; 0 means disable alternate function 
    STR R0, [R1]                    
	LDR R1, =GPIO_PORTF_PUR_R       ; enable pull-up resistors for pushbuttons
	MOV R0, #0x11
    STR R0, [R1] 
	
    ; 7) enable Port F digital port (Our Code)
	LDR R1, =GPIO_PORTF_DEN_R   ; Load address of der
    ORR R0, #0xFF              ; Enable digital functionality on all five pins
    STR R0, [R1] 
    

	BX LR


; Used to test init function
; Format should not be copied for later parts of the lab, instead use GPIO_PORTF_DATA_R
LEDs_on
	LDR R0, =LEDS
	MOV R1, #0xFF
	STR R1, [R0]
	BX LR


;Add your functions here

red_on
	LDR R1, =PF1
	LDR R0, [R1]      
	ORR R0, R0, #RED
	STR R0, [R1]     
	BX  LR 
	
blue_on
	LDR R1, =PF2
	LDR R0, [R1]      
	ORR R0, R0, #BLUE
	STR R0, [R1]     
	BX  LR 

green_toggle
  LDR R1, =PF3
  LDR R0, [R1]      
  EOR R0, R0, #GREEN
  STR R0, [R1]     
  BX  LR  
	
all_off

	LDR R1, =LEDS
	AND R0, #0x00
	STR R0, [R1]
	BX LR
	

buttons

	LDR R1, =GPIO_PORTF_DATA_R
	LDR R0, [R1]
	AND R0, #0x11
	BX LR
	
	
;DELAYVAL		EQU 32000 ;16000*500 = 8000000 1.5 seconds per loop 	;Change to calculated value

DELAYVAL		EQU 2700000 ;16000*500 = 8000000 1.5 seconds per loop 	;Change to calculated value

delay
	LDR R0, =DELAYVAL 			; put the value of DIMSEC into R0
delay_loop
	SUBS R0, R0, #1				; R0 = R0 - 1
	BNE delay_loop 			; repeat until R0 == 0
	BX LR

; REMOVE BELOW LATER

blue_led_on
	LDR R1, =PF2
	MOV R0, #BLUE                   ; R0 = BLUE (blue LED on)
    STR R0, [R1]                    ; turn the blue LED on
    BX  LR
blue_led_off
	LDR R1, =PF2
	MOV R0, #0                      ; R0 = 0
    STR R0, [R1]                    ; turn the blue LED OFF
    BX  LR
	
;------------red_led_on-------
; Turn the red LED on
; Input: none
; Output: none
red_led_on
	LDR R1, =PF1
	MOV R0, #RED                    ; R0 = RED (red LED on)
    STR R0, [R1]                    ; turn the red LED on
    BX  LR
	
;------------red_led_off------
; Turn the red LED off
; Input: none
; Output: none
red_led_off
	LDR R1, =PF1
	MOV R0, #0                      ; R0 = 0
    STR R0, [R1]                    ; turn the red LED OFF
    BX  LR
	
;------------green_led_on------
; Turn the green LED on
; Input: none
; Output: none
green_led_on
	LDR R1, =PF3
	MOV R0, #GREEN                  ; R0 = GREEN (green LED on)
    STR R0, [R1]                    ; turn the green LED on
    BX  LR
	
;------------green_led_off-----
; Turn the green LED off
; Input: none
; Output: none
green_led_off
	LDR R1, =PF3
	MOV R0, #0                      ; R0 = 0
    STR R0, [R1]                    ; turn the green LED OFF
    BX  LR





	ALIGN
	END