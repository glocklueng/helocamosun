/*
pwm.c
*/
#include <p18f4431.h>
#include "USART.h"
#include "variables.h"

char servos[4];

void PCPWMInit(void)
{
	TRISB 	= 0x00;		// PORTB ALL OUTPUT
	TRISDbits.TRISD7 = 0;// PWM7
	PWMCON0 = 0x7F; 	// ALL ODD I/O PINS ENABLED, PWM I/Os IN INDEPENDANT MODE
	PWMCON1 = 0x00;
	DTCON 	= 0x00; 		// DEAD TIME CONTROL
	FLTCONFIG = 0x00;	// FAULT CONFIGURATION
	OVDCOND = 0xFF;		// OUTPUTS CONTROLED BY PWMS
	OVDCONS = 0x00;
	
	PTMRH 	= 0x00;		// PWM TIME BASE
	PTMRL 	= 0x00;
	
	PTPERH 	= 0x16;		// TIME BASE PERIOD
	PTPERL 	= 0x1A;		// NEED TO ADJUST FOR 20.2ms PERIOD 0x161A
	
		// PWM DUTY CYCLES
	
	PDC0L 	= 0xC0;		// 1.43ms, minimum duty cycle 0x0620
	PDC0H 	= 0x01;
	PDC1L 	= 0xA0;		// 536us, maximum duty cycle
	PDC1H 	= 0x01;
	PDC2L 	= 0xA2;		// 526us, 50% duty cycle
	PDC2H 	= 0x01;
	PDC3L 	= 0x06;		// some random value
	PDC3H 	= 0x01;
	
	// START TIMERS
	PTCON0 	= 0x0C;	// POSTSCALE 1:16, TIMEBASE INPUT CLOCK FOSC/256: EDGE TRIGGERED
	PTCON1 	= 0x80;	// START TIMERS AND COUNT UP
}

void UpdatePWM(void)
{
	// PITCH
//	PDC0H = Accoustic[0];		// servo[1]
//	TXREG = Accoustic[0];
//	while(BusyUSART());	
//	TXREG = Accoustic[1];
//	while(BusyUSART());
	PDC0L = servos[1];		// 1.43ms, minimum duty cycle 0x0620
	// ROLL
	PDC1L = servos[2];		// 536us, maximum duty cycle
//	while(BusyUSART());	
	// YAW
	PDC2L = servos[3];		// 526us, 50% duty cycle
//	while(BusyUSART());	
	// COLLECTIVE
	PDC3L = servos[4];		// some random value
//	while(BusyUSART());	
}

