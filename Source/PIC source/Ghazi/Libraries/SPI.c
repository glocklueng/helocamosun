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
	SPI1CONbits.CKE = 0;		// might need to change this (1 works, 0 is testing Yaw Gyro)
	SPI1CONbits.MSTEN = 1;
	SPI1CONbits.SSEN = 0;
	
	// Really Fast:
	//SPI1CONbits.SPRE = 7;
	//SPI1CONbits.PPRE = 3;
	
	// Really Slow:
	SPI1CONbits.SPRE = 0;
	SPI1CONbits.PPRE = 0;
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

void SPI_tx_command ( char packet[MAXPACKLEN], char len )
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
		
		if (cnt > 0)
		{
			bite = SPI1BUF;
			//if (check == 0xA5) 
			//{ 
				data[cnt - 1] = bite; 
			//}
		}
		else
		{
			check = SPI1BUF;
		}
		
		
		for (i = 0; i < 500; i++);
		
	} 
}

void SPI_readYawGyro ( void )
{
	int i;
	
	SPI1CONbits.MODE16 = 1;
	
	LATBbits.LATB0 = 0;
	SPI_tx_word( 0b1000001100000000 );
	for (i = 0; i < 1700; i++);
	
	LATBbits.LATB0 = 1;
	
	for (i = 0; i < 1000; i++);
	
	
	LATBbits.LATB0 = 0;
	SPI_tx_word( 1 );
	for (i = 0; i < 1700; i++);
	LATBbits.LATB0 = 1;
	
	SPI1CONbits.MODE16 = 0;
	
	
	for (i = 0; i < 10000; i++);
	
}
