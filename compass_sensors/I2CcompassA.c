// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupI2CforCompassA(void);
void setupCompassA(void);
char readCompassA(char address);
void writeCompassA(char address, char data);
	
// GLOBALS
// ----------------------------------------------------------------------------
#define ADDRESS_COMPASS_A_READ 0x3D
#define ADDRESS_COMPASS_A_WRITE 0x3C

// FUNCTIONS
// ----------------------------------------------------------------------------

// setup I2C for compass A on I2C 0 and port B pins 2, 3
void setupI2CforCompassA(void){
	
	// enable I2C 0 clock
	SYSCTL_RCGCI2C |= 0x01;
	while((SYSCTL_PRI2C & 0x01) != 0x01);
	
	// enable GPIO port B clock
	SYSCTL_RCGCGPIO |= 0x02;
	while((SYSCTL_PRGPIO & 0x02) != 0x02);
	
	for(int i = 0; i < 400000; i++);
	
	// enable alternate function
	GPIO_PORTB_AFSEL |= 0x0C;
	
	// select alternate function
	GPIO_PORTB_PCTL &=~ 0x0000FF00;
	GPIO_PORTB_PCTL |= 0x00003300;
	
	// digital enable
	GPIO_PORTB_DEN |= 0x0C;
	
	// set SDA open drain
	GPIO_PORTB_ODR |= 0x08;
	
	// set pull up
	// GPIO_PORTB_PUR |= 0x0C;
	
	// init master
	I2C0_MCR = 0x00000010;
	
	// set speed 100kbps, 7
	I2C0_MTPR = 7;
	
	// 
}

// setup compass A 
void setupCompassA(void){
	
	// read to avoid errous communication produced by aborted communication previously
	for(int i = 0; i < 3; i++){
		
		// set read address
		I2C0_MSA = ADDRESS_COMPASS_A_READ;
		
		// start, stop  
		I2C0_MCS = 0x07;
		
		// wait for data to transmit
		while((I2C0_MCS & 0x01) == 0x01);
		
	}
	
	// set configuration: 8 samples, 15Hz
	writeCompassA(0x00, 0xF0);
	writeCompassA(0x01, 0x20);
	
	// set continuus measurement mode
	writeCompassA(0x02, 0x00);
	
}

// read data from compass A I2C device
char readCompassA(char address){
	
	// set write address
	I2C0_MSA = ADDRESS_COMPASS_A_WRITE;
	
	// set compass device memory address
	I2C0_MDR = address;
	
	// start, send single byte and stop
	I2C0_MCS = 0x07;
	
	// wait for data to transmit
	while((I2C0_MCS & 0x01) == 0x01);
	
	// set read address
	I2C0_MSA = ADDRESS_COMPASS_A_READ;
	
	// start, stop  
	I2C0_MCS = 0x07;
	
	// wait for data to transmit
	while((I2C0_MCS & 0x01) == 0x01);
	
	// read data
	return I2C0_MDR & 0xFF;
	
}

// write data to compass A I2C device
void writeCompassA(char address, char data){
	
	// set write address
	I2C0_MSA = ADDRESS_COMPASS_A_WRITE;
	
	// set compass device memory address
	I2C0_MDR = address;
	
	// start and send byte 1
	I2C0_MCS = 0x03;
	
	// wait for data to transmit
	while((I2C0_MCS & 0x01) == 0x01);
	
	// set data byte
	I2C0_MDR = data;
	
	// send byte 2 and stop
	I2C0_MCS = 0x05;
	
	// wait for data to transmit
	while((I2C0_MCS & 0x01) == 0x01);
	
}
