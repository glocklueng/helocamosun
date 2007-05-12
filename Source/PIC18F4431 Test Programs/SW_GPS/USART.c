/*
USART.c
*/
#include <p18f4431.h>

#include "USART.h"


/*
Function: SerialInit
Description: Sets up serial communications for 115200 baud, 8 N 1
*/
void SerialInit(void)
{
	TRISCbits.TRISC6 = 1;	// Set the used pins as inputs
	TRISCbits.TRISC7 = 1;	// USART will handle changing them from input to output
	
	TXSTAbits.TX9 = 0;		// 8 bit mode
	TXSTAbits.TXEN = 1;		// Transmit enabled
	TXSTAbits.SYNC = 0;		// Asynchoronous mode
	TXSTAbits.BRGH = 1;		// High speed
		
		// Receive Status and Control
	RCSTAbits.SPEN = 1; 	// Serial port enabled
	RCSTAbits.RX9 = 0;		// 8 bit mode
	RCSTAbits.CREN = 1;		// Continuous receive
	
//	SPBRG = 0x0A;			// 115 200 KBaud @ 20 MHz
	SPBRG = 0x40;			// 4800 baud @ 20MHz
		// Baud Rate control
	BAUDCTLbits.WUE = 0;
	BAUDCTLbits.ABDEN = 0;
}


