// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern void printString(char* str);
extern void setupTimerForMain(void);
extern void setupTimerInterrupt(void);

// PROTOTYPES
// ----------------------------------------------------------------------------
int main(void);
void mainISR(void);
void mainLoop(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// main
int main(void){	  

	setupTimerForMain();
	setupTimerInterrupt();
	
	while(1);
}

// main program run periodically
void mainISR(void){
	
	// run main loop
	mainLoop();
	
	// clear interrupt status
	// TODO
	
}

// loop run periodically
void mainLoop(void){
	
	printString("Now ");
	
}
