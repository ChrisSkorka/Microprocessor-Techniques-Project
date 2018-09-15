#include "registers.h"
void Delay(unsigned int);
void setupUARTforUSB(void);
char readUSB(void);
void writeUSB(char c);
void setupUARTforBluetooth(void);
char readBluetooth(void);
void writeBluetooth(char c);

void Delay(unsigned int numLoops){
	volatile unsigned int lp;
	volatile unsigned int i;

	for(lp=0; lp<numLoops; lp++)
		for (i=0; i<=0xFFFF; i++) ;
}

// configure UART0 on port A pins 0 and 1
void setupUARTforUSB(void){
	
	// enable clock for port A
	SYSCTL_RCGCGPIO |= 0x01;
	while((SYSCTL_PRGPIO & 0x01) != 0x01);
	
	// enable clock for UART0
	SYSCTL_RCGCUART |= 0x01;
	while((SYSCTL_RCGCUART & 0x01) != 0x01);
	
	// diable UART
	UART0_CTL &=~ 0x01;
	
	// set baud rate: 38400, I=26, F=3
	UART0_IBRD = 26;
	UART0_FBRD = 3;
	
	// set line control parameters: 8bits, even parity, 2 stop bits
	UART0_LCRH |= 0x6E;
	
	// enable UART
	UART0_CTL |= 0x301;
	
	// set GPIO alternate function select
	GPIO_PORTA_AFSEL |= 0x03;
	
	// digital enable
	GPIO_PORTA_DEN |= 0x03;
	
	// set GPIO alternate function
	GPIO_PORTA_PCTL &=~ 0x000000FF;
	GPIO_PORTA_PCTL |= 0x00000011;
	
}

// configure UART1 on port C pins 4 and 5
void setupUARTforBluetooth(void){
	
	// enable clock for port C
	SYSCTL_RCGCGPIO |= 0x04;
	while((SYSCTL_PRGPIO & 0x04) != 0x04);
	
	// enable clock for UART1
	SYSCTL_RCGCUART |= 0x02;
	while((SYSCTL_RCGCUART & 0x02) != 0x02);
	
	// diable UART
	UART1_CTL &=~ 0x01;
	
	// set baud rate: 38400, I=26, F=3
	UART1_IBRD = 104;
	UART1_FBRD = 11;
	
	// set line control parameters: 8bits, even parity, 2 stop bits
	UART1_LCRH |= 0x6E;
	
	// enable UART
	UART1_CTL |= 0x301;
	
	// set GPIO alternate function select
	GPIO_PORTC_AFSEL |= 0x30;
	
	// digital enable
	GPIO_PORTC_DEN |= 0x30;
	
	// set GPIO alternate function
	GPIO_PORTC_PCTL &=~ 0x00FF0000;
	GPIO_PORTC_PCTL |= 0x00220000;
	
}

// receive a character
char readUSB(void){
	
	// wait for input
	while((UART0_FR & 0x10) == 0x10);
	
	return UART0_DR;
	
}

// send a character
void writeUSB(char c){
	
	// wait for output availabiity
	while((UART0_FR & 0x20) == 0x20);
	
	// send data
	UART0_DR = c;
	
}

// receive a character
char readBluetooth(void){
	
	// wait for input
	while((UART1_FR & 0x10) == 0x10);
	
	return UART1_DR;
	
}

// send a character
void writeBluetooth(char c){
	
	// wait for output availabiity
	while((UART1_FR & 0x20) == 0x20);
	
	// send data
	UART1_DR = c;
	
}
