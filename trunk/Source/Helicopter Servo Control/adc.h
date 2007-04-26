/*
ADC.h
*/
#include "variables.h"

#define RANGEFINDER		0
#define GYRO1			1
#define GYRO2			2
#define GYRO3			3

extern	WORD RangeFinder,
			 Gyro1,
			 Gyro2,
			 Gyro3,
			 CurrentMeter,
			 VoltageMeter;
extern int	RangeAverage;	
	 
void ADCInit(void);
int GetADC(void);
void ADCchannel(char channel);
void ScanADC(void);
void GetADCAverage(void);
