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
	char data;
	TRISCbits.TRISC6 = 1;	// Set the used pins as inputs
	TRISCbits.TRISC7 = 1;	// USART will handle changing them from input to output
	
	TXSTAbits.TX9 = 0;		// 8 bit mode
	TXSTAbits.TXEN = 1;		// Transmit enabled
	TXSTAbits.SYNC = 0;		// Asynchoronous mode
	TXSTAbits.BRGH = 1;		//1 = High speed
							//0 = Low speed
							
		// Receive Status and Control
	RCSTAbits.RX9 = 0;		// 8 bit mode
	RCSTAbits.CREN = 1;		// Continuous receive
	PIE1bits.RCIE = 1;		// Rx Interrupt enable
	
	RCSTAbits.SPEN = 1; 	// Serial port enabled
	data = RCREG;			// dummy read
	SPBRG = 0x0A;			// 115 200 KBaud @ 20 MHz
//	SPBRG = 0x10;
//	SPBRG = 0x40;			// 19200 baud @ 20MHz
		// Baud Rate control
	BAUDCTLbits.WUE = 0;
	BAUDCTLbits.ABDEN = 0;
	INTCONbits.GIEL = 1;
	INTCONbits.GIEH = 1;
	
}


