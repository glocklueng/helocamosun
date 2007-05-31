/*
pwm.c
*/
#include <p18f4431.h>
#include "USART.h"
#include "pwm.h"
#include "variables.h"

char servos[5];

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
	
	PDC0H 	= 0x01;		// Pitch
	PDC0L 	= 0xC5;		// 1.43ms, minimum duty cycle 0x0620
	
	PDC1H 	= 0x01;		// Roll
	PDC1L 	= 0xD8;		// 536us, maximum duty cycle
	
	PDC2H 	= 0x01;		// Yaw
	PDC2L 	= 0xB2;		// 526us, 50% duty cycle
	
	PDC3H 	= 0x01;		// Collective
	PDC3L 	= 0xA7;		// some random value
	
	// START TIMERS
	PTCON0 	= 0x0C;	// POSTSCALE 1:16, TIMEBASE INPUT CLOCK FOSC/256: EDGE TRIGGERED
	PTCON1 	= 0x80;	// START TIMERS AND COUNT UP
}

void UpdatePWM(void)
{
	// PITCH
	short temp = 0;
	float fTemp;
//	PDC0H = Accoustic[0];		// servo[1]
//	PDC0L = Accoustic[1];
//	
//	TXREG = Accoustic[1];
//	while(BusyUSART());	
//	TXREG = Accoustic[0];
//	while(BusyUSART());
	fTemp = servos[1] * PITCH_CORRECTION_FACTOR;
	temp = ((short)fTemp)+PITCH_MIN_VAL;
	PDC0L = (char)temp;
	PDC0H = (char)(temp>>8);
	
//	PDC0L = servos[1];		// 1.43ms, minimum duty cycle 0x0620
//	TXREG = temp;
//	while(BusyUSART());
//	TXREG = temp>>8;
//	while(BusyUSART());
	// ROLL
	fTemp = servos[2] * ROLL_CORRECTION_FACTOR;
	temp = ((short)fTemp)+ROLL_MIN_VAL;
	PDC1L = (char)temp;		// 536us, maximum duty cycle
	PDC1H = (char)(temp>>8);
//	TXREG = servos[2];
//	while(BusyUSART());

//	while(BusyUSART());	
	// YAW
	fTemp = servos[3] * YAW_CORRECTION_FACTOR;
	temp = ((short)fTemp)+YAW_MIN_VAL;
	PDC2L = (char)temp;
	PDC2H = (char)(temp>>8);		// 526us, 50% duty cycle
//	TXREG = servos[3];
//	while(BusyUSART());

//	while(BusyUSART());	
	// COLLECTIVE
	fTemp = servos[4] * COLLECTIVE_CORRECTION_FACTOR;
	temp = ((short)fTemp)+COLLECTIVE_MIN_VAL;
	PDC3L = (char)temp;
	PDC3H = (char)(temp>>8);
}

