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

char Keypad_GetKey(void);
void LED_red(void);
void LED_green(void);
void InitConsole(void);
void DelayMs(uint32_t);
void ServoInit(void);
void Servo_Lock(void);
void Servo_Unlock(void);

int main(){
	
		InitConsole();
		ServoInit();
	
	//NOTES: keypad is working but servo is not. servoinit code is at bottom and the math for locking and unlocking are in respective functions aswell.
	
		UARTprintf("good\n");
	
		SYSCTL_RCGCGPIO_R |= 0x02;           // enable clock Port B
    while((SYSCTL_PRGPIO_R & 0x02)==0);  // wait for Port B ready
	
		GPIO_PORTB_LOCK_R = 0x4C4F434B;
		GPIO_PORTB_CR_R = 0xFF;

    GPIO_PORTB_DIR_R = 0x0F;             // PA0-3 rows as output, PA4-7 cols as input
    GPIO_PORTB_DEN_R = 0xFF;             // Enable digital on PA0-7
    GPIO_PORTB_PUR_R = 0xF0;             // Enable pull-ups on PA4-7
    GPIO_PORTB_AFSEL_R = 0;
    GPIO_PORTB_AMSEL_R = 0;
	
		//testing LED
		SYSCTL_RCGCGPIO_R |= 0x20;            // Enable clock for Port F
    while((SYSCTL_PRGPIO_R & 0x20)==0);   // Wait for it to stabilize

    GPIO_PORTF_LOCK_R = 0x4C4F434B;       // Unlock PF0 (not used here but good practice)
    GPIO_PORTF_CR_R |= 0x0E;              // Allow changes to PF1–PF3

    GPIO_PORTF_DIR_R |= 0x0E;             // Set PF1, PF2, PF3 as outputs
    GPIO_PORTF_DEN_R |= 0x0E;             // Digital enable for PF1–PF3
    GPIO_PORTF_AMSEL_R &= ~0x0E;          // Disable analog on PF1–PF3
    GPIO_PORTF_AFSEL_R &= ~0x0E;          // No alternate function
    GPIO_PORTF_PCTL_R &= ~0x0000FFF0;     // Clear PCTL for PF1–PF3
		
		const char pw[] = {'2','3','A','5'}; //pw abbreviated for password
    char entered[4];
    int index = 0;
		UARTprintf("test");
		
		while(1){
        char key = Keypad_GetKey();
			
        if (key) {
						UARTprintf("Key pressed: %c\n", key);
            entered[index++] = key;

            if(index == 4){
                if(entered[0] == pw[0] && entered[1] == pw[1] &&
                   entered[2] == pw[2] && entered[3] == pw[3]){
                   LED_green();
										Servo_Unlock();
                    //Servo_SetAngle(90);  // unlock position
                } else {
										LED_red();
										Servo_Lock();
                    //Servo_SetAngle(0);   // lock position
                }
                index = 0; //reset password if wrong
            }
        }
    }
}
		
		

char Keypad_GetKey(void){
    const char keymap[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };
	
		//the keypad sets a button to low when pressed, this loop scans through and finds where it is and returns the char
    for (int row = 0; row < 4; row++) {
        // Step 1: Make all rows HIGH
        GPIO_PORTB_DATA_R |= 0x0F;

        // Step 2: Pull current row LOW
        GPIO_PORTB_DATA_R &= ~(1 << row);

        DelayMs(1);  // allow pins to settle

        // Step 3: Read column pins
        for (int col = 0; col < 4; col++) {
            if ((GPIO_PORTB_DATA_R & (1 << (col + 4))) == 0) {
                DelayMs(20);  // debounce
                while ((GPIO_PORTB_DATA_R & (1 << (col + 4))) == 0);  // wait for release
                return keymap[row][col];
            }
        }
    }

    return 0;
}

void LED_red(void){
		GPIO_PORTF_DATA_R=0x02;
}

void LED_green(void){
		GPIO_PORTF_DATA_R=0x08;
}

void DelayMs(uint32_t ms){
    // 3 clock cycles per loop — use (ms * (SysCtlClock / 3000))
    SysCtlDelay(ms * (SysCtlClockGet() / 3000));
}

void Servo_Lock(void){   // 0 degree = 1.0 ms pulse
		UARTprintf("locking");
    TIMER0_TAMATCHR_R = 1600000 - 160000;
}

void Servo_Unlock(void){ // 90 degree = 1.5 ms pulse
		UARTprintf("unlocking");
    TIMER0_TAMATCHR_R = 1600000 - 240000;
}

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

void ServoInit(void){
    SYSCTL_RCGCGPIO_R |= 0x04;         // Enable clock to Port C
    SYSCTL_RCGCTIMER_R |= 0x01;        // Enable Timer0

    while((SYSCTL_PRGPIO_R & 0x04)==0);  // Wait for Port C ready

    GPIO_PORTC_AFSEL_R |= (1<<4);       // Enable alternate function on PC4
    GPIO_PORTC_PCTL_R &= ~0x000F0000;   // Clear PC4 bits
    GPIO_PORTC_PCTL_R |= 0x00070000;    // PC4 = T0CCP0 (Timer0A compare output)
    GPIO_PORTC_DEN_R |= (1<<4);         // Digital enable
    GPIO_PORTC_DIR_R |= (1<<4);         // Output

    TIMER0_CTL_R &= ~0x01;              // Disable Timer0A
    TIMER0_CFG_R = 0x00000004;          // 16-bit timer
    TIMER0_TAMR_R = 0x0000000A;         // PWM mode: periodic, count-down
    TIMER0_TAILR_R = 1600000 - 1;       // 20 ms period (16 MHz clock)
    TIMER0_TAMATCHR_R = 1600000 - 160000; // Start at 0° position (1.0 ms pulse)
    TIMER0_CTL_R |= 0x01;               // Enable Timer0A
}

//EOF