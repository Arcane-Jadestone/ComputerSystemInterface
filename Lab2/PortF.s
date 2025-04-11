; Credit: Based on software by Valvano

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
RED       		   EQU 0x02
BLUE      		   EQU 0x04
GREEN     		   EQU 0x08
PF1				   EQU 0x40025008
PF2				   EQU 0x40025010
PF3				   EQU 0x40025020

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT   PortF_Init
	EXPORT   blue_led_on
	EXPORT   blue_led_off
	EXPORT   red_led_on
	EXPORT   red_led_off
	EXPORT   green_led_on
	EXPORT   green_led_off

PortF_Init
    LDR R1, =SYSCTL_RCGC2_R         ; 1) activate clock for Port F
    LDR R0, [R1]                 
    ORR R0, R0, #0x20               ; set bit 5 to turn on clock
    STR R0, [R1]                  
    NOP
    NOP                             ; allow time for clock to finish
    LDR R1, =GPIO_PORTF_LOCK_R      ; 2) unlock the lock register
    LDR R0, =0x4C4F434B             ; unlock GPIO Port F Commit Register
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_CR_R        ; enable commit for Port F
    MOV R0, #0xFF                   ; 1 means allow access
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_AMSEL_R     ; 3) disable analog functionality
    MOV R0, #0                      ; 0 means analog is off
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_PCTL_R      ; 4) configure as GPIO
    MOV R0, #0x00000000             ; 0 means configure Port F as GPIO
    STR R0, [R1]                  
    LDR R1, =GPIO_PORTF_DIR_R       ; 5) set direction register
	LDR R0, [R1]
	MOV R0, #0x0E
	STR R0, [R1]                 
    LDR R1, =GPIO_PORTF_AFSEL_R     ; 6) regular port function
    MOV R0, #0                      ; 0 means disable alternate function 
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_PUR_R       ; pull-up resistors for PF4,PF0
    MOV R0, #0x11                   ; enable weak pull-up on PF0 and PF4
    STR R0, [R1]              
    LDR R1, =GPIO_PORTF_DEN_R       ; 7) enable Port F digital port
    MOV R0, #0xFF                   ; 1 means enable digital I/O
    STR R0, [R1]                   
    BX  LR 
	
;------------blue_led_on------
; Turn the blue LED on
; Input: none
; Output: none
blue_led_on
	LDR R1, =PF2
	MOV R0, #BLUE                   ; R0 = BLUE (blue LED on)
    STR R0, [R1]                    ; turn the blue LED on
    BX  LR
	
;------------blue_led_off-----
; Turn the blue LED off
; Input: none
; Output: none
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
	
	ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
