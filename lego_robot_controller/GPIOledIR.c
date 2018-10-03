// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupGPIOforLED(void);
void setLED(int state);
void enableLED(void);
void disableLED(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup GPIO for IR LED
void setupGPIOforLED(void){
	
	// enable clock
	SYSCTL_RCGCGPIO |= 0x02;
	while((SYSCTL_PRGPIO & 0x02) != 0x02);
	
	char PIN = 0x40;
	
	// set direction as output
	GPIO_PORTB_DIR |= PIN;
	
	// set alternate function
	GPIO_PORTB_AFSEL &=~ PIN;
	
	// digital enable
	GPIO_PORTB_DEN |= PIN;
	
	// select alternate function
	GPIO_PORTB_PCTL &=~ 0x0F00000;
	
}

// set state of IR LED
void setLED(int state){
	if(state)
		enableLED();
	else
		disableLED();
}

// turn on IR LED
void enableLED(void){
	GPIO_PORTB_DATA |= 0x40;
}

// turn off IR LED
void disableLED(void){
	GPIO_PORTB_DATA &=~ 0x40;
}

