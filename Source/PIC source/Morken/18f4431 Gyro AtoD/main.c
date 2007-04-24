#include <p18f4431.h>
#include <delays.h>
#include <xlcd.h>
#include <adc.h>
#include <stdlib.h>
#include <usart.h>
#include <stdio.h>
#include <string.h>

#define VLSB 	2.44

unsigned int adcVal;
unsigned char NumString[10];
signed int NormalizedGyroValueX;
signed int NormalizedGyroValueY;
signed int GetNormalizedGyroValue(char ADC_CHx);
void main (void)
{	
	signed int rate = 0;
	signed int Vq, Vn;
	TRISA = 0xFF;//using RA0 and RA1 as analog inputs
	TRISB = 0x00;

	OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_12_TAD,
			ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_EXT, 0b01000000);

	ADCON1bits.VCFG0 = 1;
	ADCON1bits.VCFG1 = 0;
	ADCON1 = 0b01000000;
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;

	//open the USART at 
	OpenUSART ( USART_TX_INT_OFF &
				USART_RX_INT_ON &
				USART_ASYNCH_MODE &
				USART_EIGHT_BIT &
				USART_CONT_RX &
				USART_BRGH_HIGH, 10);//10 for 115200,21 for 57600

	
	NormalizedGyroValueX = GetNormalizedGyroValue(ADC_CH1);
	NormalizedGyroValueY = GetNormalizedGyroValue(ADC_CH0);
	while(1)
	{		
		//Y gyro ADC Channel////////////////
		SetChanADC(ADC_CH0);
		Delay10TCYx(100);
		ConvertADC();
		while(BusyADC()){}
		adcVal = ReadADC();

		
		Vn = adcVal - NormalizedGyroValueY;
		
		Vq = Vn * VLSB;
		
		rate = Vq / 2;
		
		sprintf(NumString,"Y: %d",rate );
		putsUSART(NumString);
		
		//X gyro ADC Channel/////////////////
		SetChanADC(ADC_CH1);
		Delay10TCYx(100);
		ConvertADC();
		while(BusyADC()){}
		adcVal = ReadADC();

		
		Vn = adcVal - NormalizedGyroValueX;
		
		Vq = Vn * VLSB;
		
		rate = Vq / 2;
		
		sprintf(NumString,"X: %d",rate );
		
		putsUSART(NumString);
		Delay10KTCYx(10);

	}
}
signed int GetNormalizedGyroValue(char ADC_CHx)
{

	int adcVal = 0;
	char NumSamples = 0;
	int mod = 0;	
	SetChanADC(ADC_CHx);
	Delay10TCYx(100);	
	for(NumSamples = 0; NumSamples<20; NumSamples++)
	{
		ConvertADC();
		while(BusyADC()){}
		adcVal += ReadADC();
	}
	mod = adcVal % 20;
	if (mod >= 10) 
	{
		return adcVal/20 + 1;
	}
	else
	{
		return adcVal/20;
	}
	
}
