#include <p30fxxxx.h>
#include <string.h>
#include "SPI.h"
#include "RF.h"
#define MAXPACKLEN 	32		// maximum packet length

unsigned char GSPI_AccData[6] = "";
unsigned char GSPI_2GyroData[4] = "";
unsigned char GSPI_YawGyroData[2] = "";
unsigned char GSPI_CompData[2] = "";
unsigned char GSPI_AcousticData[2] = "";
unsigned char GSPI_VoltData[4] = "";
unsigned char GSPI_TempData[2] = "";
unsigned char GSPI_RpmData[2] = "";
unsigned char GSPI_StatusData[2] = "";
unsigned char GSPI_TimeData[6] = "";
unsigned char GSPI_LatData[9] = "";
unsigned char GSPI_LongData[10] = "";
unsigned char GSPI_AltData[7] = "";
unsigned char GSPI_SatData[2] = "";

void SPI_init ( char mode16, char falling )
{
	SPI1STATbits.SPIROV = 0;		// clear the receive overflow flag bit	
	SPI1CONbits.MODE16 = mode16;	// if "mode16" is true, then the SPI module is put into 16-bit mode	
	SPI1CONbits.SMP = 0;			// input is sampled at middle of data output time
	SPI1CONbits.CKP = 1;			// Idle state for clock is a high level; active state is low
	SPI1CONbits.CKE = falling;		// if "falling" is true, then data is checked on falling edges of SCLK
	SPI1CONbits.MSTEN = 1;			// Master mode
	SPI1CONbits.SSEN = 0;			// Not used in Master mode, so I cleared it
	
	// Prescaler Config = 1:1 (fastest possible)
//	SPI1CONbits.SPRE = 7;		// secondary prescaler 1:1
//	SPI1CONbits.PPRE = 3;		// primary prescaler 1:1

	// Prescaler Config = 512:1 (slowest possible)
//	SPI1CONbits.SPRE = 0;		// secondary prescaler 8:1
//	SPI1CONbits.PPRE = 0;		// primary prescaler 64:1

	SPI1CONbits.SPRE = 0;		// secondary prescaler 8:1
	SPI1CONbits.PPRE = 2;		// primary prescaler 64:1

}

void SPI_tx_byte ( char ch)
{
	char c;
	SPI1STATbits.SPIEN = 1;
	SPI1BUF = ch;
	c = SPI1BUF;
}

void SPI_tx_word ( unsigned short ch)
{
	unsigned short c;
	SPI1STATbits.SPIEN = 1;
	SPI1BUF = ch;
	c = SPI1BUF;
}

void SPI_tx_command ( unsigned char packet[], char len )
{
	char cnt = 0;
	short i;
	
	for (cnt = 0; cnt < len; cnt++)
	{
		while (SPI1STATbits.SPITBF);
		SPI_tx_byte( packet[cnt] );	
		for (i = 0; i < 500; i++);		// delays to allow 4431 to handle SPI
	} 
	
	
}

char SPI_tx_req ( unsigned char packet[], unsigned char data[] )
{
	char cnt = 0;
	int i;
	char rcv[7] = "";
	unsigned char check = 0;
	char bite = 0;
	
	uCSS = 0;
	for (cnt = 0; cnt < strlen((char*)packet); cnt++) // WATCH THIS
	{
		
		
		while (SPI1STATbits.SPITBF);
		SPI_tx_byte( packet[cnt] );	
		
		while (!SPI1STATbits.SPIRBF);

		
		// We need to ignore the first byte we receive since it is garbage
		if (cnt > 0)
		{
			bite = SPI1BUF;
			//if (check == 0xA5) 		// This is to check that we have good comms on SPI with the 4431
			//{ 
				data[cnt - 1] = bite; 
			//}
		}
		else
		{
			check = SPI1BUF;		// if it's the first byte, store it for possible error checking
		}
		
		for (i = 0; i < 500; i++);	// delays to allow 4431 to handle SPI
	} 
	uCSS = 1;
	return 1;
}

int SPI_readTemp1 ( void )
{
	short dummy = 0x0000;
	
	short delaycount = 0;
	
	SPI1STATbits.SPIEN = 0;
	SPI_init( 1, 1 );	
	SPI1STATbits.SPIEN = 1;
	
	
	for(delaycount = 0; delaycount < 1000; delaycount++);
	tempSS = 1;
	SPI1BUF = dummy;
	while (!SPI1STATbits.SPIRBF);
	dummy = SPI1BUF;
	tempSS = 0;
	for(delaycount = 0; delaycount < 1000; delaycount++);
		
	SPI1STATbits.SPIEN = 0;
	SPI_init( 0, 0 );
	SPI1STATbits.SPIEN = 1;

	return dummy;
}

//int SPI_readYawGyro ( void )
//{
//	short dummy = 0x8710;
//	short delaycount = 0;
//	
//	SPI1STATbits.SPIEN = 0;
//	SPI_init( 1, 0 );	
//	SPI1STATbits.SPIEN = 1;
//	
//	
//	
//	for(delaycount = 0; delaycount < 1000; delaycount++);
//	YawGyroSS = 0;
//	SPI1BUF = dummy;
//	while (!SPI1STATbits.SPIRBF);
//	dummy = SPI1BUF;
//	YawGyroSS = 1;
//	for(delaycount = 0; delaycount < 1000; delaycount++);
//
//	SPI1STATbits.SPIEN = 0;
//	SPI_init( 0, 0 );
//	SPI1STATbits.SPIEN = 1;
//
//	
//	
//	return dummy;
//}
