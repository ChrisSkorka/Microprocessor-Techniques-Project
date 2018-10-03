// ----------------------------------------------------------------------------
// 
// CompassMeasurements.c
// 
// includes methods to read measurements from the compass sensors and calculate
// the heading in degrees
// provides methods to get the headings of each compass, calculate the
// difference in heading bewteen two values and calibration for the sensors
// 
// ----------------------------------------------------------------------------

// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern char readCompassA(char address);
extern void writeCompassA(char address, char data);
extern char readCompassB(char address);
extern void writeCompassB(char address, char data);
extern void printNum(int num);
extern int inversetan(float x);
extern void enableOnboardLED(void);
extern void disableOnboardLED(void);

// PROTOTYPES
// ----------------------------------------------------------------------------
int getCompassAX(void);
int getCompassAY(void);
int getCompassAZ(void);
int getCompassBX(void);
int getCompassBY(void);
int getCompassBZ(void);
int getHeading(int x, int y);

int getCompassAHeading(int* calibration);
int getCompassBHeading(int* calibration);
int getDifferenceInHeading(int A, int B);
void calibrateSensors(int* calibration);
void calibrateCompassB(int* calibration);
	
// GLOBALS
// ----------------------------------------------------------------------------
#define COMPASS_MEMORY_ADDRESS_X_MSB 3
#define COMPASS_MEMORY_ADDRESS_X_LSB 4
#define COMPASS_MEMORY_ADDRESS_Z_MSB 5
#define COMPASS_MEMORY_ADDRESS_Z_LSB 6
#define COMPASS_MEMORY_ADDRESS_Y_MSB 7
#define COMPASS_MEMORY_ADDRESS_Y_LSB 8

// FUNCTIONS
// ----------------------------------------------------------------------------

// read compass A X axis measurements
int getCompassAX(void){
	
	// read most significant bit, least significant bit and combine
	short x = readCompassA(COMPASS_MEMORY_ADDRESS_X_MSB) << 8
		| readCompassA(COMPASS_MEMORY_ADDRESS_X_LSB);
	
	// convert to int
	return (int)x;
}

// read compass A Y axis measurements
int getCompassAY(void){
	
	// read most significant bit, least significant bit and combine
	short y = readCompassA(COMPASS_MEMORY_ADDRESS_Y_MSB) << 8
		| readCompassA(COMPASS_MEMORY_ADDRESS_Y_LSB);
	
	// convert to int
	return (int)y;
}

// read compass A Z axis measurements
int getCompassAZ(void){
	
	// read most significant bit, least significant bit and combine
	short z = readCompassA(COMPASS_MEMORY_ADDRESS_Z_MSB) << 8
		| readCompassA(COMPASS_MEMORY_ADDRESS_Z_LSB);
	
	// convert to int
	return (int)z;
	
}

// read compass B X axis measurements
int getCompassBX(void){
	
	// read most significant bit, least significant bit and combine
	short x = readCompassB(COMPASS_MEMORY_ADDRESS_X_MSB) << 8
		| readCompassB(COMPASS_MEMORY_ADDRESS_X_LSB);
	
	// convert to int
	return (int)x;
}

// read compass B Y axis measurements
int getCompassBY(void){
	
	// read most significant bit, least significant bit and combine
	short y = readCompassB(COMPASS_MEMORY_ADDRESS_Y_MSB) << 8
		| readCompassB(COMPASS_MEMORY_ADDRESS_Y_LSB);
	
	// convert to int
	return (int)y;
}

// read compass B Z axis measurements
int getCompassBZ(void){
	
	// read most significant bit, least significant bit and combine
	short z = readCompassB(COMPASS_MEMORY_ADDRESS_Z_MSB) << 8
		| readCompassB(COMPASS_MEMORY_ADDRESS_Z_LSB);
	
	// convert to int
	return (int)z;
	
}

// calculate the heading in degrees from 0 to 360 with 0 pointing to Y+
int getHeading(int x, int y){
	
	// calculate degrees with 0 pointing left
	int degrees = inversetan((float) y / (float) x);
	
	// shift such that 0 points up
	degrees += 90;
	
	// mirror left side to complete it
	if(x < 0)
		degrees = 180 + degrees;
	
	return degrees;
	
}

// get heading for compass A
int getCompassAHeading(int* calibration){
	
	// get compass readings
	int x = getCompassAX() - calibration[0];
	int y = getCompassAY() - calibration[1];
	int z = getCompassAZ() - calibration[2];
	
	// get heading in degrees
	return getHeading(x, y);
	
}

// get heading for compass B
// IMPORTANT compass B now uses x and z values
int getCompassBHeading(int* calibration){
	
	// get compass readings
	int x = getCompassBX() - calibration[0];
	int y = getCompassBY() - calibration[1];
	int z = getCompassBZ() - calibration[2];
	
	// get heading in degrees
	return getHeading(x, z);
}

// calculate the difference in heading bewteen two headings
int getDifferenceInHeading(int A, int B){
	
	int difference = B - A;
	
	if(difference > 180)
		difference = difference - 360;
	if(difference < -180)
		difference = difference + 360;
	
	return difference;
}

// find lower and upper bound of each sensor's axis' and then computes the center values to calibrate
void calibrateSensors(int* calibration){
	
	// indicate start of calibration
	enableOnboardLED();
	
	// init ranges
	int range[6][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	
		// take 3000 measurements (gives a few seconds to move the sensor)
	for(int i = 0; i < 3000; i++){
		
		// collect all measurements
		int measurements[] = {
			getCompassAX(),
			getCompassAY(),
			getCompassAZ(),
			getCompassBX(),
			getCompassBY(),
			getCompassBZ()
		};
		
		// update lower and upper bound
		for(int j = 0; j < 6; j++){
			if(measurements[j] < range[j][0])
				range[j][0] = measurements[j];
			if(measurements[j] > range[j][1])
				range[j][1] = measurements[j];
		}
	}
	
	// compute centers of lower and upper bound for callibration
	for(int j = 0; j < 6; j++){
		calibration[j] = (range[j][1] + range[j][0]) / 2;
	}
	
	// indicate end of calibration
	disableOnboardLED();
	
}

// find lower and upper bound of each axis' and then computes the center values to calibrate
void calibrateCompassB(int* calibration){
	
	// indicate start of calibration
	enableOnboardLED();
	
	// init ranges
	int range[3][2] = {{0, 0}, {0, 0}, {0, 0}};
	
		// take 3000 measurements (gives a few seconds to move the sensor)
	for(int i = 0; i < 3000; i++){
		
		// collect all measurements
		int measurements[] = {
			getCompassBX(),
			getCompassBY(),
			getCompassBZ()
		};
		
		// update lower and upper bound
		for(int j = 0; j < 3; j++){
			if(measurements[j] < range[j][0])
				range[j][0] = measurements[j];
			if(measurements[j] > range[j][1])
				range[j][1] = measurements[j];
		}
	}
	
	// compute centers of lower and upper bound for callibration
	for(int j = 0; j < 3; j++){
		calibration[j] = (range[j][1] + range[j][0]) / 2;
	}
	
	// indicate end of calibration
	disableOnboardLED();
	
}
