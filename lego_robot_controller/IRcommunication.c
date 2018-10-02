// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern void waitUS(int us);

// PROTOTYPES
// ----------------------------------------------------------------------------
void sendIR(int length, int message);
void sendIRMotorSpeed(int motor, int speed);
void pulseLED(int activeUS, int inactiveUS);
void messagePause(int channel, int count);

// FUNCTIONS
// ----------------------------------------------------------------------------

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
	int activeCycles = 6; 	// 157us
	int lowCycles = 10; 		// 263us
	int highCycles = 21; 		// 552us
	int startCycles = 39; 	// 1026us
	
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
}
