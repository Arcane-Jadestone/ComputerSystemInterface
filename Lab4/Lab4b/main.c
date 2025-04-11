#include "Inits.h"
#include "IO.h"
#include "stdint.h"

int main(){

	// Must be volatile to guarantee a new button state being retrieved
	volatile uint32_t buttons;

	PortD_Init();
	PortE_Init();
	
	// You will need to write more functions than just these
	LEDs_off();
	buttons=pushbuttons();
	
	return 0;
}

