#include "SysTick.h"
#include "tm4c123gh6pm.h"
#include "stdint.h"

// Used in part 3
volatile uint32_t g_handler_calls;

//Initialize Systick
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;  // Stops sysTick
	NVIC_ST_CTRL_R  = NVIC_ST_CTRL_R | 0x05; // Resarts sysTick
}


// Configure SysTick to generate an interrupt every 20ms 
// Assume 16 MHz clock or a period of  1/(16*10^6) s = 62.5 ns
void SysTick_Init_Interrupts(void){
	g_handler_calls = 0; //Initialize counter as 0
<<<<<<< Updated upstream
=======
	
	NVIC_ST_CTRL_R=0; //disables SysTick for setup
	NVIC_ST_RELOAD_R = 319999; //20ms interval
	NVIC_ST_CURRENT_R = 0; //clear current value
	NVIC_ST_CTRL_R = 0x07; //enable systick
>>>>>>> Stashed changes

}


// Clock speed is 16 MHz
static void SysTick_Delay1ms_16MHz(void){
	//Use the Systick Timer to generate a 1ms delay	
	// Choose the number of clock ticks to wait
	NVIC_ST_RELOAD_R = (16000)-1; 
	
	NVIC_ST_CURRENT_R = 0; // Any value written to write clears it
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // Wait for count flag
}


// Write code to generate a 2 sec delay
// Your code should call SysTick_Delay1ms()
void SysTick_Delay2s_16MHz(void){
	int i;
	for (i = 0; i < 2000; i++){
		SysTick_Delay1ms_16MHz();
	}
	
} 


// Write code to generate 1ms delay assuming a clock speed of 50MHz
static void SysTick_Delay1ms_50MHz(void){
	//Use the Systick Timer to generate a 1ms delay
	// Choose the number of clock ticks to wait
	NVIC_ST_RELOAD_R = (50000)-1; 
	
	NVIC_ST_CURRENT_R = 0; // Any value written to write clears it
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // Wait for count flag
}


// Write code to generate a 2 sec delay when the clock speed is 50MHz
// Your code should call SysTick_Delay1ms_50MHz()
void SysTick_Delay2s_50MHz(void){
	int i;
	for (i = 0; i < 2000; i++){
		SysTick_Delay1ms_50MHz();
	}
}
	


// Toggle green LED every 2 seconds
// Toggle blue LED every 1 second
void SysTick_Handler(void){
	
	g_handler_calls++;
<<<<<<< Updated upstream
=======

	if (g_handler_calls % 50 == 0){
		GPIO_PORTF_DATA_R ^= 0x4; //toggles blue at 1ms
	}
	
	if (g_handler_calls % 100 == 0){
		GPIO_PORTF_DATA_R ^= 0x8;
	}
>>>>>>> Stashed changes
}
