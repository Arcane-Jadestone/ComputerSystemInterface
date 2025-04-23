#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c123gh6pm.h"
#include "systick.h"


void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0x05;
}

void Delay_100ms(void){
	NVIC_ST_RELOAD_R = 1600000-1;
	NVIC_ST_CURRENT_R = 0;
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // Wait for count flag
}
