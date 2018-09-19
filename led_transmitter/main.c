#include "registers.h"

extern void setupGPIOforLED(void);
extern void setupTimerForIR(void);
extern void setupSysTick(void);
extern void sendIR(int length, int message);
extern void wait(int cycles);
extern void waitUS(int us);
extern void sendIRMotorSpeed(int motor, int speed);
extern void pulseLED(int activeUS, int inactiveUS);

	
int main(void)
{	  

	setupGPIOforLED();
	setupSysTick();
	
	sendIRMotorSpeed(1, 1);
	
	while(1);
}

