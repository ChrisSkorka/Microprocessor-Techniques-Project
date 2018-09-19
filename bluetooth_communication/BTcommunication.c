// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern char readBluetooth(void);
extern void writeBluetooth(char c);

// PROTOTYPES
// ----------------------------------------------------------------------------
void bluetoothISR(void);
void processBluetoothCommand(char command);

// GLOBAL VARIABLES
// ----------------------------------------------------------------------------
char mode = 0;
char speed = 0;
char direction = 0;
char targetHeading = 0;

// FUNCTIONS
// ----------------------------------------------------------------------------

// bluetooth recieve interrupt handler
void bluetoothISR(void){
	
	// read command and process it
	char c = readBluetooth();
	processBluetoothCommand(c);
	
	// clear interrupt status
	UART1_ICR |= 0x10;
	
}

// processes a bluetooth UART command and decodes it
void processBluetoothCommand(char command){
	
	// get mode
	mode = command & 0x80;
	
	if(mode){
		// if direct mode
		
		// get speed
		speed = command >> 3 & 0xF;
		
		// get direction
		direction = command & 0x7;
		
	}else{
		// heading mode
		
		// map from 2 bits to 4 bits
		char speedMap[] = {0, 0x7, 0, 0xF};

		// get speed
		speed = speedMap[command >> 5 & 0x3];
		
		// get heading
		targetHeading = command & 0x1F;
		
	}
	
//	printString("m: ");
//	printNum((int) mode);
//	printString("s: ");
//	printNum((int) speed);
//	printString("d: ");
//	printNum((int) direction);
//	printString("h: ");
//	printNum((int) targetHeading);
	
}
