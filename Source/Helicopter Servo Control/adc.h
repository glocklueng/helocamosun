/*
ADC.h
*/
#include "variables.h"

#define RANGEFINDER		0
#define BATTERYCURRENT	1
#define BATTERYVOLTAGE	2
#define PITCH			3
#define ROLL			4
#define YAW				5

extern	WORD RangeFinder,
			 PitchGyro,
			 RollGyro,
			 Yaw_Gyro,
			 CurrentMeter,
			 VoltageMeter;
			 
extern signed int	RangeAverage;
extern signed int 	PitchAverage;
extern signed int 	RollAverage;
extern signed int	YawAverage;	
	 
void ADCInit(void);
int GetADC(void);
void ADCchannel(char channel);
void ScanADC(void);
void GetADCAverage(void);
