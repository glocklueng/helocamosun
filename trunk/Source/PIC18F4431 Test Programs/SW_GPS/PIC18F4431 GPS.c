/*
	Title: 	PIC18F4431 SPI.c

	Author:	Kyle Wong

	Date:	15/04/07
	
	Description:	This program will initiate communications with an external EEPROM via
					the SPI port.

	Notes:	

	Compiled with MCC18 compiler (v8.01PL3)
	and MPLAB ver 7.5
*/
/************************** Include Section **************************/
#include <p18f4431.h>
#include <USART.h>
#include <delays.h>
#include "USART.h"
#include "GPS.h"

/*************************** Program Control *************************/


/*************************** Defines *********************************/


/*************************** Definitions *****************************/
typedef union
{
	char byte[2];
	int D_byte;
	long D_word;
}WORD;

/*************************** Global Variables ************************/
WORD Capture;

/*************************** Initialization Functions ****************/

/*************************** Function Definitions ********************/


/*************************** Interrupt Section ***********************/
void CCPINT(void);
#pragma interrupt INTERRUPT
void INTERRUPT(void)
{
	if(PIR1bits.CCP1IF)
	{
		CCPINT();
	}
}

void CCPINT(void)
{
	PIR1bits.CCP1IF = 0; // clear the capture flag
	Capture.byte[0] = TMR1L;
	TMR1L = 0;	// Clear the timers
	Capture.byte[1] = TMR1H;
	TMR1H = 0;	// Clear the timers
}

#pragma code high_int_vector = 0x08
void interrupt_at_high_vector(void)
{
	_asm GOTO INTERRUPT _endasm
}
#pragma code /* return to default code section */

void main(void)
{
	char	GPS_COMMAND = 0;
	SerialInit();
	TRISBbits.TRISB0 = 0;	// Debugging port for timing the USART
	do
	{
		Delay10KTCYx(100);
		
		LATBbits.LATB0 = 1;
		
		SendGPSCommand(GPS_COMMAND);
			
		GPS_COMMAND++;
		
		if(GPS_COMMAND > 9)
		{
			GPS_COMMAND = 0;
		}
		LATBbits.LATB0 = 0;
	}
	while(1);
}

