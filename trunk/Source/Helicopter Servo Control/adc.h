/*
ADC.h
*/
#include "variables.h"

#define RANGEFINDER		0
#define PITCH			1
#define ROLL			2
#define YAW				3
#define BATTERYVOLTAGE	4
#define BATTERYCURRENT	5

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
