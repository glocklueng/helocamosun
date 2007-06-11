#include "variables.h"

#define DIO_SEND	LATBbits.LATB7
#define DIO_GET		PORTBbits.RB7
#define CLK			LATBbits.LATB6
#define DIO_DIR		TRISBbits.TRISB7
#define CLK_DIR		TRISBbits.TRISB6

#define TAA_X_AXIS	0x03		// X-Axis
#define TAA_Y_AXIS	0x13		// Z-Axis
#define TAA_Z_AXIS	0x0B		// Y-Axis
#define COMPASS_RS	0x00		// Compass Reset command
#define COMPASS_ST	0x01		// Compass start command
#define COMPASS_RDY	0x03		// Compass Ready

#define pi			3.14159

#define TAA			LATDbits.LATD6		// LATBbit.LATB5
#define COMPASS		LATBbits.LATB2	
#define TAA_DIR		TRISDbits.TRISD6	// Tri-Axis Accellerometer
#define COMPASS_DIR	TRISBbits.TRISB2	// Compass module

extern	WORD X_axis,
			 Y_axis,
			 Z_axis,
			 Compass_X, 
			 Compass_Y;
		 	 
extern	signed int CompassXAverage, 
			CompassYAverage,
			CompassAverageAngle,
			XAxisAverage,
			YAxisAverage,
			ZAxisAverage;
			
extern unsigned int CompassAngle;
void ResetCompass(void);
void GetAxisAverage(void);
void GetAxisValues(void);
void GetCompassValues(void);
void GetCompassAverage(void);
void GetCompassAngle(void);
/*
Function: ShiftIO
Input Variables:	
	WORD byte_out - up to 2 bytes can be sent out LSB first
	char out_bits - # of bits to be sent out, MAX 15 bits
	WORD *byte_in - dereferenced 2 bytes returned to user
	char in_bits  - # of bits to be received, MAX 15 bits
	
Return Variable:
	char - returns 0 always
*/
char ShiftIO(WORD byte_out, char out_bits, WORD *byte_in, char in_bits);
