// ----------------------------------------------------------------------------
// 
// GPIOled.c
// 
// sets up the GPIO port for the onboard blue LED
// provides methods to enable or disable the LED
// 
// ----------------------------------------------------------------------------

// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupGPIOforOnboardLED(void);
void enableOnboardLED(void);
void disableOnboardLED(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup GPIO for onboard LED port F pin 2
void setupGPIOforOnboardLED(void){
	
	// enable clock
	SYSCTL_RCGCGPIO |= 0x20;
	while((SYSCTL_PRGPIO & 0x20) != 0x20);
	
	char PIN = 0x04;
	
	// set direction as output
	GPIO_PORTF_DIR |= PIN;
	
	// set alternate function
	GPIO_PORTF_AFSEL &=~ PIN;
	
	// digital enable
	GPIO_PORTF_DEN |= PIN;
	
	// select alternate function
	GPIO_PORTF_PCTL &=~ 0x0F00000;
	
}

// turn on Onboard LED
void enableOnboardLED(void){
	GPIO_PORTF_DATA |= 0x04;
}

// turn off Onboard LED
void disableOnboardLED(void){
	GPIO_PORTF_DATA &=~ 0x04;
}

