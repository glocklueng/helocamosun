/*
	Title: 	USART.c

	Author:	Kyle Wong

	Date:	13/04/07
	
	Description:	This program will set up the USART and send a serial stream to the 
					serial port.


	Compiled with MCC18 compiler (v8.01PL3)
	and MPLAB ver 7.5
*/
/************************** Include Section **************************/
#include <p18f4431.h>
#include "USART.h"

/*************************** Program Control *************************/


/*************************** Definitions *****************************/
typedef union
{
	char byte[2];
	int D_byte;
	//long D_word;
}WORD;

/*************************** Global Variables ************************/
WORD Capture;

/*************************** Initialization Functions ****************/

void SerialInit(void);
/*************************** Interrupt Section ***********************/
/********************** Interrupt Function Definition ****************/
void CCPINT(void);

/************************* Interrupt Functions ***********************/
#pragma interrupt INTERRUPT
void INTERRUPT(void)
{
	if(PIR1bits.CCP1IF)			// capture compare interrupt
	{
		CCPINT();
	}
}

void CCPINT(void)
{
	PIR1bits.CCP1IF = 0; 		// clear the capture flag
	Capture.byte[0] = TMR1L;
	TMR1L = 0;					// Clear the low timer
	Capture.byte[1] = TMR1H;
	TMR1H = 0;					// Clear the high timer
}

#pragma code high_int_vector = 0x08					/* change to interrupt page*/
void interrupt_at_high_vector(void)
{
	_asm GOTO INTERRUPT _endasm
}

#pragma code 										/* return to default code section */

void main(void)
{
/*********** Varible declaration *****************/



	SerialInit();
/********** Beginning of main code **************/

/******** VARIABLE INITIALIZATION ************/
		TXREG = 's';
		while(!TXSTAbits.TRMT);
		TXREG = '2';
		while(!TXSTAbits.TRMT);
/******** START OF MAIN LOOP *****************/
	do
	{

	}
	while(1);

}
/*****************************  End of Main code *********************/

/*************************** Function Definitions ********************/



