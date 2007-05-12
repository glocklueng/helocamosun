/*
	Title: 	Range Finder.c

	Author:	Kyle Wong

	Date:	8/04/07
	
	Description:	Read an analog voltage for a range finder and output an equivalent binary value 

	Compiled with MCC18 compiler 
	and MPLAB ver 7.5
*/


/************************** Include Section **************************/
#include <p18f4431.h>
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
	//PCPWMInit();

	
	TRISA  = 0x3F;	// RA5->RA0 inputs
	ADCHS  = 0x00;	// Group A,B,C,D select AN0,AN2,AN1,AN3
	ANSEL0 = 0x3F;	// ANS0->AN5 selected
	ANSEL1 = 0x00;	// all Digital I/O
	ADCON0 = 0x00;	// Autoconversion single shot, Single Channel, A/D on
	ADCON2 = 0xBD;	// 16 Tad, FOSC/16
	ADCON3 = 0xC0;	// A/D result buffer unimplemented, Trigger source disabled
	ADCON1 = 0x10;	// Vref+ = AVdd, Vref- = AVss, FIFO enabled
	
	ADCON0bits.ADON = 1;	// Start conversions
	PWMCON0 = 0x00;
	TRISB = 0x00;
	LATB = 0xF0;
	
	ADCON0bits.ACMOD0 = 0;// Group A
	ADCON0bits.ACMOD1 = 0;// Group 1
	ADCHS  = 0x00;	// AN0
	
	do
	{
		if(!ADCON0bits.GO_DONE)
		{
			ADCON0bits.GO_DONE = 1;
		}
		{}
		while(ADCON0bits.GO_DONE);
		LATB = ADRESL;
	}
while(1);
}
/*****************************  End of Main code *********************/

void PCPWMInit(void)
{
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
	
	PTPERH = 0X16;	// TIME BASE PERIOD
	PTPERL = 0X1A;	// NEED TO ADJUST FOR 20.2ms PERIOD 0x162A
	
	// PWM DUTY CYCLES
	PDC0H = 0X01;
	PDC0L = 0XC0;	// 1.43ms, minimum duty cycle 0x0620
	PDC1H = 0X00;
	PDC1L = 0XA0;	// 536us, maximum duty cycle
	PDC2H = 0X00;	
	PDC2L = 0XA2;	// 526us, 50% duty cycle
	PDC3L = 0X06;	// some random value
	PDC3H = 0X50;
	
	// START TIMERS
	PTCON0 = 0xFC;	// POSTSCALE 1:16, TIMEBASE INPUT CLOCK FOSC/256: EDGE TRIGGERED
	PTCON1 = 0x80;	// START TIMERS AND COUNT UP
}
