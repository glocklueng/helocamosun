#include <p30fxxxx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "LCD Display.h"
#include <delay.h>
#include <uart.h>
#include <i2c.h>

#define FCY			1842500 // Instruction cycle freq = xtal / 4
#define MAXPACKLEN 	256		// maximum packet length



void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);
// The UART1 RX ISR. Fires when a character is received on UART1

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void);
// The Timer1 Interrupt Vector.

void SPI_init ( void );
void SPI_tx_ch ( char );
void SPI_tx_s ( char[MAXPACKLEN], char len );
void SPI_tx_req ( char[MAXPACKLEN], char len );

char GSPI_packet[MAXPACKLEN] = ""; 
char GSPI_test[] = "Test Packet";
char GSPI_AccReq[] = { 'A', 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00 };
char GSPI_2GyroReq[] = { 'G', 0x01, 0x01, 0x01, 0x01 };
char GSPI_CompReq[] = { 'C', 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
char GSPI_AcousticReq[] = { 'O', 0x01, 0x01, 0x01, 0x01, 0x01 };
char GSPI_VoltReq[] = { 'V', 0x01, 0x01, 0x01, 0x01 };
char GSPI_TempReq[] = { 'T', 0x01, 0x01 };
char GSPI_RpmReq[] = { 'R', 0x01, 0x01 };
char GSPI_StatusReq[] = { 'S', 0x01, 0x01, 0x01, 0x01 };
char GSPI_YawGyroReq[] = { 'Y', 0x01, 0x01 };	
char rcv[6];

//*******************************   MAIN   **********************************
int main ( void )
{
	long int i;

	TRISAbits.TRISA12 = 1;
	TRISBbits.TRISB0 = 0;
	LATBbits.LATB0 = 0;
	IEC0bits.U1RXIE = 1;
	IEC0bits.T1IE = 1;
	TRISFbits.TRISF6 = 0;	// SCLK = RF6
	TRISFbits.TRISF7 = 1;	// SDI = RF7
	TRISFbits.TRISF8 = 0;	// SDO = RF8
	SPI1BUF = 0;
	SPI_init();
	TRISD = 0x00;
	Display_Setup();
	LCD_HomeClear();
	LATDbits.LATD0 = 1;
	LATDbits.LATD1 = 1;
	LATDbits.LATD2 = 1;
	LATDbits.LATD3 = 1;
	
	

	SPI_tx_req(GSPI_AccReq, strlen(GSPI_AccReq));
	
	while(1)
	{
		
		if (!PORTAbits.RA12)
		{	
			
			while(!PORTAbits.RA12);
			SPI_tx_req(GSPI_AccReq, strlen(GSPI_AccReq));
			
			Display_String(rcv);
		}
	}
	return 0;
}
//***************************************************************************


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

void SPI_tx_ch ( char ch)
{
	char c;
	
	
	SPI1STATbits.SPIEN = 1;
	SPI1BUF = ch;
	c = SPI1BUF;

	//SPI1STATbits.SPIEN = 0;
}

void SPI_tx_s ( char packet[MAXPACKLEN], char len )
{
	char cnt = 0;
	
	for (cnt = 0; cnt < (len - 1); cnt++)
	{
		while (SPI1STATbits.SPITBF);
		SPI_tx_ch( packet[cnt] );	
	} 
}

void SPI_tx_req ( char packet[MAXPACKLEN], char len )
{
	char cnt = 0;
	
	char i;
	
	for (cnt = 0; cnt < len; cnt++)
	{
		
		while (SPI1STATbits.SPITBF);
		SPI_tx_ch( packet[cnt] );	
		while (!SPI1STATbits.SPIRBF);
		rcv[cnt] = SPI1BUF;
		LATDbits.LATD1 ^= 1;
		for (i = 0; i < 100; i++);
	} 
		

	LATDbits.LATD0 ^= 1;
}

void __attribute__(( interrupt, no_auto_psv )) _U1RXInterrupt(void)
{

}

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void)
{

}
