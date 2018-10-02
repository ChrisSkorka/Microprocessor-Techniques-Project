// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupI2CforCompassB(void);
void setupCompassB(void);
char readCompassB(char address);
void writeCompassB(char address, char data);
	
// GLOBALS
// ----------------------------------------------------------------------------
#define ADDRESS_COMPASS_B_READ 0x3D
#define ADDRESS_COMPASS_B_WRITE 0x3C

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup I2C for compass B on I2C 1 and port A pins 6, 7
void setupI2CforCompassB(void){
	
	// enable I2C 1 clock
	SYSCTL_RCGCI2C |= 0x02;
	while((SYSCTL_PRI2C & 0x02) != 0x02);
	
	// enable GPIO port A clock
	SYSCTL_RCGCGPIO |= 0x01;
	while((SYSCTL_PRGPIO & 0x01) != 0x01);
	
	for(int i = 0; i < 400000; i++);
	
	// enable alternate function
	GPIO_PORTA_AFSEL |= 0xC0;
	
	// select alternate function
	GPIO_PORTA_PCTL &=~ 0xFF000000;
	GPIO_PORTA_PCTL |= 0x33000000;
	
	// digital enable
	GPIO_PORTA_DEN |= 0xC0;
	
	// set SDA open drain
	GPIO_PORTA_ODR |= 0x80;
	
	// init master
	I2C1_MCR = 0x00000010;
	
	// set speed 100kbps, 7
	I2C1_MTPR = 7;
	
}

// setup compass B 
void setupCompassB(void){
	
	// read to avoid errous communication produced by aborted communication previously
	for(int i = 0; i < 3; i++){
		
		// set read address
		I2C1_MSA = ADDRESS_COMPASS_B_READ;
		
		// start, stop  
		I2C1_MCS = 0x07;
		
		// wait for data to transmit
		while((I2C1_MCS & 0x01) == 0x01);
		
	}
	
	// set configuration: 8 samples, 15Hz
	writeCompassB(0x00, 0xF0);
	writeCompassB(0x01, 0x20);
	
	// set continuus measurement mode
	writeCompassB(0x02, 0x00);
	
}

// read data from compass A I2C device
char readCompassB(char address){
	
	// set write address
	I2C1_MSA = ADDRESS_COMPASS_B_WRITE;
	
	// set compass device memory address
	I2C1_MDR = address;
	
	// start, send single byte and stop
	I2C1_MCS = 0x07;
	
	// wait for data to transmit
	while((I2C1_MCS & 0x01) == 0x01);
	
	// set read address
	I2C1_MSA = ADDRESS_COMPASS_B_READ;
	
	// start, stop  
	I2C1_MCS = 0x07;
	
	// wait for data to transmit
	while((I2C1_MCS & 0x01) == 0x01);
	
	// read data
	return I2C1_MDR & 0xFF;
	
}

// write data to compass A I2C device
void writeCompassB(char address, char data){
	
	// set write address
	I2C1_MSA = ADDRESS_COMPASS_B_WRITE;
	
	// set compass device memory address
	I2C1_MDR = address;
	
	// start and send byte 1
	I2C1_MCS = 0x03;
	
	// wait for data to transmit
	while((I2C1_MCS & 0x01) == 0x01);
	
	// set data byte
	I2C1_MDR = data;
	
	// send byte 2 and stop
	I2C1_MCS = 0x05;
	
	// wait for data to transmit
	while((I2C1_MCS & 0x01) == 0x01);
	
}
