#include "registers.h"

extern void DelayMs(unsigned int);
extern void setupUARTforUSB(void);
extern char readUSB(void);
extern void writeUSB(char c);
extern void setupUARTforBluetooth(void);
extern char readBluetooth(void);
extern void writeBluetooth(char c);

	
int main(void){	  
	
	// setup uart for USB and Bluetooth
	setupUARTforUSB();
	setupUARTforBluetooth();
	
	while(1){
		// char c = readBluetooth();
		// writeUSB(c);
		char c = readUSB();
		writeUSB(c);
		writeBluetooth(c);
	}
	
	while(1);
}

