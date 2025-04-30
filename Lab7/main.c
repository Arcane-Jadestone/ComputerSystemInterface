#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c123gh6pm.h"
#include "lab7.h" // UART functions from the previous lab
#include "systick.h" // Delay function


//Prototypes from Lab 6 (located in lab7.c)
void InitConsole(void);
char myGetChar(void);

//Student Written Functions
void ADC0_Init(void);
uint32_t Get_TempC(void);
void Print_Temps (uint32_t TempC);
uint32_t ADC0_In(void);
uint32_t Convert_Raw_To_V(uint32_t raw);
uint32_t Convert_V_To_C(uint32_t v);
uint32_t Convert_C_To_F ( uint32_t TempC);


int main(void)
{
		unsigned char keystroke; // Can be used in Part 3
	
		// Initialize UART for 16 MHz clock and 9600 baud
    InitConsole();
	
		// Initialize SysTick
		SysTick_Init();
    
		//Initialize ADC0 to take temperature readings
		ADC0_Init();
    
		
		UARTprintf("Initialization Complete...\n");

    // Continually read and display the temperature
    while(1)
    {	
        Print_Temps (Get_TempC());
				Delay_100ms(); // Included to make console output less frequent
    }
}


 /****************************************************
 *  NOTE: You need to complete the following functions
 *****************************************************/

void ADC0_Init(void)
{
	 /*
	You will need to initialize PE4 for the termperature sensor:
	  
    SYSCTL_RCGCGPIO_R X= 0xXX;    // 1) activate clock for Port E and allow time to stabilize
	  while((SYSCTL_RCGCGPIO_R & 0xXX)==0){}	
    GPIO_PORTE_DIR_R X= 0xXX;  // 2) make PE4 input
    GPIO_PORTE_AFSEL_R X= 0xXX; // 3) enable alternate function on PE4
    GPIO_PORTE_DEN_R X= 0xXX;  // 4) disable digital I/O on PE4
    GPIO_PORTE_AMSEL_R X= 0xXX; // 5) enable analog functionality on PE4
		*/
	
	SYSCTL_RCGCGPIO_R |= 0x10;    // 1) activate clock for Port E and allow time to stabilize
	while((SYSCTL_RCGCGPIO_R & 0x10)==0){}	
  GPIO_PORTE_DIR_R &= 0xEF;  // 2) make PE4 input
  GPIO_PORTE_AFSEL_R |= 0x10; // 3) enable alternate function on PE4
  GPIO_PORTE_DEN_R &= 0xEF;  // 4) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x10; // 5) enable analog functionality on PE4
	SYSCTL_RCGCADC_R |= 0x0001;   // 6) activate ADC0 and allow time to stabilize
	while((SYSCTL_RCGCADC_R&0x1)==0){}
	
		
	/****************************************** 
	 * Uncomment and complete this line
	 ******************************************/
  ADC0_PC_R = 0x5 ;								  // 7) configure for 500K samples/sec
	
		
  ADC0_SSPRI_R = 0x0123;        // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 10) seq3 is software trigger
		
	ADC0_SSMUX3_R = 9;            // 11) Select channel 9 corresponding to PE4.
  ADC0_SSCTL3_R = 0x06;         // 12) Set IE0 and END0 (do NOT set TS0).
			
  ADC0_ACTSS_R |= 0x0008;       // 13) enable sample sequencer 3 before we sample.
}

// Returns the temperature in Celsius
// You should call ADC0_In, convertRawToVolts(), and convertVoltsToCelsius()
uint32_t Get_TempC(void){
	int i;
	uint32_t result=0;
	
	// Read raw result from ADC0
	// Try averaging multiple readings together to produce a single output
	for (i = 0; i < 64; i++){
		result += ADC0_In();
	}
	result /= 64;
	
	// For debugging, hard code the ADC output as 1746.  This will ultimately produce a temperature of 42C.
	result = 1746;
	
	// Convert raw ADC value to millivolts
	
	
	// The hardcoded raw value of 1746 should convert to roughly 1407 mV.  Use the debugger to confirm 
	//   Convert_Raw_To_V() has returned the correct value.
	
	
	// Convert voltage to Celsius
	
	
	// Return temperature in Celsius
	return result;
}

// Take a reading from the ADC0 Sample Sequencer 3 and return the raw value
uint32_t ADC0_In(void){
	uint32_t v=0;	
	
	// 1) initiate SS3

  // 2) wait for conversion done

  // 3) read result

  // 4) acknowledge completion
		
	return v;
}

uint32_t Convert_Raw_To_V(uint32_t raw){
	uint32_t result;
	
	// Convert raw ADC value to mV.
	// voltage = max_possible_voltage * ADC_result / max_possible_ADC_value
	result = raw;
	
	
	return result;
}

// You are not required to edit this.
uint32_t Convert_V_To_C(uint32_t v){
	uint32_t result;
	
	result = 100 - ((v * 45) / 1000);
	
	return result;
}

// Convert a temperature from Celsius to Fahrenheit
uint32_t Convert_C_To_F( uint32_t TempC)
{
	uint32_t TempF;
	
	// complete this equation
	TempF= ((TempC*9)/5) + 32;

	return TempF;
}

// Prints the temperature in Celsius
// Should later also convert to and print in Fahrenheit
void Print_Temps (uint32_t TempC)
{
	uint32_t TempF = 0;
	// UARTprintf() works in exactly the same way as normal printf()
	UARTprintf("Temperature = %3d*C\n", TempC);
	
	TempF = Convert_C_To_F(TempC); //converts to fahrenheit
	UARTprintf("Temperature = %3d*F\n", TempF);
}




//EOF
