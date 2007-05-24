#include <p18f4431.h>
#include "rpm.h"
#include "timers.h"
#include "variables.h"

#define NUM_AVERAGES 10

unsigned int MOTORRPM = 0;
unsigned char RPM_Ready = 0;
unsigned char Average_num = 0;
unsigned long Samples_Sum = 0;
char RPM[2];

void InitCCP(void)
{
	//RC2 must be an input
	TRISCbits.TRISC2 = 1;
	/*
		CCPxM3:CCPxM0: CCPx Mode Select bits
		0000 =Capture/Compare/PWM disabled (resets CCPx module)
		0001 =Reserved
		0010 =Compare mode, toggle output on match (CCPxIF bit is set)
		0011 =Reserved
		0100 =Capture mode, every falling edge
		0101 =Capture mode, every rising edge
		0110 =Capture mode, every 4th rising edge
		0111 =Capture mode, every 16th rising edge
		1000 =Compare mode, Initialize CCP pin Low, on compare match force CCP pin High
		(CCPxIF bit is set)
		1001 =Compare mode, Initialize CCP pin High, on compare match force CCP pin Low
		(CCPxIF bit is set)
		1010 =Compare mode, Generate software interrupt-on-compare match (CCPxIF bit is set,
		CCP pin is unaffected)
		1011 =Compare mode, Trigger special event (CCP2IF bit is set)
		11xx =PWM mode
		*/

	CCP1CONbits.CCP1M3 = 0;
	CCP1CONbits.CCP1M2 = 1;
	CCP1CONbits.CCP1M1 = 1;
	CCP1CONbits.CCP1M0 = 0;

	PIR1bits.CCP1IF = 0;//clear CCP1 interrupt flag
	PIE1bits.CCP1IE = 1;//enable the interrupt enable
	
	IPR1bits.CCP1IP = 0;//CCP1IP: CCP1 Interrupt Priority bit
						//1 = High priority
						//0 = Low priority

	///CCP1 requires timer1 to be running
	T1CONbits.RD16 = 1; //RD16: 16-bit Read/Write Mode Enable bit
						//1 = Enables register read/write of TImer1 in one 16-bit operation
						//0 = Enables register read/write of Timer1 in two 8-bit operations

	T1CONbits.T1CKPS1 = 1;  //T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits
	T1CONbits.T1CKPS0 = 1;  //11 =1:8 Prescale value
							//10 =1:4 Prescale value
							//01 =1:2 Prescale value
							//00 =1:1 Prescale value
	
	T1CONbits.TMR1CS = 0;	//TMR1CS: Timer1 Clock Source Select bit
							//1 = External clock from pin RC0/T1OSO/T1CKI (on the rising edge)
							//0 = Internal clock (FOSC/4)

	T1CONbits.TMR1ON = 1;	//	TMR1ON: Timer1 On bit
							//1 = Enables Timer1
							//0 = Stops Timer1
							
	TIMER1FLAG = 0;			//clear timer1 interrupt flag
	PIE1bits.TMR1IE = 0;	//disable timer1 interrupt enable

	TMR1L = 0x00;
	TMR1H = 0x00;
}


void CCP_interrupt(void)
{
	if(!RPM_Ready)
	{
		Samples_Sum += CCPR1;
		if(	Average_num>=NUM_AVERAGES)
		{
			MOTORRPM = 0;	
			Average_num = 0;
			MOTORRPM = Samples_Sum / NUM_AVERAGES;
			RPM[0] = MOTORRPM >> 8;
			RPM[1] = (char)MOTORRPM;
			Samples_Sum = 0;
			RPM_Ready = 1;
		}
		Average_num++;
	}
}
