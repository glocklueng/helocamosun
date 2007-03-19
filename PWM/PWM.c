/*
	Title: 	PWM.c

	Author:	Kyle Wong

	Date:	17/03/07
	
	Description:	Control a 20ms pulse width modulated signal. 

	Compiled with MCC18 compiler 
	and MPLAB ver 7.5
*/


/************************** Include Section **************************/
#include <p18f4431.h>
//#include <p18f4620.h>
#include <delays.h>


/*************************** Program Control *************************/

/*************************** Defines *********************************/


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

void PCPWMInit(void);

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



/********** Beginning of main code **************/


/********* TROUBLESHOOTING TOOL *********
		while(BusyUSART());
		TXREG = string2[17];
*/

/******** VARIABLE INITIALIZATION ************/


/******** START OF MAIN LOOP *****************/
	TRISB = 0X00;	// PORTB ALL OUTPUT
	TRISDbits.TRISD7 = 0;
	PWMCON0 = 0X7F; // ALL ODD I/O PINS ENABLED, PWM I/Os IN INDEPENDANT MODE
	PWMCON1 = 0X00;
	DTCON = 0X00; 	// DEAD TIME CONTROL
	FLTCONFIG = 0X00;// FAULT CONFIGURATION
	OVDCOND = 0XFF;	// OUTPUTS CONTROLED BY PWMS
	OVDCONS = 0X00;
	
	PTMRH = 0X00;	// PWM TIME BASE
	PTMRL = 0X00;
	
	PTPERH = 0X0C;	// TIME BASE PERIOD
	PTPERL = 0X35;	// NEED TO ADJUST FOR 20ms PERIOD 0x0C35
	
	// PWM DUTY CYCLES
	PDC0L = 0X39;	// 0.5ms, minimum duty cycle
	PDC0H = 0X01;
	PDC1L = 0X1B;	// 2.5ms, maximum duty cycle
	PDC1H = 0X06;
	PDC2L = 0XAA;	// 1.5ms, 50% duty cycle
	PDC2H = 0X03;	
	PDC3L = 0X10;	// some random value
	PDC3H = 0X04;
	
	// START TIMERS
	PTCON0 = 0xFC;	// POSTSCALE 1:16, TIMEBASE INPUT CLOCK FOSC/256: EDGE TRIGGERED
	PTCON1 = 0x80;	// START TIMERS AND COUNT UP
	
	do
	{
/******** INITILIZE LOOP VARIABLES ***********/
	if(PORTBbits.RB4)
	{
		Nop();
	}
	if(!PORTBbits.RB4)
	{
		Nop();
	}

	}
while(1);
}
/*****************************  End of Main code *********************/

void PCPWMInit(void)
{

}
