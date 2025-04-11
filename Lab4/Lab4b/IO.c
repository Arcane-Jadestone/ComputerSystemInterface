#include "IO.h"
#include "tm4c123gh6pm.h"

//Place the definition for bit specific addressing here.  
//The below example is for PortA pin 5
//#define PA5 (*((volatile uint32_t *)0x40004080))

//Place your pushbutton and led functions here

//Turns off all LEDs
void LEDs_off(void){
	
}

//Should return the button states
uint32_t pushbuttons(void){
	
	// Set to 42 because the compiler expects a return value.  
	// Your function only needs to return a value denoting the button states
	// rather than the answer to life, the universe, and everything...
	return 42;
}

