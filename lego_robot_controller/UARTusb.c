// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupUARTforUSB(void);
char readUSB(void);
void writeUSB(char c);
void printNum(int num);
void printDec(int num);
void printString(char* str);

// FUNCTIONS
// ----------------------------------------------------------------------------

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
	
	// set baud rate: 9600, I=104, F=11
	UART0_IBRD = 104;
	UART0_FBRD = 11;
	
	// set line control parameters: 8bits, no parity, 1 stop bit
	UART0_LCRH &=~ 0x0E;
	UART0_LCRH |= 0x60;
	
	// enable UART
	UART0_CTL |= 0x301;
	
	// set GPIO alternate function select
	GPIO_PORTA_AFSEL |= 0x03;
	
	// enable pull up
	GPIO_PORTA_PUR |= 0x03;
	
	// digital enable
	GPIO_PORTA_DEN |= 0x03;
	
	// set GPIO alternate function
	GPIO_PORTA_PCTL &=~ 0x000000FF;
	GPIO_PORTA_PCTL |= 0x00000011;
	
}

// receive a character
char readUSB(void){
	
	// wait for input
	while((UART0_FR & 0x10) == 0x10);
	
	return UART0_DR & 0x000000FF;
	
}

// send a character
void writeUSB(char c){
	
	// wait for output availabiity
	while((UART0_FR & 0x20) == 0x20);
	
	// send data
	UART0_DR = c;
	
}

// send string of characters
void printString(char* str){
	
	while(*str != 0){
		writeUSB(*str);
		str++;
	}
	
}

// prints to USB UART the hex representation of given int
void printNum(int num){
	
	char characters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	
	// for each nibble
	for(int i = 28; i >= 0; i -= 4){
		int n = num >> i & 0xF;
		writeUSB(characters[n]);
	}
	
	// new line
	writeUSB('\r');
	writeUSB('\n');
	
}

// prints to USB UART the decimal representation of a given int
void printDec(int num){
	
	char characters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	
	// if negative print - 
	if(num < 0)
		writeUSB('-');
	
	int tens = 1000000000;
	
	for(int index = 10; index > 0; index--){
		
		int digit = num / tens;
		
		num -= tens * digit;
		
		if(digit < 0)
			digit = -digit;
		
		tens /= 10;
		
		writeUSB(characters[digit]);
	}
	
}
