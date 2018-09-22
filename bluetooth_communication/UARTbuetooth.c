// INCLUDE
// ----------------------------------------------------------------------------
#include "registers.h"
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

// PROTOTYPES
// ----------------------------------------------------------------------------
void setupUARTforBluetooth(void);
void setupBluetoothInterrupt(void);
char readBluetooth(void);
void writeBluetooth(char c);
void clearBluetoothInterrupt(void);

// FUNCTIONS
// ----------------------------------------------------------------------------

// configure UART1 on port B pins 0 and 1
void setupUARTforBluetooth(void){
	
	// enable clock for port C
	SYSCTL_RCGCGPIO |= 0x02;
	while((SYSCTL_PRGPIO & 0x02) != 0x02);
	
	// enable clock for UART1
	SYSCTL_RCGCUART |= 0x02;
	while((SYSCTL_RCGCUART & 0x02) != 0x02);
	
	// diable UART
	UART1_CTL &=~ 0x01;
	
	// set baud rate: 9600, I=104, F=11
	UART1_IBRD = 104;
	UART1_FBRD = 11;
	
	// set line control parameters: 8bits, no parity, 1 stop bit
	UART1_LCRH &=~ 0x0E;
	UART1_LCRH |= 0x60;
	
	// enable UART
	UART1_CTL &=~ 0x20;
	UART1_CTL |= 0x301;
	
	// set GPIO alternate function select
	GPIO_PORTB_AFSEL |= 0x03;
	
	// enable pull up
	GPIO_PORTB_PUR |= 0x01;
	
	// digital enable
	GPIO_PORTB_DEN |= 0x03;
	
	// set GPIO alternate function
	GPIO_PORTB_PCTL &=~ 0x000000FF;
	GPIO_PORTB_PCTL |= 0x00000011;
	
}

// enable interrupt on bluetooth receive
void setupBluetoothInterrupt(void){
	
	// disable all interrupts
	// CPSID();
	DisableInterrupts();
	
	// set interrupt FIFO level = 1
	UART1_IFLS &=~ 0x28;
	UART1_IFLS |= 0x08;
	
	// enable interrupt
	UART1_IM |= 0x10;
	
	// enable UART1 interrupts
	NVIC_EN0 |= 0x40;
	
	// set priority = 1 (second highest)
	NVIC_PRI1 &=~ (unsigned)(0x7) << 13;
	NVIC_PRI1 |= (unsigned)(0x1) << 13;
	
	// set interrupt service routine
	// done in startup.s
	
	// enable all interrupts
	// CPSIE();
	EnableInterrupts();
	
}

// receive a character
char readBluetooth(void){
	
	// wait for input
	while((UART1_FR & 0x10) == 0x10);
	
	return UART1_DR & 0x000000FF;
	
}

// send a character
void writeBluetooth(char c){
	
	// wait for output availabiity
	while((UART1_FR & 0x20) == 0x20);
	
	// send data
	UART1_DR &=~ 0xFF;
	UART1_DR = c;
	
}

// clear interrupt status
void clearBluetoothInterrupt(void){
	UART1_ICR |= 0x10;
}
