// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern char readBluetooth(void);
extern void writeBluetooth(char c);
extern void clearBluetoothInterrupt(void);
extern void writeUSB(char c);
extern void updateControlls(int type, int data);

// PROTOTYPES
// ----------------------------------------------------------------------------
void bluetoothISR(void);
void decode(void);
void processBluetoothCommand(char command);

// GLOBAL VARIABLES
// ----------------------------------------------------------------------------


// FUNCTIONS
// ----------------------------------------------------------------------------

// bluetooth recieve interrupt handler
void bluetoothISR(void){
	
	// read command and process it
	char c = readBluetooth();
	processBluetoothCommand(c);
	// writeUSB(c);
	
	// clear interrupt status
	clearBluetoothInterrupt();
	
}

// processes a bluetooth UART command and decodes it
void processBluetoothCommand(char command){
	
	// get mode
	int type = command & 0x80;
	
	if(type){
		// speed
		updateControlls(type, command & 0xF);
		
	}else{
		// target heading
		updateControlls(type, command & 0x7F);
	}
	
//	if(mode == 0){
//		// if direct mode
//		
//		// get speed
//		speed = command >> 3 & 0xF;
//		
//		// map from 3 bits to 4 bits
//		// char speedMap[] = {0, 0x2, 0x5,  0x7, 0, 0xA, 0xD, 0xF};
//		
//		// get direction
//		int d = (command & 0x7) * 2;
//		direction = d & 0xF;
//		// direction = speedMap[d];
//		
//	}else if(mode == 1){
//		// heading mode
//		
//		// map from 2 bits to 4 bits
//		char speedMap[] = {0, 0x2, 0x5,  0x7, 0, 0xA, 0xD, 0xF};

//		// get speed
//		speed = speedMap[command >> 5 & 0x3];
//		
//		// get heading
//		direction = command & 0x1F;
//		
//	}
	
}
