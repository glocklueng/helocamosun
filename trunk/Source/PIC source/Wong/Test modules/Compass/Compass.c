/*
	Title: 	Compass.c

	Author:	Kyle Wong

	Date:	13/04/07
	
	Description:	This program will communicate with a compass module(HMB55) using SPI
					communications.


	Compiled with MCC18 compiler (v8.01PL3)
	and MPLAB ver 7.5
*/
/************************** Include Section **************************/
//#include <p18f4431.h>
#include <p18f4620.h>
#include <delays.h>
#include <xlcd.h>
#include <spi.h>
#include <usart.h>
#include <capture.h>
#include "Compass.h"

/*************************** Program Control *************************/
#define LCD_DISPLAY 	1
#define LCD_ON 		0
/*************************** Defines *********************************/
#define DIO_SEND	LATBbits.LATB7
#define DIO_GET		PORTBbits.RB7
#define CLK			LATBbits.LATB6
#define DIO_DIR		TRISBbits.TRISB7
#define CLK_DIR		TRISBbits.TRISB6

#define TAA_X_AXIS	0x03		// X-Axis
#define TAA_Y_AXIS	0x13		// Y-Axis
#define TAA_Z_AXIS	0x0B		// Z-Axis
#define COMPASS_RS	0x00		// Compass Reset command
#define COMPASS_ST	0x01		// Compass start command
#define COMPASS_RDY	0x03		// Compass Ready

#define TAA			LATBbits.LATB5
#define COMPASS		LATBbits.LATB2
#define TAA_DIR		TRISBbits.TRISB5	// Tri-Axis Accellerometer
#define COMPASS_DIR	TRISBbits.TRISB2	// Compass module

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
	WORD Compass_X,Compass_Y,byte_out, byte_in, X_axis, Y_axis, Z_axis;
	char X_axis_array[4] = {0};
	int X_axis_average = 0;
	char Y_axis_array[4] = {0};
	int Y_axis_average = 0;
	char array_count = 0;
	char array_position = 0;
	char out_bits, in_bits;
	int value;

#if LCD_ON
	char string1[21] = {"<--  Compass  --->"};	// Create an empty lineset
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
	

/******** Aquiring Compass measurements *********/
		COMPASS_DIR = 0;
		
		COMPASS = 1;
		out_bits = 4;
		in_bits = 0;
		byte_out.byte[0] = COMPASS_RS;
		COMPASS = 0;
		ShiftIO(byte_out, out_bits, &Compass_X, in_bits);
		
		COMPASS = 1;
		out_bits = 4;
		in_bits = 0;
		byte_out.byte[0] = COMPASS_ST;
		COMPASS = 0;
		ShiftIO(byte_out, out_bits, &Compass_X, in_bits);
		
		COMPASS = 1;
		Delay10KTCYx(15);

		COMPASS = 0;
		do
		{	
			out_bits = 4;
			in_bits = 4;
			byte_out.byte[0] = COMPASS_RDY;
			ShiftIO(byte_out, out_bits, &Compass_X, in_bits);
		}
		while(byte_out.byte[0] != COMPASS_RDY);
/*		
		while(BusyUSART());
		TXREG = Compass_X.byte[0];
		while(BusyUSART());
		TXREG = Compass_X.byte[1];
*/		
		out_bits = 0;
		in_bits = 12;
		ShiftIO(byte_out, out_bits, &Compass_X, in_bits);
		ShiftIO(byte_out, out_bits, &Compass_Y, in_bits);

		COMPASS = 1;
#if 1
		while(BusyUSART());
		TXREG = 's';	
		while(BusyUSART());
		TXREG = Compass_X.byte[1];	
		while(BusyUSART());
		TXREG = Compass_X.byte[0];
#endif
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
	if(out_bits > 15)
	{
		out_bits = 15;
	}
		
	if(out_bits)	// takes into account the value 0
	{
		do
		{
			CLK = 0;

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
	}
	
	DIO_DIR = 1;	// SET TO INPUT FOR RECEIVING DATA
	
	if(in_bits > 15)
	{
		in_bits = 15;
	}
	
	if(in_bits)
	{
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
			Nop();
			CLK = 1;
		}
		while(in_bits);
	}
	return 0;
}


