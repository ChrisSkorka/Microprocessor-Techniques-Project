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

extern void setupI2CforCompassB(void);
extern void setupCompassB(void);
extern char readCompassB(char address);
extern void writeCompassB(char address, char data);

extern int getCompassAX(void);
extern int getCompassAY(void);
extern int getCompassAZ(void);
extern int getCompassBX(void);
extern int getCompassBY(void);
extern int getCompassBZ(void);

extern int getCompassAHeading(void);
extern int getCompassBHeading(void);
extern int getDifferenceInHeading(int A, int B, int callibration);

int main(void){	  
	
	// setup UART for USB
	setupUARTforUSB();
	for(int i = 0; i < 400000; i++);
	
	printString("Ready\r\n");
	
	// setup Compass A
	setupI2CforCompassA();
	setupCompassA();
	
	// setup Compass B
	setupI2CforCompassB();
	setupCompassB();
	
	// calibrate alignment of sensors
	int compassCallibration = 0;
	for(int i = 0; i < 10; i++)
		compassCallibration += getDifferenceInHeading(getCompassAHeading(), getCompassBHeading(), 0);
	compassCallibration /= 10;
	
//	for(int i = -10; i <= 10; i++)
//		printNum(i % 5);
//	
//	while(1);
	
	
	while(1){
//		printNum((int)(readCompassA(0)));
		

		
//		printString(" AX: ");
//		printNum(getCompassAX());
//		printString(" AY: ");
//		printNum(getCompassAY());
//		printString(" AZ: ");
//		printNum(getCompassAZ());
//		printString(" BX: ");
//		printNum(getCompassBX());
//		printString(" BY: ");
//		printNum(getCompassBY());
//		printString(" BZ: ");
//		printNum(getCompassBZ());
//		printString("\r\n");
		
	int a = getCompassAHeading();
	int b = getCompassBHeading();
	
	printString("A: ");
	printNum(a);
	printString(" B: ");
	printNum(b);
	printString(" D: ");
	printNum(getDifferenceInHeading(a, b, compassCallibration));
	printString("\r\n");
//		
		
//		for(int i = 0; i < 400000;i++);
// 		break;
	}
	
	
// 	while(1);
}

