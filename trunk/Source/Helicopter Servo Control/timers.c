/*
timers.c
*/
#include <p18f4431.h>
#include "timers.h"

#define STATUS_LED1	LATDbits.LATD4
#define SET_STATUS_LED1 TRISDbits.TRISD4
#define STATUS_LED2 LATBbits.LATB6
#define SET_STATUS_LED2 TRISBbits.TRISB6

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

	PIR1bits.C = 0;			// clear timer1 int flag
	PIE1bits.TMR1IE = 1;	// enable timer 1 interrupt
	IPR1bits.TMR1IP = 1;	// set to high priority interrupt
	
	T1CONbits.RD16 = 0;		// read/write to timer in 2 8-bit operations

	TMR1H = 0xCF;			// Initialise timer 1
	TMR1L = 0x2D;			// for 10 ms interrupt ***debug change to 100ms

	T1CONbits.T1RUN = 1;
	T1CONbits.T1CKPS1=1;	// set prescaler to 1:8
	T1CONbits.T1CKPS0=0;
	T1CONbits.TMR1CS=0;		// use internal clock
	T1CONbits.T1OSCEN=1;		// Oscillator enabled
	T1CONbits.TMR1ON=1;		// Start timer
	
	INTCONbits.PEIE = 1;	// enable peripheral interrupt
	INTCONbits.GIE = 1;		// enable global interrupt

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

void LedStates(void)
{
	if(cFlag.bist)
	{
		STATUS_LED1 = 1; // start with the LED on
		STATUS_LED2 = 1; // start with the LED on
	}
	if(cFlag.startup)
	{
		STATUS_LED2 ^= 1;
		STATUS_LED2 != STATUS_LED1;
	}
	if(cFlag.main)
	{
		STATUS_LED1 = 1; // start with the LED on
		STATUS_LED2 ^= 1;
	}	
}
