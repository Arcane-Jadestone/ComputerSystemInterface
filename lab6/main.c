// The following UART signals are configured only for displaying console
// messages for this example.  These are not required for operation of the
// ADC.
// - UART0 peripheral
// - GPIO Port A peripheral (for UART0 pins)
// - UART0RX - PA0
// - UART0TX - PA1
//
//
// NOTE: The internal temperature sensor is not calibrated.  This example
// just takes the raw temperature sensor sample and converts it using the
// equation found in the datasheet.
//
//*****************************************************************************


//Includes
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"		// TI provided UART I/O function library
#include "inc/tm4c123gh6pm.h"


// Instructor Provided Prototypes
extern void WaitForInterrupt(void); // Location in startup.s
uint32_t ToTime(char* buff, uint32_t len);
void SysTickInit(void);

// Student Written Functions
char myGetChar(void);
void myPutChar(unsigned char c);
void PrintTime(void);
void InitConsole(void);

// Global counter for number of seconds that have elapsed
volatile uint32_t g_seconds;

int main(void)
{   	
		char input_buffer[10];
		char choice='.';
		memset(input_buffer,'.',10*sizeof(char));
		
		// Initialize UART
    InitConsole();
		
		// Message to confirm correct UART initialization
		UARTprintf("\nIf you are seeing this then you have completed Part 1!\n");
	
		while(choice != '0' && choice != '1'){
			UARTprintf("Press 0 or 1 to select mode of operation:\n");
			UARTprintf("0) Start at 0\n");
			UARTprintf("1) Specify Time\n");
			choice = myGetChar();	
			if(choice != '0' && choice != '1'){
				UARTprintf("\nInvalid input %c!\n\n",choice);
			}
		}
		
		UARTprintf("\nYou have chosen option %c! \nPart 2 complete!\n\n",choice);
		
		if(choice == '1'){
			// Enter a number of seconds
			UARTprintf("\nEnter the desired time in seconds.\n");
			UARTprintf("Invalid inputs will result in 0!\n");
			UARTgets(input_buffer, 10);
			g_seconds = ToTime(input_buffer,10);
		}
		else{
				UARTprintf("Using default time of 0 seconds\n");
				g_seconds=0;
		}
		
		UARTprintf("Starting at %i seconds.\n",g_seconds);
	   
		// Initialize SysTick with interrupts
		SysTickInit();		
		
    while(1)
    {
				WaitForInterrupt();
				PrintTime();
    }
}
//***************************************************************************
//NOTE: Fill in The Following Functions
// Student: You gave us some of the functions...
//***************************************************************************

// Change this function so that time is displayed as a 24-hour clock (hh:mm:ss)
// Each new call should overwrite the previous time in the terminal rather than
//   printing each new time on the next line.


void PrintTime(void){
	
		// Comment out this line when you write your own implementation
		UARTprintf("%i\n",g_seconds);
		
		// Use myPutChar(char c) to output each character in the desired format
		// Be careful not to mangle the value of g_seconds
		uint32_t hr,min,sec;

		hr = g_seconds / 3600;
		min = (g_seconds % 3600) / 60;
		sec = g_seconds % 60;

		myPutChar('\r');
	
		myPutChar('0' + hr / 10); //tens place
    		myPutChar('0' + hr % 10); //ones

		myPutChar(':');

		myPutChar('0' + min % 10);
		myPutChar('0' + min / 10);

		myPutChar(':');

		myPutChar('0' + sec % 10);
		myPutChar('0' + sec / 10);		
		

		// g_seconds contains the current time in seconds
	
		// Look into carriage returns (\r) vs. new lines (\n) to understand how to
		//   get the new time to overwrite the old time in the display.
}


// Should block until the user presses a key and then returns that value
// Returns a newline until it has been correctly implemented
/*
unsigned char myGetChar(void){
		char retchar = '\n';
	
		// Wait for the proper FIFO flag to be 0
		
		// Return the received value
    //retchar = ;
		return retchar;
}
*/

// Should output the given char c to the terminal
/*
void myPutChar(unsigned char c){
	
		// Wait for the proper Transmit FIFO flag to be 0
	
		// Write the character to the data register 
		//UART0_DR_R = ;
}
*/


 // Function Given in Lab 6 slides
char myGetChar(void){							//UART_InChar: Wait for input then return it
	while((UART0_FR_R & 0x0010) != 0);
	return ((char) (UART0_DR_R & 0xFF));
}
void myPutChar(unsigned char data){ 			//UART_OutChar:Wait for empty buffer, Then output
	while((UART0_FR_R & 0x0020) != 0);
	UART0_DR_R = data;
}
char UART_InCharNonBlocking(void){  //UART_InCharNonBlocking: See if input = 0 or N/A
	if ((UART0_FR_R & UART_FR_RXFE) == 0){
		return ((char) (UART0_DR_R & 0xFF));
	} else {
		return 0;
	}
}


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

//***************************************************************************
//NOTE: The following functions do not require any modifications
//***************************************************************************
// Initialize SysTick Interrupts, interrupt every 1 second
void SysTickInit(void){
		NVIC_ST_CTRL_R=0;
		NVIC_ST_RELOAD_R = 16000000-1;
		NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
		NVIC_ST_CTRL_R = 0x07;
}

// Updates the second count 
void SysTick_Handler(void){
		g_seconds++;
	
		if(g_seconds>=86400){ // Number of seconds in a day
			g_seconds=0;
		}
}

// Helper function to convert time from char array to uint32_t
uint32_t ToTime(char* buff, uint32_t len){
		int i;
		uint32_t ret=0;
		for(i=0; i<len; i++){
			if(buff[i]<'0' || buff[i]>'9'){break;}
			else{
				ret *= 10;
				ret += buff[i] - '0';
			}
		}
		return ret;
}

//EOF
