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
		SYSCTL_RCGC1_R |= 0x1;	// Enable UART0
		SYSCTL_RCGC2_R |= 0x1;	// Enable PortA clk
		UART0_CTL_R &= ~0x1;		// Disable UART0
	
		// Set the baud rate - need to change for Part 1
		UART0_IBRD_R = 104 ;//16000000 / (9600*16);
		UART0_FBRD_R = 11; //64/3 -> 11
	
		UART0_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
	
		// Uncomment these lines in a friendly manner
		GPIO_PORTA_AFSEL_R |= 0x03;	// Enable alt function on PA1, PA0
		GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFF00FFFF) + 0x00220000;	// Set PCT to choose UART alt function on PortA
		GPIO_PORTA_AMSEL_R &= ~0x03;	// Clear AMSEL to disable analog
		GPIO_PORTA_DEN_R |= 0x03;		// Enable PA1, PA0
	
	
		UART0_CTL_R |= 0x1;  // Enable UART0
	
		// Needed for the UARTprintf() functions to work correctly.  
		// Intentially broken to remove normal behavior.
		// UART0 will not work without correctly completing the above initialization.
    UARTStdioConfig(0, 9600, 16000000);
}

// Should block until the user presses a key and then returns that value
// Returns a newline until it has been correctly implemented
char myGetChar(void){			
	while((UART0_FR_R & 0x0010) != 0);
	return ((char) (UART0_DR_R & 0xFF));
}
