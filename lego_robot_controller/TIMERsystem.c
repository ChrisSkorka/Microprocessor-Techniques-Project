// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupSysTick(void);
void wait(int cycles);
void waitUS(int us);

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup Systick timer
void setupSysTick(void){
	
	// disable timer
	NVIC_ST_CTRL &=~ 0x01;
	
	// set reload
	NVIC_ST_RELOAD = 0xFFFFFF;
	NVIC_ST_CURRENT = 0x00;
	
	// set source
	NVIC_ST_CTRL |= 0x04;
	
	// disable interrupts
	NVIC_ST_CTRL &=~ 0x02;
	
	// enable timer
	NVIC_ST_CTRL |= 0x01;
	
}

// wait the given micro seconds
void waitUS(int us){
	wait(us * 16);
	// wait(us * 16 * 10000);
}

// wait for the given number of cycles pass
void wait(int cycles){
	
	// now
	int start = NVIC_ST_CURRENT;
	
	// wait for timer to timeup
	while(((start - NVIC_ST_CURRENT) & 0xFFFFFF) < cycles);
}
