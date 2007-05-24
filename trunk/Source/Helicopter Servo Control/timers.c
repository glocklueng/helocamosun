/*
timers.c
*/
#include <p18f4431.h>
#include "timers.h"
#include "variables.h"

#define STATUS_LED1	LATDbits.LATD4
#define SET_STATUS_LED1 TRISDbits.TRISD4
#define STATUS_LED2 LATBbits.LATB0
#define SET_STATUS_LED2 TRISBbits.TRISB0


TIMEFLAGS tFlag;
CONTROL cFlag;
TIMERS tDelay;

/*
Function: TimerInit
Description: Initializes the timer to interrupt every 100ms
			 Change the values of TMR1H/L to change the interrupt
*/
void TimerInit(void)
{
	T0CONbits.TMR0ON = 1;	// Turn on Timer 0
	T0CONbits.T016BIT = 0;  // Configured as 16-bit timer
	T0CONbits.T0CS = 0;		// Internal instruction cycle clock
	T0CONbits.T0SE = 0; 	// Increment on Low to High edge
	T0CONbits.PSA = 0;		// Timer0 clock input comes from prescaler output
	T0CONbits.T0PS2 = 0;
	T0CONbits.T0PS1 = 0;	// Set prescaler to 1:8
	T0CONbits.T0PS0 = 1;

	TMR0H = 0xCF;			// Initialise timer 0
	TMR0L = 0x2D;			// for 10 ms interrupt ***debug change to 100ms

	INTCONbits.T0IE = 1;	// Timer 0 interrupt enable
	
//	T1CONbits.T1RUN = 1;
//	T1CONbits.T1CKPS1=1;	// set prescaler to 1:8
//	T1CONbits.T1CKPS0=0;
//	T1CONbits.TMR1CS=0;		// use internal clock
//	T1CONbits.T1OSCEN=1;	// Oscillator enabled
//	T1CONbits.TMR1ON=1;		// Start timer
//	
//	TIMER1FLAG = 0;			// clear timer1 int flag
//	PIE1bits.TMR1IE = 1;	// enable timer 1 interrupt
//	IPR1bits.TMR1IP = 1;	// set to high priority interrupt
}

void TimeKeeping(void)
{
	tFlag.new100msTickFlag = 0; // clear the flag
	tFlag.new1sTickFlag = 0;

	tDelay.delay100ms++;			// counts up to 10
	
	if(tDelay.delay100ms >= 10)	// after 100ms, set a 100ms Flag
	{
		tFlag.new100msTickFlag = 1;
		tDelay.delay100ms = 0;
		tDelay.delay1s++;
		if(tDelay.delay1s >= 10)
		{
			tFlag.new1sTickFlag = 1;
			tDelay.delay1s = 0;
		}
	}
}
/*
Function LedStates
Description:  Indicates to the user the state at which the microcontroller is functioning
*/
void LedStates(void)
{
	switch(command[0])
	{
		case MAIN:
			STATUS_LED1 = 1; 			// on
			STATUS_LED2 ^= 1;			// toggle		
			break;
		case DEBUG:
			STATUS_LED1 ^= 1; 			// toggle
			STATUS_LED2 = 1;			// on		
			break;
		default:
			STATUS_LED1 = 1; 			// on
			STATUS_LED2 = 0;			// off
			break;
	}
}
