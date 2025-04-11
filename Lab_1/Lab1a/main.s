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
	LDR R0, =GPIO_PORTF_DATA_R
	LDR R1, [R0]
	AND R1, #0x11
	CMP R1, #0x01
	BEQ dim 		;SW1 is pressed
	CMP R1, #0x10
	BEQ toggle 		;Both pressed
	CMP R1, #0x00   
	BEQ routine 	;Both pressed
	BL blue_led_on	;No buttons pressed
	B   loop
	
dim
	BL blue_led_on
	BL delay_dim
	BL blue_led_off
	BL delay_dim
	B loop
	
toggle
	BL blue_led_on
	BL delay
	BL blue_led_off
	BL green_led_on
	BL delay
	BL green_led_off
	B loop
	
routine
	BL blue_led_off
	BL red_led_on
	BL delay
	BL delay
	BL red_led_off
	BL green_led_on
	BL delay
	BL delay
	BL green_led_off
	BL blue_led_on
	BL delay
	BL delay
	
	B loop
	
	ALIGN        ; make sure the end of this section is aligned
	END          ; end of file