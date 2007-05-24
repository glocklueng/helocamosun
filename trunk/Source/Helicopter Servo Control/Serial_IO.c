#include <p18f4431.h>
#include <math.h>
#include "variables.h"
#include "Serial_IO.h"

WORD X_axis;
WORD Y_axis;
WORD Z_axis;
WORD Compass_X, 
 	 Compass_Y;
 	 
signed int CompassXAverage, 
		CompassYAverage,
		CompassAverageAngle,
		XAxisAverage,
		YAxisAverage,
		ZAxisAverage;
signed float fCompassAngle;
unsigned int CompassAngle;
char Accelerator[6];
char Compass[2];

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
void GetCompassValues(void)
{
	char out_bits, 
		 in_bits;
	WORD byte_out = 0;

	COMPASS = 0;	// Enable the compass module
	
	Compass_X.D_byte = 0;	// Set the return values to 0
	Compass_Y.D_byte = 0;

	out_bits = 4;	// Set the # of bits to be read from the compass
	in_bits = 4;
					// Ask if the compass is ready to transmit data back
	byte_out.byte[0] = COMPASS_RDY;
	ShiftIO(byte_out, out_bits, &Compass_X, in_bits);

	out_bits = 0;	// Set the # of bits to be read from the compass
	in_bits = 11;
					// Get the 11-bit data value for the X-axis of the compass
	Compass_X.D_byte = 0;
	ShiftIO(byte_out, out_bits, &Compass_X, in_bits);
					// Get the 11-bit data value for the Y-axis of the compass
	Compass_Y.D_byte = 0;
	ShiftIO(byte_out, out_bits, &Compass_Y, in_bits);
	
	COMPASS = 1;
}

/*
Function: GetCompassAverage
Description: Calculates a moving average of the X and Y values received by the compass module
*/
void GetCompassAverage(void)
{
	char loopcounter;
	float xAxisComp, yAxisComp;
	static int CompassXtemp = 0,
			   CompassYtemp = 0;
	static char CompassCount = 0;
	static char AverageCount = 0;
	signed int Xtemp = Compass_X.D_byte,
		   	   Ytemp = Compass_Y.D_byte;
	
    if (( Xtemp >= 1024) && (Ytemp >= 1024))
    {
          Xtemp = -1 * (2048.0 - Xtemp);
          Ytemp = -1 * (2048.0 - Ytemp);
    } else
    
    if (( Xtemp >= 1024) && (Ytemp < 1024))
    {
          Xtemp = -1 * (2048.0 - Xtemp);
    } else
 
    if (( Xtemp < 1024) && (Ytemp >= 1024))
    {
          Ytemp = -1 * (2048.0 - Ytemp);
    } 
	
	if(CompassCount > AVERAGEVALUE-1)
	{
		CompassCount = 0;
		
		CompassYtemp /= AVERAGEVALUE;
		CompassXtemp /= AVERAGEVALUE;
		xAxisComp = XAxisAverage * pi / 180; // convert to radians
		yAxisComp = YAxisAverage * pi / 180;
		CompassYAverage = CompassYtemp * cos( xAxisComp);
		CompassXAverage = CompassXtemp * cos( yAxisComp);
		CompassXtemp = 0;
		CompassYtemp = 0;
		
		if (( CompassXAverage >= 0) && (CompassYAverage >= 0))
	    {
	          fCompassAngle = atan( -1.0 * (float)CompassYAverage /(float) CompassXAverage );
	          fCompassAngle *= (180.0 / pi);	// convert to degrees
	          fCompassAngle += 360.0;
	          CompassAngle = (unsigned int) fCompassAngle;
	    } else
	     
	    if (( CompassXAverage < 0) && (CompassYAverage < 0))
	    {
	          fCompassAngle = atan(-1.0*(float)CompassYAverage/(float)CompassXAverage);
	          fCompassAngle *= (180.0 / pi);
	          fCompassAngle += 180.0;
	          CompassAngle = (unsigned int) fCompassAngle;
	    } else
	    
	    if (( CompassXAverage < 0) && (CompassYAverage >= 0))
	    {
	          fCompassAngle = atan(-1.0*(float)CompassYAverage/(float)CompassXAverage);
	          fCompassAngle *= (180.0 / pi);
	          fCompassAngle += 180.0;
	          CompassAngle = (unsigned int) fCompassAngle;
	    } else
	 
	    if (( CompassXAverage >= 0) && (CompassYAverage < 0))
	    {
	          fCompassAngle = atan(-1.0*(float)CompassYAverage/(float)CompassXAverage);
	          fCompassAngle *= (180.0 / pi);
	          CompassAngle = (unsigned int) fCompassAngle;
	    }
	}
	else
	{
		CompassCount++;
		CompassXtemp += Xtemp;
		CompassYtemp += Ytemp;
	}

}

void GetCompassAngle(void)
{
	char loopcounter;
	static char AverageCount = 0;
	static signed int CompassAverage[AVERAGEVALUE];
	
	CompassAverage[AverageCount] = CompassAngle;
	
	AverageCount++;
	if( AverageCount > AVERAGEVALUE - 1 )
	{
	 AverageCount = 0;
	}
	
	for(loopcounter = 0; loopcounter < (AVERAGEVALUE - 1); loopcounter++)
	{
		CompassAverageAngle += CompassAverage[loopcounter];
	}
	
	CompassAverageAngle = CompassAverageAngle / AVERAGEVALUE;
	
	 	    Compass[0] = CompassAverageAngle>>8;
	      Compass[1] = (char) CompassAverageAngle;
	
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
{
	char ArrayCount;
	static int Xaxistemp = 0,
			   Yaxistemp = 0,
			   Zaxistemp = 0;
	static int AxisCount = 0;
	static int XAxisData[AVERAGEVALUE],
			   YAxisData[AVERAGEVALUE],
			   ZAxisData[AVERAGEVALUE];

	X_axis.D_byte = X_axis.D_byte >> 3;		// remove the lower 3 bits to 
	Y_axis.D_byte = Y_axis.D_byte >> 3;		// elminate high frequency components
	Z_axis.D_byte = Z_axis.D_byte >> 3;
	
	X_axis.D_byte &= 0x00FF;
	Y_axis.D_byte &= 0x00FF;
	Z_axis.D_byte &= 0x00FF;
	
	if(X_axis.D_byte >= 128)
	{
		X_axis.D_byte = -1*(256 - X_axis.D_byte);
	}
	if(Y_axis.D_byte >= 128)
	{
		Y_axis.D_byte = -1*(256 - Y_axis.D_byte);
	}
	if(Y_axis.D_byte >= 128)
	{
		Z_axis.D_byte = -1*(256 - Z_axis.D_byte);
	}

	if(AxisCount > AVERAGEVALUE - 1)
	{
		AxisCount = 0;
		XAxisAverage = Xaxistemp / AVERAGEVALUE;
		YAxisAverage = Yaxistemp / AVERAGEVALUE;
		ZAxisAverage = Zaxistemp / AVERAGEVALUE;
		Accelerator[0] = XAxisAverage>>8;
		Accelerator[1] = (char)XAxisAverage;
		Accelerator[2] = YAxisAverage>>8;
		Accelerator[3] = (char)YAxisAverage;
		Accelerator[4] = ZAxisAverage>>8;
		Accelerator[5] = (char)ZAxisAverage;
		Xaxistemp = 0;
		Yaxistemp = 0;
		Zaxistemp = 0;
	}
	else
	{	
		AxisCount++;	// stores the newest value over the oldest value
		Xaxistemp += X_axis.D_byte;
		Yaxistemp += Y_axis.D_byte;
		Zaxistemp += Z_axis.D_byte;
	}
}
