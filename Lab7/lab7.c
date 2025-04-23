#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c123gh6pm.h"

// In this file you should insert your code from the previous lab


// Initialize UART0
// Assumes 16 MHz clock, creates 9600 baud rate
void InitConsole(void){

}

// Should block until the user presses a key and then returns that value
// Returns a newline until it has been correctly implemented
unsigned char myGetChar(void){
		char retchar = '\n';
	
		return retchar;
}
