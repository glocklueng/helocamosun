#include <p30fxxxx.h>
#include <string.h>

#define FCY			1842500 // Instruction cycle freq = xtal / 4


#include "LCD Display.h"	// can be removed in release version
#include "../Libraries/RFprotocol.h"
#include "../Libraries/SPI.h"


//#define baud		19200


void GP_init_UART( unsigned int baud );

void GP_TX_char ( char ch );

void GP_TX_packet ( char packet[MAXPACKLEN], unsigned short len );




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
