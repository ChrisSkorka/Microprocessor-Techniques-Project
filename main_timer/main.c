// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern void setupUARTforUSB(void);
extern void printString(char* str);
extern void setupTimerForMain(void);
extern void setupTimerInterrupt(void);
extern void clearTimerInterrupt(void);

// PROTOTYPES
// ----------------------------------------------------------------------------
int main(void);
void mainISR(void);
void mainLoop(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// main
int main(void){	  

	setupUARTforUSB();
	setupTimerForMain();
	setupTimerInterrupt();
	
	printString("Ready ");
	
	while(1);
}

// main program run periodically
void mainISR(void){
	
	// run main loop
	mainLoop();
	
	// clear interrupt status
	clearTimerInterrupt();
	
}

// loop run periodically
void mainLoop(void){
	
	printString("Now ");
	
}
