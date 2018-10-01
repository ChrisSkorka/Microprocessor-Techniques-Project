#include "registers.h"

extern void setupUARTforUSB(void);
extern char readUSB(void);
extern void writeUSB(char c);
extern void printNum(int num);
extern void printString(char* str);
extern void printDec(int num);

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
extern int getDifferenceInHeading(int A, int B, int calibration);
extern void calibrateSensors(int* calibration);

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
	int compassCalibration = 0;
	for(int i = 0; i < 10; i++)
		compassCalibration += getDifferenceInHeading(getCompassAHeading(), getCompassBHeading(), 0);
	compassCalibration /= 10;
	
//	for(int i = -10; i <= 10; i++)
//		printNum(i % 5);
//	
//	while(1);
	
	printString("Begin calibrations\r\n");
	for(int i = 0; i < 8000000; i++);
	printString("Calibrating\r\n");
	
	int calibration[12];
	calibrateSensors(calibration);
	
	printString("Calibration finished\r\n");
	
	for(int i = 0; i < 12; i++){
		printDec(i);
		printString(": ");
		printDec(calibration[i]);
		printString("\r\n");
	}
	
	while(1){
//		printNum((int)(readCompassA(0)));
		

		
		printString(" AX: ");
		printDec(getCompassAX());
		printString(" AY: ");
		printDec(getCompassAY());
		printString(" AZ: ");
		printDec(getCompassAZ());
		printString(" BX: ");
		printDec(getCompassBX());
		printString(" BY: ");
		printDec(getCompassBY());
		printString(" BZ: ");
		printDec(getCompassBZ());
		printString("\r\n");
		
//	int a = getCompassAHeading();
//	int b = getCompassBHeading();
//	
//	printString("A: ");
//	printDec(a);
//	printString(" B: ");
//	printDec(b);
//	printString(" D: ");
//	printDec(getDifferenceInHeading(a, b, compassCalibration));
//	printString("\r\n");

		
//		for(int i = 0; i < 400000;i++);
// 		break;
	}
	
	
// 	while(1);
}

