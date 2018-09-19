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

// FUNCTIONS
// ----------------------------------------------------------------------------

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
	GPIO_PORTC_AFSEL |= 0x30;
	
	// enable pull down
	GPIO_PORTC_PUR |= 0x30;
	
	// digital enable
	GPIO_PORTC_DEN |= 0x30;
	
	// set GPIO alternate function
	GPIO_PORTC_PCTL &=~ 0x00FF0000;
	GPIO_PORTC_PCTL |= 0x00220000;
	
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
