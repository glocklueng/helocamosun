/*
USART.c
*/
#include <p18f4431.h>
#include <stdlib.h>
#include <stdio.h>
#include "USART.h"
#include "Serial_IO.h"
#include "adc.h"
#include "variables.h"

char clearscreen[5] = {0x1B,0x5B,'2','J',0x00};
char outputstring[65] = "| X Tilt | Y Tilt | Z Axis | Compass X | Compass Y | Angle |";
char dottedline[65]   = "------------------------------------------------------------";
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

void convertstring(char *string, int value)
{
	char dif;
	string[0] = value / 1000;
	value -= ( string[0] * 1000);
	string[1] = value / 100;
	value -= ( string[1] * 100);
	value &= 0x00FF;
	string[2] = value / 10;
	
	string[3] = value % 10;
	for(dif = 0; dif < 4; dif++)
	{
		string[dif] += '0';
	}
}

void SendVariables(void)
{
	char outputstring[6];
	TRISCbits.TRISC2 = 0;

	setCursor(4,3);
//	convertstring(outputstring, XAxisAverage);
//	itoa(XAxisAverage,outputstring);
	sprintf(outputstring, "%04d", XAxisAverage);
	putsUSART(outputstring);
		
	setCursor(13,3);
//	convertstring(outputstring, XAxisAverage);
	sprintf(outputstring, "%04d", YAxisAverage);
	putsUSART(outputstring);
	
	setCursor(22,3);
//	convertstring(outputstring, Z_axis.D_byte);
	sprintf(outputstring, "%04d", ZAxisAverage);
	putsUSART(outputstring);

	setCursor(33,3);

	convertstring(outputstring, CompassXAverage);
	putsUSART(outputstring);
			
	setCursor(45,3);

	convertstring(outputstring, CompassYAverage);
	putsUSART(outputstring);
	
	setCursor(55,3);
	sprintf(outputstring, "%03d", CompassAngle);
	putsUSART(outputstring);
	
	setCursor(4,8);

	sprintf(outputstring, "%03d", RangeFinder.D_byte);
	putsUSART(outputstring);
		
	setCursor(13,8);
	
	sprintf(outputstring, "%03d", servos[1]);
	putsUSART(outputstring);
	
	setCursor(22,8);
	sprintf(outputstring, "%03d", servos[2]);
	putsUSART(outputstring);
	
	setCursor(34,8);
	sprintf(outputstring, "%03d", servos[3]);
	putsUSART(outputstring);
	
	setCursor(46,8);
	sprintf(outputstring, "%03d", servos[4]);
	putsUSART(outputstring);
//	LATCbits.LATC2 = 0;

}


