// ----------------------------------------------------------------------------
// 
// main.c
// 
// sets up all subsystems, calibrates the compass and starts periodic interrupt 
// for the main loop
// in each periodic interrupt it measures the compass values and canculates the
// required motor speeds and transmits them to the robot via IR
// 
// ----------------------------------------------------------------------------

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

// GPIO IR LED
extern void setupGPIOforLED(void);

//GPIO onboard LED
extern void setupGPIOforOnboardLED(void);

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
extern int getCompassAHeading(int* calibration);
extern int getCompassBHeading(int* calibration);
extern int getDifferenceInHeading(int A, int B);
extern void calibrateSensors(int* calibration);
extern void calibrateCompassB(int* calibration);
extern int getHeading(int x, int y);

// PROTOTYPES
// ----------------------------------------------------------------------------
int main(void);
void mainISR(void);
void mainLoop(void);
void updateControlls(int type, int data);

// GLOBALS
// ----------------------------------------------------------------------------
int speed = 0;
int direction = 0;
int maxBend = 45;
int centersB[] = {0, 0, 0};

// FUNCTIONS
// ----------------------------------------------------------------------------

// main
int main(void){	  

	// setup UART for USB and Bluetooth
	setupUARTforUSB();
	setupUARTforBluetooth();
	setupBluetoothInterrupt();
	
	// setup IR communication
	setupGPIOforLED();
	setupSysTick();
	
	// setup onboard LED
	setupGPIOforOnboardLED();
	
// NOTE: the following code was intended for the old modes of the original design
//	// setup Compass A
//	setupI2CforCompassA();
//	setupCompassA();
	
	// setup Compass B
	setupI2CforCompassB();
	setupCompassB();
	
	// calibrate compasses
	printString("Begin calibration\r\n");
	calibrateCompassB(centersB);
	printString("Finished calibration\r\n");
	
	// setup main timer and periodic interrupt
	setupTimerForMain();
	setupTimerInterrupt();
	
	
	printString("Ready\r\n");
	
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
// measures compass orientation, compares it to the target heading, calculates 
// motor speeds and sends the motor speeds via RI to the lego robot
void mainLoop(void){
	
	// get heading and difference to target heading
	int currentHeading = getCompassBHeading(centersB);
	int difference = getDifferenceInHeading(currentHeading, direction);
	
	printString("H: ");
	printDec(currentHeading);
	printString(" D: ");
	printDec(difference);
	printString("\r\n");
	
	// limit range
	if(difference > maxBend)
		difference = maxBend;
	if(difference < -maxBend)
		difference = -maxBend;
	
	// calculate corresponding action
	int directionSpeed = difference / (maxBend / 6);
	
	// if stationary dont turn
	if(speed == 0)
		directionSpeed = 0;
	
	// send IR commands
	sendIRMotorSpeed(speed, directionSpeed);
	
}

// set speed and direction
void updateControlls(int type, int data){
	
	if(type){
		// speed
		speed = data;
	}else{
		// target direction
		direction = data * 3;
	}
	
}
