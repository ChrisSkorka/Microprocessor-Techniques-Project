// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern char readCompassA(char address);
extern void writeCompassA(char address, char data);
extern char readCompassB(char address);
extern void writeCompassB(char address, char data);
extern void printNum(int num);
extern int inversetan(float x);

// PROTOTYPES
// ----------------------------------------------------------------------------
int getCompassAX(void);
int getCompassAY(void);
int getCompassAZ(void);
int getCompassBX(void);
int getCompassBY(void);
int getCompassBZ(void);
int getHeading(int x, int y);
int getCompassAHeading(void);
int getCompassBHeading(void);
int getDifferenceInHeading(int A, int B, int calibration);
void calibrateSensors(int* calibration);
	
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
int getCompassAHeading(void){
	
	// get compass readings
	int x = getCompassAX();
	int y = getCompassAY();
	int z = getCompassAZ();
	
	// get heading in degrees
	return getHeading(x, y);
	
}

// get heading for compass B
int getCompassBHeading(void){
	
	// get compass readings
	int x = getCompassBX();
	int y = getCompassBY();
	int z = getCompassBZ();
	
	// get heading in degrees
	return getHeading(x, y);
}

// calculate the difference in heading bewteen two headings
int getDifferenceInHeading(int A, int B, int calibration){
	
	// int difference = B - (A + calibration) % 360;
	int difference = B - A;
	
	if(difference > 180)
		difference = difference - 360;
	if(difference < -180)
		difference = difference + 360;
	
	return difference;
}

void calibrateSensors(int* calibration){
	
	calibration[0] = 0;
	calibration[1] = 0;
	calibration[2] = 0;
	calibration[3] = 0;
	calibration[4] = 0;
	calibration[5] = 0;
	calibration[6] = 0;
	calibration[7] = 0;
	calibration[8] = 0;
	calibration[9] = 0;
	calibration[10] = 0;
	calibration[12] = 0;
	
	for(int i = 0; i < 3000; i++){
		int measurements[] = {
			getCompassAX(),
			getCompassAY(),
			getCompassAZ(),
			getCompassBX(),
			getCompassBY(),
			getCompassBZ()
		};
		
		for(int j = 0; j < 3; j++){
			
			// lower bound of A
			if(measurements[j] < calibration[2*j])
				calibration[2*j] = measurements[j];
			
			// upper bound of A
			if(measurements[j] > calibration[2*j+1])
				calibration[2*j+1] = measurements[j];
			
			// lower bound of B
			if(measurements[j+3] < calibration[2*j+6])
				calibration[2*j+6] = measurements[j+3];
			
			// upper bound of B
			if(measurements[j+3] > calibration[2*j+1+6])
				calibration[2*j+1+6] = measurements[j+3];
		}
		
	}
	
}
