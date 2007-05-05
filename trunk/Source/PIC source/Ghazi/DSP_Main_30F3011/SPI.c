#include <p30fxxxx.h>
#include <string.h>
#include "SPI.h"

#define MAXPACKLEN 	32		// maximum packet length

char GSPI_AccData[6] = "";
char GSPI_2GyroData[4] = "";
char GSPI_YawGyroData[2] = "";
char GSPI_CompData[2] = "";
char GSPI_AcousticData[2] = "";
char GSPI_VoltData[4] = "";
char GSPI_TempData[2] = "";
char GSPI_RpmData[2] = "";
char GSPI_StatusData[2] = "";

void SPI_init ()
{
	SPI1STATbits.SPIROV = 0;	// clear the receive overflow flag bit	
	SPI1CONbits.MODE16 = 0;		// 8-bit mode	
	SPI1CONbits.SMP = 0;		// input is sampled at middle of data output time
	SPI1CONbits.CKP = 1;		// Idle state for clock is a high level; active state is low
	SPI1CONbits.CKE = 0;		// Serial output data changes on transition from Idle clock state to active clock state
	SPI1CONbits.MSTEN = 1;		// Master mode
	SPI1CONbits.SSEN = 0;		// Not used in Master mode, so I cleared it
	
	// Current prescaler configuration: Fuckin Fast Yo!
	SPI1CONbits.SPRE = 7;		// secondary prescaler 1:1
	SPI1CONbits.PPRE = 3;		// primary prescaler 1:1
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
	char c;
	SPI1STATbits.SPIEN = 1;
	SPI1BUF = ch;
	c = SPI1BUF;
}

void SPI_tx_command ( unsigned char packet[MAXPACKLEN], char len )
{
	char cnt = 0;
	
	for (cnt = 0; cnt < len; cnt++)
	{
		while (SPI1STATbits.SPITBF);
		SPI_tx_byte( packet[cnt] );	
	} 
}

void SPI_tx_req ( char packet[MAXPACKLEN], char data[MAXPACKLEN] )
{
	char cnt = 0;
	char dummy = strlen(packet);
	char dummy2 = strlen(data);
	int i;
	char rcv[7] = "";
	unsigned char check = 0;
	char bite = 0;
	
	for (cnt = 0; cnt < strlen(packet); cnt++)
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
		
		for (i = 0; i < 500; i++);
	} 
}

void SPI_readYawGyro ( void )
{
	int i;
	
	SPI1CONbits.MODE16 = 1;					// switch SPI to 16 bit mode
	
	LATBbits.LATB0 = 0;						// pull framing pin low
	SPI_tx_word( 0b1000001100000000 );		// Send command to read gyro
	for (i = 0; i < 1700; i++);				// delay some (time for 16 clocks)
	
	LATBbits.LATB0 = 1;						// set framing pin high		
	
	for (i = 0; i < 1000; i++);				// wait some
	
	
	LATBbits.LATB0 = 0;						// pull framing pin low
	SPI_tx_word( 1 );						// tx 2 byte so we can read 2
	for (i = 0; i < 1700; i++);				// go low for the duration of 2 bytes
	LATBbits.LATB0 = 1;						// set framing pin high	
	
	SPI1CONbits.MODE16 = 0;					// switch SPI back to 8-bit mode
	
	
	for (i = 0; i < 10000; i++);			// wait some
	
}
