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

// timers.c
#define MAIN		0x02
#define DEBUG		0x03

// SPIComms.c
extern char Accelerator[6];
extern char Gyro[4];
extern char Compass[2];
extern char Voltages[4];
extern char Accoustic[2];
extern char Temperature[2];
extern char RPM[2];
extern char command[3];
extern char servos[6];
