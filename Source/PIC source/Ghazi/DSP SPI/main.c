#include <p30fxxxx.h>
#include <string.h>

#include "LCD Display.h"	// can be removed in release version
#include "../Libraries/RFprotocol.h"

#define FCY			1842500 // Instruction cycle freq = xtal / 4
#define MAXPACKLEN 	256		// maximum packet length
//#define baud		19200

void SPI_init ( void );
// Initialize the SPI module

void SPI_tx_ch ( char );
// transmit 1 byte on the SPI bus

void SPI_tx_s ( char[MAXPACKLEN], char len );
//transmit a string of character on the SPI bus, expect no response

void SPI_tx_req ( char[MAXPACKLEN], char data[MAXPACKLEN] );
//transmit a request for info on the SPI bus, record the response in "data"

void GP_init_UART( unsigned int baud );

void GP_TX_char ( char ch );

void GP_TX_packet ( char packet[MAXPACKLEN], unsigned short len );

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



//*******************************   MAIN   **********************************
int main ( void )
{
	long int i;
	char lcv;

	
	TRISAbits.TRISA12 = 1;
	
	TRISD = 0x00;
	
	TRISFbits.TRISF3 = 0;
	TRISFbits.TRISF6 = 0;	// SCLK = RF6
	TRISFbits.TRISF7 = 1;	// SDI = RF7
	TRISFbits.TRISF8 = 0;	// SDO = RF8
	
	//SPI1BUF = 0;
	SPI_init();
	
	//Display_Setup();
	//LCD_HomeClear();
	//Display_String(GSPI_test);
	
	GP_init_UART(19200);
	
	// Turn off the LEDs on the dsPICDEM board
	LATDbits.LATD0 = 1;
	LATDbits.LATD1 = 1;
	LATDbits.LATD2 = 1;
	LATDbits.LATD3 = 1;
	
	//SPI_tx_req(GSPI_AccReq, GSPI_AccData);
	
	while(1)
	{
		
		
		if (!PORTAbits.RA12)
		{	
			
			while(!PORTAbits.RA12); // wait for button release
			
			switch (lcv)
			{
				case 0:
					SPI_tx_req(GSPI_2GyroReq, GSPI_2GyroData); // transmit the request
					GP_TX_packet(GSPI_2GyroData, strlen(GSPI_2GyroReq)-1);
					lcv++;
					break;
					
			
				case 1:
					SPI_tx_req(GSPI_CompReq, GSPI_CompData); // transmit the request
					GP_TX_packet(GSPI_CompData, strlen(GSPI_CompReq)-1);
					lcv++;
					break;	
					
				case 2:
					SPI_tx_req(GSPI_AccReq, GSPI_AccData); // transmit the request
					GP_TX_packet(GSPI_AccData, strlen(GSPI_AccReq)-1);
					lcv++;
					break;	
					
				case 3:
					SPI_tx_req(GSPI_AcousticReq, GSPI_AcousticData); // transmit the request
					GP_TX_packet(GSPI_AcousticData, strlen(GSPI_AcousticReq)-1);
					lcv++;
					break;
					
				case 4:
					SPI_tx_req(GSPI_VoltReq, GSPI_VoltData); // transmit the request
					GP_TX_packet(GSPI_VoltData, strlen(GSPI_VoltReq)-1);
					lcv++;
					break;	
					
				case 5:
					SPI_tx_req(GSPI_TempReq, GSPI_TempData); // transmit the request
					GP_TX_packet(GSPI_TempData, strlen(GSPI_TempReq)-1);
					lcv++;
					break;
					
				case 6:
					SPI_tx_req(GSPI_RpmReq, GSPI_RpmData); // transmit the request
					GP_TX_packet(GSPI_RpmData, strlen(GSPI_RpmReq)-1);
					lcv++;
					break;
					
				case 7:
					SPI_tx_req(GSPI_StatusReq, GSPI_StatusData); // transmit the request
					GP_TX_packet(GSPI_StatusData, strlen(GSPI_StatusReq)-1);
					lcv = 0;
					break;	
					
					
						
			}
		
		}
	}
	

	return 0;
}
//***************************************************************************

void GP_init_UART( unsigned int baud )
// GP_init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'
{
	U1MODEbits.ALTIO = 0;		// use U1TX and U1RX as tx/rx pins
	U1MODEbits.LPBACK = 0;		// disable loopback mode
	U1MODEbits.PDSEL = 0b00;	// 8 data bits, no parity
	U1MODEbits.STSEL = 0;		// 1 stop bit

	U1BRG = ( FCY / (16 * baud) ) - 1; // calculate the BRG value for a
									   // given baud rate
	//U1BRG = 1;
	U1MODEbits.UARTEN = 1;		// enable the UART
	IEC0bits.U1RXIE = 1;		// enable the UART RX interrupt
}

void GP_TX_char ( char ch )
// transmit 1 byte on the open UART port 
{
	
	while ( !U1STAbits.TRMT );	// wait until the Transmit Shift Register is not full
													
	U1STAbits.UTXEN = 1;		// enable the transmitter
	U1TXREG = ch;				// load the transmit shift register
}

void GP_TX_packet ( char packet[MAXPACKLEN], unsigned short len )
{
	unsigned short lcv;
	IEC0bits.U1RXIE = 0;
	for (lcv = 0; lcv < len; lcv++)
	{
		GP_TX_char ( packet[lcv] );
	}
	IEC0bits.U1RXIE = 1;
}

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

void SPI_tx_req ( char packet[MAXPACKLEN], char data[MAXPACKLEN] )
{
	char cnt = 0;
	char dummy = strlen(packet);
	char dummy2 = strlen(data);
	char i;
	char rcv[7] = "";
	
	for (cnt = 0; cnt < strlen(packet); cnt++)
	{
		
		while (SPI1STATbits.SPITBF);
		SPI_tx_ch( packet[cnt] );	
		
		while (!SPI1STATbits.SPIRBF);
	
		if (cnt > 0)
		{
			data[cnt - 1] = SPI1BUF;
		}
		
		
		for (i = 0; i < 100; i++);
		
	} 
	//GP_TX_packet(rcv, 7);
	LATDbits.LATD0 ^= 1;
}
