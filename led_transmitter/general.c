#include "registers.h"
void Delay(unsigned int);
void setupGPIOforLED(void);
void setupTimerForIR(void);
void setupSysTick(void);
void sendIR(int length, int message);
void wait(int cycles);
void waitUS(int us);
void sendIRMotorSpeed(int motor, int speed);
void pulseLED(int activeUS, int inactiveUS);
void messagePause(int channel, int count);

void Delay(unsigned int numLoops){
	volatile unsigned int lp;
	volatile unsigned int i;

	for(lp=0; lp<numLoops; lp++)
		for (i=0; i<=0x0FA0; i++) ;
}

// setup GPIO for the IR LED, it is used as a digital output on port PB6
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

// setup Timer0 for LED on period, the timer will be a one shot timer on PB6
void setupTimerForIR(void){
	
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

// seyup Systick timer
void setupSysTick(void){
	
	// disable timer
	NVIC_ST_CTRL &=~ 0x01;
	
	// set reload
	NVIC_ST_RELOAD = 0xFFFFFF;
	NVIC_ST_CURRENT = 0x8FFFFF;
	
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

// sends IR codes for motor speed
void sendIRMotorSpeed(int speedA, int speedB){
	// 01CC BBBB AAAA LLLL ---- ---- ---- ----
	// 0100 0111 0111 1011 0000 0000 0000 0000
	// 0x477B0000
	
	int nibble1 = 0x4; // mode
	int nibble2 = speedB & 0xF;
	int nibble3 = speedA & 0xF;
	int nibble4 = 0xF ^ nibble1 ^ nibble2 ^ nibble3;
	
	// compose message
	int message = nibble1 << 28 | nibble2 << 24 | nibble3 << 20 | nibble4 << 16;
	
	for(int i = 0; i < 5; i++){
		messagePause(0, i);
		sendIR(16, message);
	}
		
}

// pulse LED at 38kHz for active cycles and then pause for inactive cycles
void pulseLED(int activeCycles, int inactiveCycles){
	
	// pulse LED at 38kHz
	for(int i = 0; i < activeCycles; i++){
		
		// turn on LED
		GPIO_PORTB_DATA |= 0x40;
		
		// wait active time
		waitUS(13);
		
		// turn off LED
		GPIO_PORTB_DATA &=~ 0x40;
		
		// wait active time
		waitUS(13);
	}
	
	// wait delay time
	// waitUS(inactiveCycles * 26);
	
	for(int i = 0; i < inactiveCycles; i++){
		waitUS(13);
		waitUS(13);
	}
	
}

// wait between message repeats as specified in the protocol provided by LEGO
void messagePause(int channel, int count){
 unsigned char a = 0;
 
 if(count == 0)
   a = 4 - channel + 1;
 else if(count == 1 || count == 2)
   a = 5;
 else if(count == 3 || count == 4)
   a = 5 + (channel + 1) * 2;
     
 waitUS(a * 77);
}

// send an array if 2bits (2 bits = 1 transmitted bit)
void sendIR(int length, int message){
	
	// durations for signals
	int activeCycles = 6; // 6 cycles, 157us
	int lowCycles = 10; // 10 cycles, 263us
	int highCycles = 21; // 21 cycles, 552us
	int startCycles = 39; // 39 cycles, 1026us
	
//	activeTime 	=  100000;
//	lowTime 		=  500000;
//	highTime		= 1000000;
//	startTime		=  750000;
//	stopTime		=  750000;
	
	// send start bit
	pulseLED(activeCycles, startCycles);
	
	// process each bit
	for(int i = 0; i < length; i++){
		
		// get position of bit pair in array
		int bit = message & (0x80000000 >> i);
		
		// wait delay time
		if(bit == 0)
			pulseLED(activeCycles, lowCycles);
		else
			pulseLED(activeCycles, highCycles);
	}
	
	// send stop bit
	pulseLED(activeCycles, startCycles);
	// pulseLED(activeCycles, stopCycles);
}

