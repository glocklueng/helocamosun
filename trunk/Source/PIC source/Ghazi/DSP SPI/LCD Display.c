
/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        LCD Display.c
* Dependencies:    p30fxxxx.h
* Processor:       dsPIC30F6014
* Compiler:        MPLAB® C30 v1.32.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Richard Fischer   07/18/05  dsPICDEM 1.1 board LCD routines
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/

#include <p30fxxxx.h>
#include "LCD Display.h"


/*---------------------------------------------------------------------
  Function Name: Display_Setup
  Description:   Initialize the SPI port to communicate with the LCD display
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void Display_Setup( void )
{
	unsigned int dummy;
	
	LATGbits.LATG9 = 1;                 // Set SPI slave select pin high
	TRISGbits.TRISG9 = 0;               // and set as output
	LATGbits.LATG9 = 0;                 // Set SPI slave select pin high
	SPI2CON = 0x003c;                   // Master mode, SCK = Fcy/64 = 230kHz, Input data sampled at 
	                                    // middle of output time, Data clocked out on rising edge, Clk idle is low

	SPI2STAT = 0x8000;                  // Enable SPI port
}


 
/*---------------------------------------------------------------------
  Function Name: Display_Byte
  Description:   Send a character to the LCD
  Inputs:        Byte to Display on dsPICDEM 1.1 Board LCD
  Returns:       None
-----------------------------------------------------------------------*/
void Display_Byte( unsigned char ByteToDisplay )
{
	unsigned int dummy;
	
	LATGbits.LATG9 = 1;                 // Set slave select high
	SPI2STATbits.SPIROV = 0;            // Clear overflow flag
	dummy = SPI2BUF;                    // Read buffer to avoid overflow
	LATGbits.LATG9 = 0;                 // Set slave select low for new transmission
    SPI2BUF = ByteToDisplay;            // Write the data to the output buffer
    while (SPI2STATbits.SPIRBF == 0);   // Wait for transmission to complete
	LATGbits.LATG9 = 1;                 // Set slave select high
}


/*---------------------------------------------------------------------
  Function Name: Display_ClrCol
  Description:   Clear a column x of the display
  Inputs:        ..
  Returns:       None
-----------------------------------------------------------------------*/
void Display_ClrCol( unsigned char x )
{
		unsigned char counter;
		counter = 32;			   // Load counter to clear pixel 32 times
		while(counter > 0){
   		   Display_Byte(0xd9);	    // Load command to clear a pixel, Call routine to send the command to the display
	   	   Display_Byte(x);	 	    // Load X location of pixel, Call routine to send the command to the display
		   Display_Byte(counter--); // Load Y location of pixel, Call routine to send the command to the display
		}
}


/*---------------------------------------------------------------------
  Function Name: Display_Pixel
  Description:   Display a pixel at x,y of the display
  Inputs:        ..
  Returns:       None
-----------------------------------------------------------------------*/
void Display_Pixel( unsigned char x,unsigned char y )
{
		Display_Byte(0xd8);		// Load command to set a pixel, Call routine to send the command to the display
		Display_Byte(x);		// Load X location of pixel, Call routine to send the command to the display
		Display_Byte(y);		// Load Y location of pixel, Call routine to send the command to the display
}


/*---------------------------------------------------------------------
  Function Name: Display_String
  Description:   Send a string of characters to the LCD
  Inputs:        ..
  Returns:       None
-----------------------------------------------------------------------*/
void Display_String( unsigned char *Data )
{
   while(*Data)
   {
      Display_Byte(0xa8);
      Display_Byte(*Data++);
   }	
}

