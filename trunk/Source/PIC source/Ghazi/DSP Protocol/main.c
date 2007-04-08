#include <p30fxxxx.h>
//#include <string.h>

#define FCY 10000000


void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);

void init_UART( unsigned short );
// init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'

void TX_char ( unsigned char );
// transmit 1 byte on the open UART port 

void TX_packet ( unsigned char *packet, unsigned short len );
// transmit a packet of length 'len'

void state_machine ( void );
// advances the state machine through a received packet

unsigned char bytercvd;  	// 0 = no byte in buffer, 1 = byte in buffer

int main ( void )
{
	IEC0bits.U1RXIE = 1;
	init_UART( 19200 );
	TRISB = 0;
	while(1)
	{
		state_machine();
	}
	return 0;
}

void init_UART( unsigned short baud )
// init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'
{
	U1MODEbits.ALTIO = 0;		// use U1TX and U1RX as tx/rx pins
	U1MODEbits.LPBACK = 0;		// disable loopback mode
	U1MODEbits.PDSEL = 0b00;	// 8 data bits, no parity
	U1MODEbits.STSEL = 0;		// 1 stop bit

	U1BRG = ( FCY / (16 * baud) ) - 1; // calculate the BRG value for a
									   // given baud rate
	U1MODEbits.UARTEN = 1;		// enable the UART
}

void TX_char ( unsigned char ch )
// transmit 1 byte on the open UART port 
{
	U1TXREG = ch;
}

void TX_packet ( unsigned char *packet, unsigned short len )
{
	unsigned short lcv;
	U1STAbits.UTXEN = 1;
	for (lcv = 0; lcv < len; lcv++)
	{
		TX_char ( packet[lcv] );
	}
}

void state_machine ( void )
{
	static unsigned short state = 0;
	static unsigned short chksum = 0;

	if (bytercvd)
	{
		bytercvd = 0;
		switch (state)
		{
			case 0:
			{
				if (U1RXREG == 0xA5)
				{
					chksum = 0;
					state++;
					break;
				}
			}

			case 1:
			{
				if (U1RXREG == 0x51)
				{
					state++;
					break;
				}

				else state = 0;
			}
		
			default:
			{
				state++;
				// NEED TO RECORD DATA PACKETS UNTIL EOT
			}


		}
	}	
}

//void _ISR _U1RXInterrupt ( void )
void __attribute__(( interrupt, no_auto_psv )) _U1RXInterrupt(void)
{
	IEC0bits.U1RXIE = 0;
	IFS0bits.U1RXIF = 0;
	bytercvd = 1;
	IEC0bits.U1RXIE = 1;
}
