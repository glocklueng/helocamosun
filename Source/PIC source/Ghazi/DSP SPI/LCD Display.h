/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        LCD Display.h
* Dependencies:    none
* Processor:       dsPIC30F
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
* Richard Fischer   07/18/05  dsPICDEM 1.1 board LCD function prototypes
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/



//--------------------------------------------------------------------
// LCD function prototypes
void Display_Setup( void);              // initialize display		
void Display_Byte ( unsigned char );    // write data to lcd
void Display_String( unsigned char * ); // write string to LCD
void Display_ClrCol( unsigned char );
void Display_Pixel( unsigned char,unsigned char );


//-------------------------------------------------------------------
// LCD command macros
#define LCD_HomeClear()       {Display_Byte(0x82);Display_Byte(0x82);}
#define LCD_Home()            {Display_Byte(0x81);}
#define LCD_CharPos(col,row)  {Display_Byte(0xc5);Display_Byte(col);Display_Byte(row);}
#define LCD_WriteNext(data)   {Display_Byte(0xa8);Display_Byte(data);}
#define LCD_ClearEOL()        {Display_Byte(0x8a);}
#define LCD_CursorOn()        {Display_Byte(0x8c);}
#define LCD_CursorOff()       {Display_Byte(0x8b);}


