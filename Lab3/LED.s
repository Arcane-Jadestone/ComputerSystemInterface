GPIO_PORTF_DATA_R  EQU 0x400253FC
RED       		   EQU 0x02
BLUE      		   EQU 0x04
GREEN     		   EQU 0x08
PF1				   EQU 0x40025008
PF2				   EQU 0x40025010
PF3				   EQU 0x40025020

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	;Be sure to export created functions
	EXPORT blue_led_on
	EXPORT blue_led_off
	EXPORT red_led_on
	EXPORT red_led_off
	EXPORT green_led_on
	EXPORT green_led_off
	EXPORT leds_off
	
		
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


leds_off
	LDR R1, =GPIO_PORTF_DATA_R
	LDR R0, [R1]
	BIC R0, #0x0E
	STR R0, [R1]
	BX LR
	
		
		
	ALIGN
	END