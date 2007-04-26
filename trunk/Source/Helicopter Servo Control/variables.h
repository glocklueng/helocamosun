/*
variables.h
Global variables
*/
typedef unsigned short int WORD;

typedef union _WORD
{
	char byte[2];
	int D_byte;
	//long D_word;
}WORD;

// main.c
#define AVERAGEVALUE	4
		 
// SPIComms.c
extern char Accelerator[6];
extern char Gyro[4];
extern char Compass[4];
extern char Voltages[4];
extern char Accoustic[2];
extern char Temperature[2];
extern char RPM[2];
extern char command[2];
extern char servos[6];
