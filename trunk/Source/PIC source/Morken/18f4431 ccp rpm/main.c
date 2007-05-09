#include <p18f4431.h>
#include <usart.h>
#include <string.h>
#include <stdio.h>
#include <delays.h>
#include "rpm.h"



/////LOW PRIORITY SPI ISR CODE///////////////

void CCP1_isr (void);
#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
	if(PIR1bits.CCP1IF)
	{
		_asm GOTO CCP1_isr _endasm
	}
}
#pragma code // return to the default code section 
#pragma interrupt CCP1_isr

/////////////////////////////////////////////


////HIGH PRIORITY SPI ISR CODE////////////////
/*
void CCP1_isr (void);
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
	if(PIR1bits.CCP1IF)
	{
		_asm GOTO CCP1_isr _endasm
	}
}
#pragma code // return to the default code section 
#pragma interrupt CCP1_isr
*/
//////////////////////////////////////////////

void CCP1_isr(void)
{
	TRISCbits.TRISC2 = 1;
	CCP_interrupt();
	
	TMR1L = 0x00;
	TMR1H = 0x00;
	
	PIR1bits.CCP1IF = 0;//clear CCP1 interrupt flag		
TRISCbits.TRISC2 = 1;
}

char RPMString[50];
	
void main (void)
{
	float RPMf = 0;
	
	TRISB = 0x00;
	TRISC = 0xFF;
	
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;


	//open the USART at 
	OpenUSART ( USART_TX_INT_OFF &
				USART_RX_INT_ON &
				USART_ASYNCH_MODE &
				USART_EIGHT_BIT &
				USART_CONT_RX &
				USART_BRGH_HIGH, 10);//10 for 115200,21 for 57600
	IPR1bits.RCIP = 1;//set UART interrupt to high priority
	
	InitCCP();
	
	while(1)
	{
		Delay10KTCYx(100);
				
		if(RPM_Ready)
		{	
			RPMf = 1.0 / (float)RPM * 2440483.0 + 4.0;

			sprintf(RPMString, "%d", (int)RPMf);
	
			putsUSART(RPMString);
			RPM_Ready = 0;
		}
	}
}
