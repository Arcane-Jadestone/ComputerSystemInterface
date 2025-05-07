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
//void ServoInit(void);
//void Servo_Lock(void);
//void Servo_Unlock(void);
void PortF_Init(void);
void Stepper_Unlock(void);
void Stepper_Lock(void);
void Stepper_Move(void);


int main(){
	
		InitConsole();
		PortF_Init();
	
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
		int state = 0; //1 denotes unlocked 0 is locked
		//Servo_Lock();
		//Servo_Unlock();
		UARTprintf("test\n");
		
		while(1){
        char key = Keypad_GetKey();
			
        if (key) {
						UARTprintf("Key pressed: %c\n", key);
            entered[index++] = key;

            if(index == 4){
                if(entered[0] == pw[0] && entered[1] == pw[1] &&
                   entered[2] == pw[2] && entered[3] == pw[3]){
                   
										if (state == 0) { //unlock statement
											LED_green();
											for (int i=0; i<8; i++){
												Stepper_Lock();
												DelayMs(10);
											}
											state = 1;
										}
                } 
								else {
										if (state == 1){
											LED_red();
											for (int i=0; i<8; i++){
												Stepper_Lock();
												DelayMs(10);
											}
											state = 0;
									}
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
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = 16000 - 1;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 0x5;

    for(uint32_t i = 0; i < ms; i++){
        while((NVIC_ST_CTRL_R & 0x10000) == 0);
    }

    NVIC_ST_CTRL_R = 0;
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

void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x20;
    while((SYSCTL_PRGPIO_R & 0x20)==0);

    GPIO_PORTF_DIR_R |= 0x0F;   // PF0–PF3 = output
    GPIO_PORTF_DEN_R |= 0x0F;
}

void Stepper_Step(uint8_t step){
    const uint8_t sequence[4] = {
        0x03, // IN1 + IN2
        0x06, // IN2 + IN3
        0x0C, // IN3 + IN4
        0x09  // IN4 + IN1
    };

    GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0F) | sequence[step % 4];
}

void Stepper_Unlock(void){
    for (int i = 0; i < 128; i++) {
        Stepper_Step(i % 4);
        DelayMs(5);
    }
}

void Stepper_Lock(void){
    for (int i = 0; i < 128; i++) {
        Stepper_Step(3 - (i % 4)); // reverse
        DelayMs(10);
    }
}

void Stepper_Move(void){
    uint8_t current_step = 0;

    for (int i = 0; i < 256; i++) {
        Stepper_Step(current_step);
        current_step = (current_step + 1) % 4;
        DelayMs(3);  // You can fine-tune this for speed
    }
}

//EOF
