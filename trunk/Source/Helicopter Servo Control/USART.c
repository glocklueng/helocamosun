/*
USART.c
*/
#include <p18f4431.h>
#include "USART.h"
#include "Serial_IO.h"
#include "adc.h"
#include "variables.h"

char clearscreen[5] = {0x1B,0x5B,'2','J',0x00};
char outputstring[65] = "| X Axix | Y Axis | Z Axis | Compass X | Compass Y |";
char dottedline[65]   = "----------------------------------------------------";
char datastring[65]   = "| Range  | Pitch  |  Roll  |    Yaw    | Collective| ";

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
	//TXSTAbits.TRMT = 1;		// TSR empty
	TXSTAbits.BRGH = 1;		// High speed
		
		// Receive Status and Control
	RCSTAbits.SPEN = 1; 	// Serial port enabled
	RCSTAbits.RX9 = 0;		// 8 bit mode
	RCSTAbits.CREN = 1;		// Continuous receive
	
	SPBRG = 0x0A;			// 115 200 KBaud @ 20 MHz
	
		// Baud Rate control
//	BAUDCTLbits.BRG16 = 1;
	BAUDCTLbits.WUE = 0;
	BAUDCTLbits.ABDEN = 0;
	
//	PIE1bits.RCIE = 1;		// enable receiver interrupt
//	IPR1bits.RCIP = 0;		// High priority interrupt
// 	if(PIR1bits.RCIF)
//	{
//		data = RCREG;// used to receive characters RCIF is cleared on read
//	}

}
void setCursor( char x, char y)
{
	WriteUSART(0x1B);
	while(BusyUSART());
	WriteUSART(0x5B);
	while(BusyUSART());
	
	if(y >= 10)
	{
		WriteUSART('0'+( y / 10 ));
		while(BusyUSART());
	}
	WriteUSART('0' + ( y % 10 ));
	while(BusyUSART());
	WriteUSART(';');
	while(BusyUSART());
	
	if(x >= 10)
	{
		WriteUSART('0' + ( x / 10 ));
		while(BusyUSART());
	}
	WriteUSART('0' + ( x % 10 ));
	while(BusyUSART());
	WriteUSART('H');
	while(BusyUSART());
}

void ClearScreen(void)
{
	putsUSART(clearscreen);
	WriteUSART(0x0D);	
}
void prepscreen(void)
{
	setCursor(1,1);
	putsUSART(outputstring);
	
	setCursor(1,2);
	putsUSART(dottedline);
	
	setCursor(1,6);
	putsUSART(datastring);
	setCursor(1,7);
	putsUSART(dottedline);
}


void SendVariables(void)
{
	setCursor(4,3);
	WriteUSART(((X_axis.byte[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( X_axis.byte[1] & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(((X_axis.byte[0] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( X_axis.byte[0] & 0x0F)+0x30);
	while(BusyUSART());
	
	setCursor(13,3);

	WriteUSART(((Y_axis.byte[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Y_axis.byte[1] & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(((Y_axis.byte[0] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Y_axis.byte[0] & 0x0F)+0x30);
	while(BusyUSART());
	
	setCursor(22,3);

	WriteUSART(((Z_axis.byte[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Z_axis.byte[1] & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(((Z_axis.byte[0] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Z_axis.byte[0] & 0x0F)+0x30);
	while(BusyUSART());

	setCursor(33,3);

	WriteUSART(((Compass_X.byte[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Compass_X.byte[1] & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(((Compass_X.byte[0] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Compass_X.byte[0] & 0x0F)+0x30);
	while(BusyUSART());
		
	setCursor(45,3);

	WriteUSART(((Compass_Y.byte[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Compass_Y.byte[1] & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(((Compass_Y.byte[0] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( Compass_Y.byte[0] & 0x0F)+0x30);
	while(BusyUSART());

	setCursor(4,8);

	WriteUSART(((RangeFinder.byte[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( RangeFinder.byte[1] & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(((RangeFinder.byte[0] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART(( RangeFinder.byte[0] & 0x0F)+0x30);
	while(BusyUSART());
	
	setCursor(14,8);
	WriteUSART(((servos[1] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART((servos[1] & 0x0F)+0x30);
	while(BusyUSART());
	
	setCursor(23,8);
	WriteUSART(((servos[2] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART((servos[1] & 0x0F)+0x30);
	while(BusyUSART());
	
	setCursor(34,8);
	WriteUSART(((servos[3] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART((servos[1] & 0x0F)+0x30);
	while(BusyUSART());
	
	setCursor(46,8);
	WriteUSART(((servos[4] >> 4) & 0x0F)+0x30);
	while(BusyUSART());
	WriteUSART((servos[1] & 0x0F)+0x30);
	while(BusyUSART());
}


