#include <p18f4431.h>
#include <delays.h>
#include <xlcd.h>
#include <adc.h>
#include <stdlib.h>
#include <usart.h>
#include <stdio.h>

#define VLSB 	2.44
#define VREST 	1690

unsigned int adcVal;
unsigned char string[10];

void main (void)
{	
	signed int rate = 0;
	signed int Vq, Vn;
	TRISA = 0x03;//using RA0 and RA1 as analog inputs
	TRISAbits.TRISA3 = 1;
	TRISB = 0x00;

	OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_12_TAD,
			ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_EXT, 0b01000000);

	//ADCON1 = 0b00001101;//OpenADC doesnt seem to actually set this value to ADCON1
	ADCON1bits.VCFG0 = 0;
	ADCON1bits.VCFG1 = 1;
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;

	//open the USART at 
	OpenUSART ( USART_TX_INT_OFF &
				USART_RX_INT_ON &
				USART_ASYNCH_MODE &
				USART_EIGHT_BIT &
				USART_CONT_RX &
				USART_BRGH_HIGH, 10);//10 for 115200,21 for 57600


	while(1)
	{		
		
		SetChanADC(ADC_CH0);
		ConvertADC();
		while(BusyADC()){}
		adcVal = ReadADC();
		//Vq = adcVal * VLSB;
		//Vn = Vq - VREST;
		//rate = Vn / 2;
		
		Vn = adcVal - 609;
		
		Vq = Vn * VLSB;
		
		rate = Vq / 2;
		
		sprintf(string,"%d",rate );
		
		putsUSART(string);
		putcUSART(0x0A);
		Delay10KTCYx(10);

	}
}
