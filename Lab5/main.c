#include "PortF.h"
#include "SysTick.h"
#include "PLL.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

// Change this number to the current part of the lab you are working on
#define LAB_PART 3

int main(){
	PortF_Init();
	
	if(LAB_PART==1){
		SysTick_Init();
		while(1){
			// LED will only toggle every second while a button is held down
			while(pushbuttons() != 0x11){
				SysTick_Delay2s_16MHz();
				GPIO_PORTF_DATA_R ^= 0x4; // Toggle blue
			}
			GPIO_PORTF_DATA_R &= ~0x4;	// Turn off when not testing
		}
	}
	
	else if(LAB_PART==2){
		SysTick_Init();
		PLL_Init();
		while(1){
			// LED will only toggle every second while a button is held down
			while(pushbuttons() != 0x11){
				SysTick_Delay2s_50MHz();
				GPIO_PORTF_DATA_R ^= 0x2; // Toggle red
			}
			GPIO_PORTF_DATA_R &= ~0x2;	// Turn off when not testing
		}
	}
	
	else if(LAB_PART==3){
		// Will hang until a button is pressed.  Makes it easier to time yourself
		while(pushbuttons() == 0x11){}
		SysTick_Init_Interrupts();
		while(1){
			// Do nothing except wait for a SysTick Interrupt
		}
	}
	
	// Shouldn't reach unless you set an incorrect value for LAB_PART
	else return 0;
}
