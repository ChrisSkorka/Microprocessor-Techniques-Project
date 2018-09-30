#include "registers.h"

extern void setupUARTforUSB(void);
extern char readUSB(void);
extern void writeUSB(char c);
extern void setupUARTforBluetooth(void);
extern char readBluetooth(void);
extern void writeBluetooth(char c);
extern void setupBluetoothInterrupt(void);

	
int main(void){	  
	
	// setup UART for USB and Bluetooth
	setupUARTforUSB();
	setupUARTforBluetooth();
	setupBluetoothInterrupt();
	
	writeUSB('A');
	writeUSB('B');
	writeUSB('C');
	
//	while(1){
//		char c = readBluetooth();
//		writeBluetooth(c);
//		writeUSB(c);
////		char c = readUSB();
////		writeUSB(c);
////		writeBluetooth(c);
//	}
	
	while(1);
}

