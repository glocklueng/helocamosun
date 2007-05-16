/*
USART.c
*/
#include <p18f4431.h>
#include <stdio.h>
#include "USART.h"
#include "Serial_IO.h"
#include "adc.h"
#include "variables.h"


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
//	SPBRG = 0x40;			// 19200 baud @ 20MHz
		// Baud Rate control
	BAUDCTLbits.WUE = 0;
	BAUDCTLbits.ABDEN = 0;
}

#ifdef USART_DEBUG

char clearscreen[5] = {0x1B,0x5B,'2','J',0x00};
char RollString[5] = "Roll";
char YawString[4]  = "Yaw";
char CollectiveString[11] = "Collective";

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
	putsUSART("Distance");

	setCursor(2,6);
	putsUSART("Pitch");
	
	setCursor(3,6);
	putsUSART("Compass X");
	
	setCursor(4,6);
	putsUSART("Compass Y");
}

void UpdateADC(void)
{
	char outputstring[5]={0};
	
	setCursor(55,3);	
	sprintf(outputstring, "%04d", RangeAverage);
	putsUSART(outputstring);
}

void UpdateAccelerometer(void)
{
	char outputstring[5]={0};

	setCursor(4,3);		// Pitch
	sprintf(outputstring, "%04d", XAxisAverage);
	putsUSART(outputstring);
		
	setCursor(13,3);	// Roll
	sprintf(outputstring, "%04d", YAxisAverage);
	putsUSART(outputstring);	
}
void UpdateCompass(void)
{
	char outputstring[5]={0};
	
	setCursor(22,3);	// Yaw
	sprintf(outputstring, "%04d", CompassAverageAngle);
	putsUSART(outputstring);

	setCursor(31,3);	// Compass X Average value
	sprintf(outputstring, "%04d", CompassXAverage);
	putsUSART(outputstring);
			
	setCursor(43,3);	// Compass Y Average value
	sprintf(outputstring, "%04d", CompassYAverage);
	putsUSART(outputstring);
}
void UpdateServos(void)
{
	char outputstring[5]={0};
	
	setCursor(4,8);		// Servo Pitch
	sprintf(outputstring, "%04d", servos[1]);
	putsUSART(outputstring);
		
	setCursor(13,8);	// Servo Roll 
	sprintf(outputstring, "%04d", servos[2]);
	putsUSART(outputstring);
	
	setCursor(22,8);	// Servo Yaw
	sprintf(outputstring, "%04d", servos[3]);
	putsUSART(outputstring);
	
	setCursor(32,8);	// Servo Collective
	sprintf(outputstring, "%04d", servos[4]);
	putsUSART(outputstring);	
}

#endif
