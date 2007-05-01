#include <p18f4431.h>
#include <math.h>
#include "variables.h"
#include "Serial_IO.h"
#include "USART.h"
	WORD X_axis;
	WORD Y_axis;
	WORD Z_axis;
	WORD Compass_X, 
		 Compass_Y;	
signed int CompassXAverage, 
		CompassYAverage,
		CompassAngle,
		XAxisAverage,
		YAxisAverage,
		ZAxisAverage;
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
			CLK = 1;

			if(byte_out.byte[0] & 0x01)	// mask off the LSB and transmit
			{
				DIO_SEND = 1;
			}
			else
			{
				DIO_SEND = 0;
			}
			Nop();
			CLK = 0;					// data is sent on a positive going pulse
			byte_out.D_byte = byte_out.D_byte >> 1;
			out_bits--;					// ensure that we are sending the correct number of bits

		}
		while(out_bits);
	}
	
	DIO_DIR = 1;	// SET TO INPUT FOR RECEIVING DATA
	CLK = 1;
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
	CLK = 0;
	return 0;
}

/*
Function: ResetCompass
Description: Sends a reset command followed by a start conversion command to the 
			 compass module.
*/
void ResetCompass(void)
{
	char out_bits = 4;
	char in_bits = 0;
	WORD Compass,
		 byte_out;
	
	COMPASS_DIR = 0;
	
	byte_out.byte[0] = COMPASS_RS;
	COMPASS = 0;
	ShiftIO(byte_out, out_bits, &Compass, in_bits);
	COMPASS = 1;
	
	byte_out.byte[0] = COMPASS_ST;
	COMPASS = 0;
	ShiftIO(byte_out, out_bits, &Compass, in_bits);
	COMPASS = 1;	
}

/*
Function: GetCompassValues
Description: Checks the status of the compass module and clocks in the 11-bit X and Y values
		     from the compass module.
*/
//void GetCompassValues(WORD Compass_X, WORD Compass_Y)
void GetCompassValues(void)
{
	char out_bits, 
		 in_bits;
	WORD byte_out = 0;

	COMPASS = 0;
	
	Compass_X.D_byte = 0;
	Compass_Y.D_byte = 0;

	out_bits = 4;
	in_bits = 4;
	byte_out.byte[0] = COMPASS_RDY;
	ShiftIO(byte_out, out_bits, &Compass_X, in_bits);

	out_bits = 0;
	in_bits = 11;
	
	Compass_X.D_byte = 0;
	ShiftIO(byte_out, out_bits, &Compass_X, in_bits);
	Compass_Y.D_byte = 0;
	ShiftIO(byte_out, out_bits, &Compass_Y, in_bits);
	
	COMPASS = 1;
}

/*
Function: GetCompassAverage
Description: Calculates a moving average of the X and Y values received by the compass module
*/
//void GetCompassAverage(WORD Compass_X, WORD Compass_Y, int *CompassXAverage, int *CompassYAverage)
void GetCompassAverage(void)
{
	char loopcounter;
	long CompassXtemp,
		CompassYtemp;
	static int CompassXData[AVERAGEVALUE],
			   CompassYData[AVERAGEVALUE];
	static char CompassCount = 0;
	CompassXAverage = 0;
	CompassYAverage = 0;
	CompassXtemp = Compass_X.D_byte;
	CompassYtemp = Compass_Y.D_byte;

    if (( CompassXtemp >= 1024) && (CompassYtemp >= 1024))
    {
          CompassXtemp = -1 * (2048.0 - CompassXtemp);
          CompassYtemp = -1 * (2048.0 - CompassYtemp);
    } else
    
    if (( CompassXtemp >= 1024) && (CompassYtemp < 1024))
    {
          CompassYtemp = -1 * (2048.0 - CompassXtemp);

    } else
 
    if (( CompassXtemp < 1024) && (CompassYtemp >= 1024))
    {
          CompassYtemp = -1 * (2048.0 - CompassYtemp);

    } 
    CompassXData[CompassCount] = CompassXtemp;
    CompassYData[CompassCount] = CompassYtemp;
    
    for(loopcounter = 0; loopcounter < AVERAGEVALUE; loopcounter++)
    {
	    CompassXAverage += CompassXData[loopcounter];
	    CompassYAverage += CompassYData[loopcounter];
	}
	
	CompassXAverage = CompassXAverage /(int) loopcounter;
	CompassYAverage = CompassYAverage /(int) loopcounter;
	
	if(CompassCount > AVERAGEVALUE)
	{
		CompassCount = 0;
	}
}

/*
Function: GetAxisValues
Description: Gets the X, Y, and Z axis values from the tri-axis accelerometer module
*/
void GetAxisValues(void)
{
	char out_bits = 5;
	char in_bits = 15;
	WORD byte_out;
	
	TAA_DIR = 0;
	
	byte_out.byte[0] = TAA_X_AXIS;	// Get the X axis values
	TAA = 0;
	ShiftIO(byte_out, out_bits, &X_axis, in_bits);
	TAA = 1;
	
	byte_out.byte[0] = TAA_Y_AXIS;	// Get the Y axis values
	TAA = 0;
	ShiftIO(byte_out, out_bits, &Y_axis, in_bits);
	TAA = 1;

	
	
	byte_out.byte[0] = TAA_Z_AXIS;	// Get the Z axis values
	TAA = 0;
	ShiftIO(byte_out, out_bits, &Z_axis, in_bits);
	TAA = 1;
}
/*
Function: GetAxisAverage
Description: Calculates the moving average of the X, Y, and Z axis values
*/
void GetAxisAverage(void)
//void GetAxisAverage(WORD *X_axis, WORD *Y_axis, WORD *Z_axis, int *XAxisAverage, int *YAxisAverage, int *ZAxisAverage)
{
	char ArrayCount;
	static int AxisCount = 0;
	static int XAxisData[AVERAGEVALUE],
			   YAxisData[AVERAGEVALUE],
			   ZAxisData[AVERAGEVALUE];

	X_axis.D_byte &= 0x0FFF;
	Y_axis.D_byte &= 0x0FFF;
	Z_axis.D_byte &= 0x0FFF;
	
	X_axis.D_byte = X_axis.D_byte >> 3;
	Y_axis.D_byte = Y_axis.D_byte >> 3;
	Z_axis.D_byte = Z_axis.D_byte >> 3;
	
//	WriteUSART('Y');
//	while(BusyUSART());
//	WriteUSART(X_axis.byte[1]);
//	while(BusyUSART());
//	WriteUSART(X_axis.byte[0]);
//	while(BusyUSART());
/*		
	// Take the average of the new value with the previous values
	for(ArrayCount = 0; ArrayCount < AVERAGEVALUE; ArrayCount++)
	{
		if(X_axis.byte[1])
		{
			XAxisAverage += (XAxisData[AxisCount] + AXISOFFSET);
		} 
		else
		{
			XAxisAverage -= ((0xFF - XAxisData[ArrayCount]) + AXISOFFSET);
		}
		
		if(Y_axis.byte[1])
		{
			YAxisAverage += (YAxisData[AxisCount] + AXISOFFSET);
		} 
		else
		{
			YAxisAverage -= ((0xFF - YAxisData[ArrayCount]) + AXISOFFSET);
		}
		
		if(Z_axis.byte[1])
		{
			ZAxisAverage += (ZAxisData[AxisCount] + AXISOFFSET);
		} 
		else
		{
			ZAxisAverage -= ((0xFF - ZAxisData[ArrayCount]) + AXISOFFSET);
		}
	}
	
	XAxisAverage = XAxisAverage / AVERAGEVALUE;
	YAxisAverage = YAxisAverage / AVERAGEVALUE;
	ZAxisAverage = ZAxisAverage / AVERAGEVALUE;
*/	
	if(X_axis.byte[0] & 0x80)
	{
		X_axis.byte[0] = (0xFF - X_axis.byte[0] + 1);
		Accelerator[0] = X_axis.byte[1];
	}
	else
	{
		Accelerator[0] = 0x00;
	}
	
	if(Y_axis.byte[0] & 0x80)
	{
		Y_axis.byte[0] = (0xFF - Y_axis.byte[0] + 1);
		Accelerator[2] = Y_axis.byte[1];
	}
	else
	{
		Accelerator[2] = 0x00;
	}

	if(Z_axis.byte[0] & 0x80)
	{
		Z_axis.byte[0] = (0xFF - Z_axis.byte[0] + 1);
		Accelerator[4] = Z_axis.byte[1];
	}
	else
	{
		Accelerator[4] = 0x00;
	}
		
	Accelerator[1] = X_axis.byte[0];
	Accelerator[3] = Y_axis.byte[0];
	Accelerator[5] = Z_axis.byte[0];
	
//	WriteUSART('X');
//	while(BusyUSART());
//	WriteUSART(Accelerator[0]);
//	while(BusyUSART());
//	WriteUSART(Accelerator[1]);
//	while(BusyUSART());
	
	AxisCount++;	// stores the newest value over the oldest value
	if(AxisCount > AVERAGEVALUE)
	{
		AxisCount = 0;
	}
}
