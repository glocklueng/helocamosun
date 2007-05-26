/*
adc.c
*/
#include <p18f4431.h>
#include "variables.h"
#include "adc.h"

WORD RangeFinder,
	 PitchGyro,
	 RollGyro,
	 Yaw_Gyro,
	 CurrentMeter,
	 VoltageMeter;
	 
signed int RangeAverage;
signed int 	PitchAverage;
signed int 	RollAverage;
signed int	YawAverage;

char Gyro[4];
char Voltages[4];
char Accoustic[2];

void ADCInit(void)
{
	TRISA  = 0x3F;	// RA5->RA0 inputs
	ADCHS  = 0x00;	// Group A,B,C,D select AN0,AN2,AN1,AN3
	ANSEL0 = 0x3F;	// ANS0->AN5 selected
	ANSEL1 = 0x00;	// ANS8 Digital I/O
	ADCON0 = 0x00;	// Autoconversion single shot, Single Channel, A/D on
	ADCON2 = 0x80;	// 16 Tad, FOSC/16	
	ADCON3 = 0xC0;	// A/D result buffer unimplemented, Trigger source disabled
	ADCON1 = 0x10;	// Vref+ = AVdd, Vref- = AVss, FIFO enabled
	
	ADCON0bits.ADON = 1;	// Start conversions	
}

int GetADC(void)
{
	WORD adc;
	if(!ADCON0bits.GO_DONE)
	{
		ADCON0bits.GO_DONE = 1;
	}
	while(ADCON0bits.GO_DONE);
	adc.byte[1] = ADRESH;
	adc.byte[0] = ADRESL;
	return adc.D_byte;
}

void ADCchannel(char channel)
{
	switch(channel)
	{
		case 0:		// Rangefinder
			ADCON0bits.ACMOD0 = 0;// Group A
			ADCON0bits.ACMOD1 = 0;// Group A
			ADCHS  = 0x00;	// AN0
			
		break;
		case 1:		// Pitch
			ADCON0bits.ACMOD0 = 1;// Group B
			ADCON0bits.ACMOD1 = 0;// Group B
			ADCHS  = 0x00;	// AN1
			
		break;
		case 2:		// Roll
			ADCON0bits.ACMOD0 = 0;// Group C
			ADCON0bits.ACMOD1 = 1;// Group C
			ADCHS  = 0x00;	// AN2
		break;
		case 3:		// Yaw
			ADCON0bits.ACMOD0 = 1;// Group D
			ADCON0bits.ACMOD1 = 1;// Group D
			ADCHS  = 0x00;	// AN3
		break;
		default:
		break;
	}
}
/*
ScanADC
*/
void ScanADC(void)
{
	ADCchannel(RANGEFINDER);
	RangeFinder.D_byte = GetADC();
	Accoustic[0] = RangeFinder.byte[1];
	Accoustic[1] = RangeFinder.byte[0];

	ADCchannel(PITCH);		// Pitch
	Nop();
	PitchGyro.D_byte = GetADC();
	

	ADCchannel(ROLL);		// Roll	
	Nop();
	RollGyro.D_byte = GetADC();
	Gyro[0] = RollGyro.byte[0];
	
	ADCchannel(YAW);		// Yaw
	Nop();
	Yaw_Gyro.D_byte = GetADC();
	
	ADCchannel(BATTERYVOLTAGE);	
	Nop();	
	VoltageMeter.D_byte = GetADC();
	
	ADCchannel(BATTERYCURRENT);
	Nop();	
	CurrentMeter.D_byte = GetADC();
}

void GetADCAverage(void)
{
	char loopCounter;
	static signed int Range[AVERAGEVALUE];
	static signed int Pitch[AVERAGEVALUE];
	static signed int Roll[AVERAGEVALUE];
	static signed int YawGyro[AVERAGEVALUE];
	static char Rangecount = 0;
	
	Range[Rangecount] = RangeFinder.D_byte;
	Pitch[Rangecount] = PitchGyro.D_byte;
	Roll[Rangecount] = RollGyro.D_byte;
	YawGyro[Rangecount] = Yaw_Gyro.D_byte;
	
	Rangecount++;
	if( Rangecount > AVERAGEVALUE - 1 )
	{
		Rangecount = 0;
	}
	
	for(loopCounter = 0; loopCounter < (AVERAGEVALUE - 1); loopCounter++)
	{
		RangeAverage += Range[loopCounter];
		PitchAverage += Pitch[loopCounter];
		RollAverage  += Roll[loopCounter];
		YawAverage   += YawGyro[loopCounter];
	}
	
	RangeAverage = RangeAverage / AVERAGEVALUE;
	PitchAverage = PitchAverage / AVERAGEVALUE;
	RollAverage = RollAverage / AVERAGEVALUE;
	YawAverage = YawAverage / AVERAGEVALUE;
}
