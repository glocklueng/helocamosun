/*
pwm.c
*/
#include <p18f4431.h>
#include "USART.h"
#include "variables.h"

char servos[6];

void PCPWMInit(void)
{
	TRISB = 0X00;		// PORTB ALL OUTPUT
	TRISDbits.TRISD7 = 0;// PWM7
	PWMCON0 = 0X7F; 	// ALL ODD I/O PINS ENABLED, PWM I/Os IN INDEPENDANT MODE
	PWMCON1 = 0X00;
	DTCON = 0X00; 		// DEAD TIME CONTROL
	FLTCONFIG = 0X00;	// FAULT CONFIGURATION
	OVDCOND = 0XFF;		// OUTPUTS CONTROLED BY PWMS
	OVDCONS = 0X00;
	
	PTMRH = 0X00;		// PWM TIME BASE
	PTMRL = 0X00;
	
	PTPERH = 0X16;		// TIME BASE PERIOD
	PTPERL = 0X1A;		// NEED TO ADJUST FOR 20.2ms PERIOD 0x161A
	
		// PWM DUTY CYCLES
	
//	PDC0L = 0XC0;		// 1.43ms, minimum duty cycle 0x0620
	PDC0H = 0X01;
//	PDC1L = 0XA0;		// 536us, maximum duty cycle
	PDC1H = 0X01;
//	PDC2L = 0XA2;		// 526us, 50% duty cycle
	PDC2H = 0X01;
//	PDC3L = 0X06;		// some random value
	PDC3H = 0X01;
	
	// START TIMERS
	PTCON0 = 0x0C;	// POSTSCALE 1:16, TIMEBASE INPUT CLOCK FOSC/256: EDGE TRIGGERED
	PTCON1 = 0x80;	// START TIMERS AND COUNT UP
}

void UpdatePWM(void)
{
	// PITCH
	PDC0L = Accoustic[1];		// 1.43ms, minimum duty cycle 0x0620
//	putcUSART(servos[1]);
	while(BusyUSART());	
	// ROLL
	PDC1L = servos[2];		// 536us, maximum duty cycle
//	putcUSART(servos[2]);
	while(BusyUSART());	
	// YAW
	PDC2L = servos[3];		// 526us, 50% duty cycle
//	putcUSART(servos[3]);
	while(BusyUSART());	
	// COLLECTIVE
	PDC3L = servos[4];		// some random value
//	putcUSART(servos[4]);
	while(BusyUSART());	
}

