

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT Start
	IMPORT PortF_Init
	IMPORT LEDs_on
	IMPORT red_on
	IMPORT blue_on
	IMPORT green_toggle
	IMPORT all_off
	IMPORT buttons
	IMPORT delay
	
	IMPORT   blue_led_on
	IMPORT   blue_led_off
	IMPORT   red_led_on
	IMPORT   red_led_off
	IMPORT   green_led_on
	IMPORT   green_led_off

	
		
	
	;Be sure to import your created I/O functions


		
Start
	BL PortF_Init
	
	
loop
	;Used by Part 1 to prove PortF_Init is correct
	
	; Part 3
	; Call delay
	; Read the state of the buttons
	; None -> Call toggle
	; SW1 -> Call blue_on
	; SW2 -> Call red_on
	; Both -> Call toggle, blue_on, red_on
	
	BL delay
	BL buttons
	MOV R4, R0

	CMP R4, #0x11	;No buttons pressed?
	BNE checkSW1
	BL green_toggle
	B loop
checkSW1
	CMP R4, #0x01 	;SW1 pressed?
	BNE checkSW2
	BL blue_on
	B loop
checkSW2
	CMP R4, #0x10 	;SW2 pressed?
	BNE bothSW
	BL red_on
	B loop

bothSW
	BL red_on
	BL blue_on
	BL green_toggle

	B loop

end
	ALIGN
	END