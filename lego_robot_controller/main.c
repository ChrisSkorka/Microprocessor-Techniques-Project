// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// main timer
extern void setupTimerForMain(void);
extern void setupTimerInterrupt(void);
extern void clearTimerInterrupt(void);
extern void wait(int cycles);
extern void waitUS(int us);

// system timer
extern void setupSysTick(void);
// extern void setupTimerForIR(void);

// UART usb
extern void setupUARTforUSB(void);
extern void printString(char* str);
extern void printNum(int num);
extern void printDec(int num);

// UART bluetooth
extern void setupUARTforBluetooth(void);
extern void setupBluetoothInterrupt(void);

// GPIO ir led
extern void setupGPIOforLED(void);

// IR communication
extern void sendIR(int length, int message);
extern void sendIRMotorSpeed(int motor, int speed);
extern void pulseLED(int activeUS, int inactiveUS);

// I2C compass A
extern void setupI2CforCompassA(void);
extern void setupCompassA(void);

// I2C compass B
extern void setupI2CforCompassB(void);
extern void setupCompassB(void);

// Compass measurements
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

// PROTOTYPES
// ----------------------------------------------------------------------------
int main(void);
void mainISR(void);
void mainLoop(void);
void updateControlls(int m, int s, int d);

// GLOBALS
// ----------------------------------------------------------------------------
int speed = 0;
int direction = 0;
int mode = 0;

// FUNCTIONS
// ----------------------------------------------------------------------------

// main
int main(void){	  

	// setup main timer
	setupTimerForMain();
	setupTimerInterrupt();
	
	// setup UART for USB and Bluetooth
	setupUARTforUSB();
	setupUARTforBluetooth();
	setupBluetoothInterrupt();
	
	// setup IR communication
	setupGPIOforLED();
	setupSysTick();
	
	// setup Compass A
	setupI2CforCompassA();
	setupCompassA();
	
	// setup Compass B
	setupI2CforCompassB();
	setupCompassB();
	
	
	
	
	
	printString("Ready ");
	
	while(1);
}

// main program run periodically
void mainISR(void){
	
	// run main loop
	mainLoop();
	
	// clear interrupt status
	clearTimerInterrupt();
	
}

// loop run periodically
void mainLoop(void){
	
	printString("Now ");
	
	// sendIRMotorSpeed(speed, direction);
	
	
	
}

// set speed and direction
void updateControlls(int m, int s, int d){
	mode = m & 0xF;
	speed = s & 0xF;
	direction = d & 0xF;
}
