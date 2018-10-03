// ----------------------------------------------------------------------------
// 
// TIMERmain.c
// 
// sets up periodic main loop timer and interrupt
// requires mainISR() to be the TIMER0A_Handler in Startup.s
// 
// ----------------------------------------------------------------------------

// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupTimerForMain(void);
void setupTimerInterrupt(void);
void clearTimerInterrupt(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup periodic main timer
void setupTimerForMain(void){
	
	// enable clock
	SYSCTL_RCGCTIMER |= 0x01;
	while((SYSCTL_PRTIMER & 0x01) != 0x01);

	// disable timer
	TIMER0_CTL &=~ 0x01;
	
	// select 16 bit timer mode
	TIMER0_CFG &=~ 0x07;
	TIMER0_CFG |= 0x04;
	
	// set timer type to periodic
	TIMER0_TAMR &=~ 0x03;
	TIMER0_TAMR |= 0x02;
	
	// set reload to 0.1 second
	TIMER0_TAILR &=~ 0xFFFF;
	TIMER0_TAPR &=~ 0xFF;
	TIMER0_TAILR |= 0x186A;
	TIMER0_TAPR |= 0xFF;
	
	// enable timer
	TIMER0_CTL |= 0x01;

}

// setup interrupts for periodic timer
void setupTimerInterrupt(void){
	
	// disable all interrupts
	DisableInterrupts();
	
	// enable interrupt
	TIMER0_IMR |= 0x01;
	
	// enable TIMER interrupts
	NVIC_EN0 |= 0x1 << 19;
	
	// set priority = 0 (highest)
	NVIC_PRI4 &=~ (unsigned)(0x7) << 29;
	NVIC_PRI4 |= (unsigned)(0x0) << 29;
	
	// set interrupt service routine
	// done in startup.s
	
	// clear any interrupts
	clearTimerInterrupt();
	
	// enable all interrupts
	EnableInterrupts();
	
}


// clears interrupt
void clearTimerInterrupt(void){
	TIMER0_ICR |= 0x01;
}

