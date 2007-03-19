/*
	Title: 	Accelerometer.c

	Author:	Kyle Wong

	Date:	26/09/05
	
	Description:	This program will communicate with a tri-axis accelerometer using SPI
					communications.

	Notes:	done - May need to add delays in ShiftIO() function to slow down the SPI
			- Use an array for a lookup table
			- PWM may not be useful in this application since the smallest resolution is 
			  2.44kHz and we need 20ms ~ 50Hz
			- Try using compare interrupts to generate accurate pulse widths

	To Do List:
			- Create a 5ms resolution RTC(25000 instructions)
			- Need to incorporate I2C communications
			- Need to find a PIC with enough timers for at least 5 servos, consider looking at
			  16-bit PICS

	Compiled with MCC18 compiler (v8.01PL3)
	and MPLAB ver 7.5
*/
/************************** Include Section **************************/
#include <p18f4431.h>
//#include <p18f4620.h>
#include <delays.h>
#include <xlcd.h>
#include <spi.h>
#include <usart.h>
#include <capture.h>
#include "Accelerometer.h"

/*************************** Program Control *************************/
#define LCD_DISPLAY 	1
#define LCD_ON 		0
/*************************** Defines *********************************/
#define TAA_X_AXIS	0x03		// X-Axis
#define TAA_Y_AXIS	0x0B		// Z-Axis
#define TAA_Z_AXIS	0x13		// Y-Axis
#define DIO_SEND	LATBbits.LATB7
#define DIO_GET		PORTBbits.RB7
#define CLK			LATBbits.LATB6
#define TAA			LATBbits.LATB5
#define DIO_DIR		TRISBbits.TRISB7
#define CLK_DIR		TRISBbits.TRISB6
#define TAA_DIR		TRISBbits.TRISB5	// Tri-Axis Accellerometer
#define	DEBUG_DELAY	200

/*************************** Definitions *****************************/
typedef union
{
	char byte[2];
	int D_byte;
	//long D_word;
}WORD;

/*************************** Global Variables ************************/
WORD Capture;

/*************************** Initialization Functions ****************/
void OpenLCD(void);

void SerialInit(void);

char ShiftIO(WORD byte_out, char out_bits, WORD *byte_in, char in_bits);

/*************************** Interrupt Section ***********************/
/********************** Interrupt Function Definition ****************/
void CCPINT(void);

/************************* Interrupt Functions ***********************/
#pragma interrupt INTERRUPT
void INTERRUPT(void)
{
	if(PIR1bits.CCP1IF)			// capture compare interrupt
	{
		CCPINT();
	}
}

void CCPINT(void)
{
	PIR1bits.CCP1IF = 0; 		// clear the capture flag
	Capture.byte[0] = TMR1L;
	TMR1L = 0;					// Clear the low timer
	Capture.byte[1] = TMR1H;
	TMR1H = 0;					// Clear the high timer
}

#pragma code high_int_vector = 0x08					/* change to interrupt page*/
void interrupt_at_high_vector(void)
{
	_asm GOTO INTERRUPT _endasm
}

#pragma code 										/* return to default code section */

void main(void)
{
/*********** Varible declaration *****************/
	unsigned char command;
	unsigned char data[4];
	int angle;
	WORD byte_out, byte_in, X_axis, Y_axis, Z_axis;
	char out_bits, in_bits;
	int value;

#if LCD_ON
	char string1[21] = {"<--Accelerometer--->"};	// Create an empty lineset
	char string2[21] = {"Axis x value:   "};
	char string3[21] = {"Axis y value:   "};
	char string4[21] = {"Axis z value:   "};
#endif

/********** Beginning of main code **************/
	TRISB = 0;
	TAA_DIR = 0;
	TAA = 1;

#if LCD_ON
	OpenLCD();
	
	while(BusyXLCD());		// everything is OK
	WriteCmdXLCD(LCD_LINE1);// write over whatever is on line 1 of the LCD

	while(BusyXLCD());
	putsXLCD(string1);
	
	while(BusyXLCD());		// everything is OK
	WriteCmdXLCD(LCD_LINE2);// write over whatever is on line 1 of the LCD
	
	while(BusyXLCD());
	putsXLCD(string2);
	
	while(BusyXLCD());		// everything is OK
	WriteCmdXLCD(LCD_LINE3);// write over whatever is on line 1 of the LCD
	
	while(BusyXLCD());
	putsXLCD(string3);
	
	while(BusyXLCD());		// everything is OK
	WriteCmdXLCD(LCD_LINE4);// write over whatever is on line 1 of the LCD
	
	while(BusyXLCD());
	putsXLCD(string4);
#endif
/********* TROUBLESHOOTING TOOL *********
		while(BusyUSART());
		TXREG = string2[17];
*/

/******** VARIABLE INITIALIZATION ************/
	byte_out.byte[0] = 0x18;
	byte_in.D_byte = 0;
	X_axis.D_byte = 0;
	Y_axis.D_byte = 0;
	Z_axis.D_byte = 0;
	out_bits = 5;
	in_bits = 15;
	SerialInit();
	value = 0;
/******** START OF MAIN LOOP *****************/
	do
	{
/******** INITILIZE LOOP VARIABLES ***********/
		byte_in.D_byte = 0;
		X_axis.D_byte = 0;
		Y_axis.D_byte = 0;
		Z_axis.D_byte = 0;

/******** Aquiring Tilt measurements *********/
		byte_out.byte[0] = TAA_X_AXIS;
		TAA = 0;
		ShiftIO(byte_out, out_bits, &X_axis, in_bits);
		TAA = 1;

		byte_out.byte[0] = TAA_Y_AXIS;
		TAA = 0;
		ShiftIO(byte_out, out_bits, &Y_axis, in_bits);
		TAA = 1;
		
		byte_out.byte[0] = TAA_Z_AXIS;
		TAA = 0;
		ShiftIO(byte_out, out_bits, &Z_axis, in_bits);
		TAA = 1;
		
//
//		Delay10KTCYx(200);
	}
	while(1);

}
/*****************************  End of Main code *********************/

/*************************** Function Definitions ********************/
/*
Function:	ShiftIO
Description: This function will serially communicate to a Tri-axis accelerometer and magnetic 
			 compass module using a form of SPI.
			 The byte_out variable is of a union type and can contain up to 2 bytes
			 The byte_in variable is also of a union type can can return up to 2 bytes
			 out_bits and in_bits contain the number of bits in each WORD variable that is 
			 going to be sent out.
			 Bytes are sent out LSB first

*/
char ShiftIO(WORD byte_out, char out_bits, WORD *byte_in, char in_bits)
{
	CLK_DIR = 0;	// Set the direction of the clock line to be an output
	DIO_DIR = 0;	// SET TO OUTPUT
	TAA_DIR = 0;
	
	do
	{
		CLK = 0;					// clock line goes low
		if(byte_out.byte[0] & 0x01)	// mask off the LSB and transmit
		{
			DIO_SEND = 1;
		}
		else
		{
			DIO_SEND = 0;
		}
		Nop();
		CLK = 1;					// data is sent on a positive going pulse
		byte_out.D_byte = byte_out.D_byte >> 1;
		out_bits--;					// ensure that we are sending the correct number of bits
	}
	while(out_bits);

	DIO_DIR = 1;	// SET TO INPUT FOR RECEIVING DATA

	do
	{
		CLK = 0;
		if(DIO_GET)// WHAT VALUE ARE WE RECEIVING
		{
			byte_in->D_byte |= 0x0001;
		}
		else
		{
			byte_in->D_byte &= 0xFFFE;
		}
		in_bits--;
		if(in_bits)
		{
			byte_in->D_byte = byte_in->D_byte << 1;	// receive data and shift to the right
		}
		CLK = 1;
		Nop();
	}
	while(in_bits);
	return 0;
}


