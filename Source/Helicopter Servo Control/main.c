/*
	Title: 	main.c

	Author:	Kyle Wong

	Date:	18/03/07
	
	Description: Main servo control system
				 This program will communicate with a tri-axis accelerometer,
				 and compass module and control the appropriate servos required
				 to maintain a level flight.
				 
	Revision History:
	1.000	Included PWM, Serial I/O
	1.001	Added ADC input and output value to PWM
	1.002	Cleaned up code, created functions out of original code
			Added USART capabilities, and the compass module is working
	
	Compiled with MCC18 compiler
	and MPLAB ver 7.5
*/

/************************** Include Section **************************/
#include <p18f4431.h>
#include <delays.h>
#include <math.h>
#include "variables.h"
#include "Serial_IO.h"
#include "adc.h"
#include "pwm.h"
#include "timers.h"
#include "USART.h"
#include "SPIComms.h"

/********************** Interrupt Function Definition ****************/
void CCPINT(void);
void SPI_isr (void);

/************************* Interrupt Functions ***********************/
#pragma code high_int_vector = 0x08					/* change to interrupt page*/
void interrupt_at_high_vector(void)
{
	if(PIR1bits.SSPIF)
	{
		_asm GOTO SPI_isr _endasm
	}
	if(PIR1bits.C)			// capture compare interrupt
	{
		_asm GOTO CCPINT _endasm
	}
}

#pragma code

#pragma interrupt SPI_isr
void SPI_isr (void)
{
	unsigned char Incoming_Data;
	unsigned char Reply;
	
	Incoming_Data = SPI_Read_Byte();
//	putcUSART(Incoming_Data);//debugging
	Reply = SPI_State_Machine(Incoming_Data);
	SPI_Write(Reply);//fill the buffer with data back to the SPI master, it will go out the next time an SPI packet is received
//	putcUSART(Reply);//debugging
	
	PIR1bits.SSPIF = 0;//clear the interrupt flag
}
#pragma interrupt CCPINT
void CCPINT(void)
{
	PIR1bits.C = 0; 		// clear the capture flag
	Nop();					// additional padding
	tFlag.newTickFlag = 1;	// Set Tick flag
	TMR1H = 0xCF;			// reset timer values, for a 40MHz crystal 0x9E59 = 10ms
	TMR1L += 0x2D;			// for a 20MHz crystal 0xCF2D = 10.0000 ms
}

#pragma code 	/* return to code section */

void main(void)
{
/*********** Varible declaration *************/
	char TickCounter;
	
/********** Beginning of main code ***********/
	
	PCPWMInit();
	ADCInit();
	SerialInit();
	SPI_Init();
	TimerInit();
	ClearScreen();
	prepscreen();		// for debugging
/******** VARIABLE INITIALIZATION ************/
	INTCONbits.PEIE = 1;	// enable peripheral interrupt
	INTCONbits.GIE = 1;		// enable global interrupt

	TickCounter = 0;
	tDelay.delay1s = 0;
	tDelay.delay100ms = 0;

	cFlag.bist = 0;
	cFlag.startup = 0;
	cFlag.main = 1;
	LedStates();

/******** START OF MAIN LOOP *****************/

	do
	{
		if(tFlag.newTickFlag)
		{
			tFlag.newTickFlag=0;		// Set all flags to zero
			TickCounter++;				// cycles 50ms period
			TimeKeeping();

			switch(TickCounter)
			{
				case 1:		
					ResetCompass();		// start compass module
					break;
				case 2:		// 10ms
					ScanADC();
					GetADCAverage();
					break;
				case 4:		// 30ms
					GetCompassValues();
					if(Compass_X.D_byte != 0x00)
					{
						GetCompassAverage();
					}
					break;
				case 5:
					GetAxisValues();
					GetAxisAverage();
					SendVariables();	// for debugging
					TickCounter = 0;	// Reset Tick Counter
					break;
				default:
					break;
			}
			if(tFlag.new1sTickFlag)
			{
				LedStates();
			}
		}
	}
	while(1);
}
/*****************************  End of Main code *********************/


