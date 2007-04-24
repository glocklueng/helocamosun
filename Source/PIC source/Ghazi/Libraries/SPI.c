#include "SPI.h"
#include <p30fxxxx.h>
#include <string.h>

#define MAXPACKLEN 	256		// maximum packet length

// Info Requests
char GSPI_packet[MAXPACKLEN] = ""; 
char GSPI_test[] = "Test Packet";

char GSPI_AccReq[] = { 'A', 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 };
char GSPI_AccData[6] = "";

char GSPI_2GyroReq[] = { 'G', 0x01, 0x01, 0x01, 0x01, 0x00 };
char GSPI_2GyroData[4] = "";
	
char GSPI_YawGyroData[2] = "";

char GSPI_CompReq[] = { 'C', 0x01, 0x01, 0x00 };
char GSPI_CompData[2] = "";

char GSPI_AcousticReq[] = { 'O', 0x01, 0x01, 0x00 };
char GSPI_AcousticData[2] = "";

char GSPI_VoltReq[] = { 'V', 0x01, 0x01, 0x01, 0x01, 0x00 };
char GSPI_VoltData[4] = "";

char GSPI_TempReq[] = { 'T', 0x01, 0x01, 0x00 };
char GSPI_TempData[2] = "";

char GSPI_RpmReq[] = { 'R', 0x01, 0x01, 0x00 };
char GSPI_RpmData[2] = "";

char GSPI_StatusReq[] = { 'S', 0x01, 0x01, 0x00 };
char GSPI_StatusData[2] = "";




void SPI_init ()
{
	SPI1STATbits.SPIROV = 0;
	
	SPI1CONbits.MODE16 = 0;
	SPI1CONbits.SMP = 0;
	SPI1CONbits.CKP = 1;
	SPI1CONbits.CKE = 0;		// might need to change this
	SPI1CONbits.MSTEN = 1;
	SPI1CONbits.SSEN = 0;
	SPI1CONbits.SPRE = 0;
	SPI1CONbits.PPRE = 0b00;
	
}

void SPI_tx_byte ( char ch)
{
	char c;
	
	
	SPI1STATbits.SPIEN = 1;
	SPI1BUF = ch;
	c = SPI1BUF;

	//SPI1STATbits.SPIEN = 0;
}

void SPI_tx_command ( char packet[MAXPACKLEN], char len )
{
	char cnt = 0;
	
	for (cnt = 0; cnt < (len - 1); cnt++)
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
	char i;
	char rcv[7] = "";
	unsigned char check = 0;
	char bite = 0;
	
	for (cnt = 0; cnt < strlen(packet); cnt++)
	{
		
		while (SPI1STATbits.SPITBF);
		SPI_tx_byte( packet[cnt] );	
		
		while (!SPI1STATbits.SPIRBF);
		
		if (cnt > 0)
		{
			bite = SPI1BUF;
			if (check == 0xA5) 
			{ 
				data[cnt - 1] = bite; 
			}
		}
		else
		{
			check = SPI1BUF;
		}
		
		
		for (i = 0; i < 100; i++);
		
	} 
	LATDbits.LATD0 ^= 1;
}
