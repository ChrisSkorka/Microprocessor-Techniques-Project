#include "registers.h"

extern void setupUARTforUSB(void);
extern char readUSB(void);
extern void writeUSB(char c);
extern void printNum(int num);
extern void printString(char* str);

extern void setupI2CforCompassA(void);
extern void setupCompassA(void);
extern char readCompassA(char address);
extern void writeCompassA(char address, char data);

//extern void setupI2CforCompassB(void);
//extern void setupCompassB(void);
//extern char readCompassB(char address);
//extern void writeCompassB(char address, char data);

extern void setupGPIOforCompassA(void);

int main(void){	  
	
	// setup UART for USB
	setupUARTforUSB();
	for(int i = 0; i < 400000; i++);
	
	printString("Ready\r\n");
	
	// setup Compass A
	setupI2CforCompassA();
	setupGPIOforCompassA();
	setupCompassA();
	
	// setup Compass B
//	setupI2CforCompassB();
//	setupCompassB();
	
	while(1){
//		printNum((int)(readCompassA(0)));
		
		int ax = readCompassA(3) << 8 | readCompassA(4);
		int ay = readCompassA(7) << 8 | readCompassA(8);
//		short bx = readCompassB(3) << 8 | readCompassB(4);
//		short by = readCompassB(7) << 8 | readCompassB(8);
		
		printString("AX: ");
		printNum(ax);
		printString(" AY: ");
		printNum(ay);
//		printString(" BX: ");
//		printNum(bx);
//		printString(" BY: ");
//		printNum(by);
		printString("\r\n");
		
//		for(int i = 0; i < 400000;i++);
//		break;
	}
	
	
//	while(1);
}

