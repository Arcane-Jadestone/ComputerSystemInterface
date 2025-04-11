

	AREA    |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT Start
	IMPORT PortF_Init
	IMPORT LEDs_on
	
	
	;Be sure to import your created I/O functions


		
Start
	BL PortF_Init
loop
	;Used by Part 1 to prove PortF_Init is correct
	BL LEDs_on ;Comment out for later parts
	
	
	
	; Part 3
	; Call delay
	; Read the state of the buttons
	; None -> Call toggle
	; SW1 -> Call blue_on
	; SW2 -> Call red_on
	; Both -> Call toggle, blue_on, red_on
	


	B loop

	ALIGN
	END