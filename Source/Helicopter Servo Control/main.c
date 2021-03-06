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
#include <math.h>
#include <usart.h>
#include "variables.h"
#include "Serial_IO.h"
#include "adc.h"
#include "pwm.h"
#include "timers.h"
#include "USART.h"
#include "SPIComms.h"
#include "GPS.h"
#include "rpm.h"


/********************** Interrupt Function Definition ****************/

void TMR0_isr(void);
void SPI_isr (void);
void CCP1_isr(void);
void RX_isr(void);

unsigned char ByteNum = 0;

/************************* Interrupt Functions ***********************/
#pragma code high_int_vector = 0x08					/* change to interrupt page*/
void interrupt_at_high_vector(void)
{
	if(PIR1bits.SSPIF)
	{
		_asm GOTO SPI_isr _endasm
	}
	if(PIR1bits.CCP1IF)
	{
		_asm GOTO CCP1_isr _endasm
	}
	if(TIMER0FLAG)			// capture compare interrupt
	{
		_asm GOTO TMR0_isr _endasm
	}
	if(PIR1bits.RCIF)
	{
		_asm GOTO RX_isr _endasm
	}
}

#pragma code

#pragma interrupt RX_isr
void RX_isr(void)
{
	// used to receive characters RCIF is cleared on read
	GetGPSString(RCREG);
}

#pragma interrupt TMR0_isr
void TMR0_isr(void)
{
	TIMER0FLAG = 0; 		// clear the capture flag
	Nop();					// additional padding
	tFlag.newTickFlag = 1;	// Set Tick flag
	TMR0H = 0xCF;			// reset timer values, for a 40MHz crystal 0x9E59 = 10ms
	TMR0L += 0x2D;			// for a 20MHz crystal 0xCF2D = 10.0000 ms	
}

#pragma interrupt CCP1_isr
void CCP1_isr(void)
{
	CCP_interrupt();
	PIR1bits.CCP1IF = 0;//clear ccp1 interrupt flag		
	TMR1L = 0x00;
	TMR1H = 0x00;
}
#pragma interrupt SPI_isr
void SPI_isr (void)
{
	unsigned char Incoming_Data;
	unsigned char Reply;

		
	Incoming_Data = SPI_Read_Byte();
	

	Reply = SPI_State_Machine(Incoming_Data);

	SPI_Write(Reply);//fill the buffer with data back to the SPI master, it will go out the next time an SPI packet is received

	PIR1bits.SSPIF = 0;//clear the interrupt flag
}
//#pragma code 	/* return to code section */

void main(void)
{
/*********** Varible declaration *************/
	char TickCounter = 0;

/********** Beginning of main code ***********/
	SerialInit();
	PCPWMInit();
	ADCInit();
	SPI_Init();
	TimerInit();
	InitCCP();
	ANTI_COLL_LED = 0;
#ifdef USART_DEBUG
	ClearScreen();
	prepscreen();		// for debugging
#endif

/******** VARIABLE INITIALIZATION ************/
	INTCONbits.PEIE = 1;	// enable peripheral interrupt
	INTCONbits.GIE = 1;		// enable global interrupt
	
	SET_STATUS_LED1 = 0;	// for debugging purposes
	SET_STATUS_LED2 = 0;
	SET_ANTI_COLL_LED = 0;
	
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
					GetCompassAngle();
					ResetCompass();		// start compass module
					break;
				case 2:		// 10ms
					ScanADC();
					GetADCAverage();
					break;
				case 3:
					RPM_Ready = 0;		// check the motor RPM
					break;
				case 4:		// 30ms
					GetCompassValues();	// Get the compass values
					GetCompassAverage();// Find the average x-y values
					break;
				case 5:
					GetAxisValues();	// Get Tri-axis values
					GetAxisAverage();	// Get the average values for x-y-z
					TickCounter = 0;	// Reset Tick Counter
					break;
				default:
					break;
			}
			// Provisions for if we need 100ms timers
			if(tFlag.new100msTickFlag)
			{

			}
			// Provisions for if we need 1 second timers
			if(tFlag.new1sTickFlag)
			{
				LedStates();
			}
		}
	}
	while(1);
}
/*****************************  End of Main code *********************/


