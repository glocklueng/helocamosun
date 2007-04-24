#include <p30fxxxx.h>
#include <string.h>
#include "../Libraries/RFprotocol.h"
#include "../Libraries/SPI.h"

//#define FCY			1842500 // Instruction cycle freq = xtal / 4
//

void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);
// The UART1 RX ISR. Fires when a character is received on UART1

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void);
// Hopefully the TIMER1 UART

//*******************************   MAIN   **********************************
int main ( void )
{
	TRISAbits.TRISA12 = 1;
	TRISD = 0x00;
	TRISFbits.TRISF3 = 0;
	TRISFbits.TRISF6 = 0;	// SCLK = RF6
	TRISFbits.TRISF7 = 1;	// SDI = RF7
	TRISFbits.TRISF8 = 0;	// SDO = RF8
	
	
	GP_init_UART( 19200 );
	TRISB = 0;
	GP_pitch = 0;
	GP_roll = 0;
	GP_yaw = 0;
	GP_coll = 0;
	GP_engRPM = 0;
	GP_errorSOT = 0;
	GP_errorEOT = 0;
	GP_hs = 0;

	

	
	
	
	SPI_init();	
	GP_init_UART(19200);
	
	// Turn off the LEDs on the dsPICDEM board
	LATDbits.LATD0 = 1;
	LATDbits.LATD1 = 1;
	LATDbits.LATD2 = 1;
	LATDbits.LATD3 = 1;
	
	while(1)
	{
		GP_state_machine();
		if (GP_datavalid)
		{
			GP_datavalid = 0;
			GP_parse_data(GP_data, GP_data_len);
		}
		else
		{
			if (GP_hs)
			{
				// have to use time slicing, and only send this at intervals
				GP_TX_packet(GP_handshake, 9);	
			}
		}
	
	}
	return 0;
}
//***************************************************************************


void __attribute__(( interrupt, no_auto_psv )) _U1RXInterrupt(void)
{
	IEC0bits.U1RXIE = 0;	// disable the receive interrupt
	IFS0bits.U1RXIF = 0;	// clear the receive interrupt flag
	GP_bytercvd = 1;		// indicate a byte was received
	GP_dump = U1RXREG;		// read the byte from the receive register
	IEC0bits.U1RXIE = 1;	// re-enable the receive interrupt
	LATDbits.LATD0 ^= 1;
}

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void)
{}
