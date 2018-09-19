// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupTimerForMain(void);
void setupTimerInterrupt(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup periodic main timer
void setupTimerForMain(void){
	
	// enable clock
	SYSCTL_RCGCTIMER |= 0x01;
	while((SYSCTL_PRTIMER & 0x01) != 0x01);

	//disable timer
	TIMER0_CTL &=~ 0x01;
	
	// select 16 bit timer mode
	TIMER0_CFG &=~ 0x07;
	TIMER0_CFG |= 0x04;
	
	// set timer type to one shot
	TIMER0_TAMR &=~ 0x03;
	TIMER0_TAMR |= 0x01;

}

// setup interrupts for periodic timer
void setupTimerInterrupt(void){
	
}

