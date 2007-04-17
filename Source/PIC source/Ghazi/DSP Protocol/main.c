#include <p30fxxxx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <xlcd.h>
#include <delay.h>
#include <uart.h>
#include <i2c.h>

#define FCY			1842500 // Instruction cycle freq = xtal / 4
#define MAXPACKLEN 	256		// maximum packet length

void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);
// The UART1 RX ISR. Fires when a character is received on UART1

void init_UART( unsigned short );
// init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'

void TX_char ( char );
// transmit 1 byte on the open UART port 

void TX_packet ( char *packet, unsigned short len );
// transmit a packet of length 'len'

void state_machine ( void );
// advances the state machine through a received packet

void parse_data ( char vdata[MAXPACKLEN], char len );
// parses the data portion of a valid packet

void ACK( char vdata[MAXPACKLEN], char len );
// builds an acknowledgement packet based on the contents of data

char *itoa(char *buffer, int i);
// converts an integer into a string

void i2c_setup( void );
//-------------------------- Global Variables -------------------------------
unsigned char bytercvd = 0;  	// 0 = no byte in buffer, 1 = byte in buffer
unsigned char datavalid = 0;	// 0 = no valid data ready, 1 = valid data ready
char data[MAXPACKLEN] = "";
char data_len = 0;
unsigned char engRPM = 0;
unsigned char pitch = 0;
unsigned char roll = 0;
unsigned char yaw = 0;
unsigned char coll = 0;
unsigned char engRPMsp[3] = "";
unsigned char pitchsp[3] = "";
unsigned char rollsp[3] = "";
unsigned char yawsp[3] = "";
unsigned char collsp[3] = "";
char errorSOT;
char errorEOT;
unsigned char dump;
unsigned char hs = 0;
//--------------------------- Messages --------------------------------
char err_chksum[] = "Error: Checksum incorrect\n";
char err_SOT[] = "Error: SOT Invalid\n";
char err_EOT[] = "Error: EOR Invalid\n";
char it_works[] = "It works!";
char debug_dv[] = "Data is Valid\n";
char handshake[] = { 0xa5, 0x5a, 0x02, 0x43, 0x06, 0x00, 0x45, 0xCC, 0x33 } ;

//---------------------------------------------------------------------------

//*******************************   MAIN   **********************************
int main ( void )
{


	IEC0bits.U1RXIE = 1;
	init_UART( 19200 );
	TRISB = 0;
	pitch = 0;
	roll = 0;
	yaw = 0;
	coll = 0;
	engRPM = 0;
	errorSOT = 0;
	errorEOT = 0;
	hs = 0;
	
	//TRISBbits.TRISB0 = 0;
	TRISD = 0;
	LATDbits.LATD0 = 1;
	LATDbits.LATD1 = 1;
	LATDbits.LATD2 = 1;
	LATDbits.LATD3 = 1;
	while(1)
	{
		//LATBbits.LATB0 ^= 1;
		
		state_machine();
		if (datavalid)
		{
			datavalid = 0;
			parse_data(data, data_len);
			
		}
		else
		{
			if (hs)
			{
				// have to use time slicing, and only send this at intervals
				TX_packet(handshake, 9);	
			}
		}
	
	}

	//OpenXLCD( EIGHT_BIT );
		
	//OpenXLCD();
	//OpenXLCD( FOUR_BIT & TWO_LINE & INCR_MODE );
	//putcXLCD(0x42);
	/*while(1)
	{
		TX_packet( err_EOT, strlen(err_EOT) );
	}*/
	
	return 0;
}
//***************************************************************************

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

void TX_char ( char ch )
// transmit 1 byte on the open UART port 
{
	
	while ( !U1STAbits.TRMT );	// wait until the Transmit Shift Register is not full
													
	U1STAbits.UTXEN = 1;		// enable the transmitter
	U1TXREG = ch;				// load the transmit shift register
}

void TX_packet ( char packet[MAXPACKLEN], unsigned short len )
{
	unsigned short lcv;
	for (lcv = 0; lcv < len; lcv++)
	{
		TX_char ( packet[lcv] );
	}
}

void state_machine ( void )
{
	static unsigned short state = 0;
	static unsigned short chksum = 0;
	static unsigned short packet_length = 0;
	static unsigned char packet_cnt = 0;
	static char packet[MAXPACKLEN] = "";
	static unsigned short checksum[2];
	static unsigned short check = 0;
	short lcv; 

	if (bytercvd)
	{
		LATDbits.LATD0 ^= 1;
		bytercvd = 0;
	
		switch (state)
		{
		
			case 0:
			{
				if (dump == 0xA5)
				{
					chksum = 0;
					state++;
					LATDbits.LATD1 ^= 1;
					//TX_packet(err_SOT, strlen(err_SOT));
				}
			
				else
				{
					errorSOT = 1;
					//TX_packet(err_SOT, strlen(err_SOT));
				}
				break;
			}

			case 1:
			{
				if (dump == 0x5A)
				{
					state++;
					//LATDbits.LATD1 ^= 1;
				}

				else 
				{
					errorSOT = 1;
					state = 0;
				}
				break;
			}
			
			case 2:
			{
				chksum += dump;
				packet_length = dump;
				state++;
				break;
			}

			case 3:
			{				
				if (packet_cnt < packet_length)
				{
					chksum += dump;
					packet[packet_cnt] = dump;
					packet_cnt++;
					if (packet_cnt == packet_length)
					{
						state++;
					}
				}
				else
				{
					state++;
				}		
				break;
			}

			case 4:
			{
				checksum[0] = dump;
				
				state++;
				break;
			}
			case 5:
			{
				checksum[1] = dump;
				check = checksum[0] * 256 + checksum[1];
				LATDbits.LATD2 ^= 1;
				
				state++;
				break;
			}

			case 6:
			{
				if (dump == 0xCC) 
				{
					state++;
					packet_cnt--;
					
				}

				else 
				{
					state = 0;
					chksum = 0;
					packet_length = 0;
					packet_cnt = 0;
					//TX_packet( err_EOT, strlen(err_EOT) );
				}
				break;
			}
			
			

			case 7:
			{
				if (dump == 0x33) 
				{
					if (check == chksum)
					{	
						strcpy(data, packet);
						datavalid = 1;
						LATDbits.LATD3 ^= 1;
						data_len = packet_length;
						//TX_packet( it_works, strlen(it_works) );
					}
					else
					{
						// The calculated checksum did not match the transmitted one:
						TX_packet(err_chksum, strlen(err_chksum));
					
					}
					state = 0;
					packet_length = 0;
					packet_cnt = 0;
					chksum = 0;
					checksum[0] = 0;
					checksum[1] = 0;
					check = 0;
				}
	
				else
				{
					//TX_packet( err_EOT, strlen(err_EOT) );
				}
			
				break;
			}
			default:
			{
				LATDbits.LATD2 ^= 1;
			}
		}
	}	
}

void parse_data ( char vdata[MAXPACKLEN], char len )
{
	char msg[MAXPACKLEN] = "";
	char packet_msg[] = "Packet Received: ";
	char engRPM_msg[] = "Engine RPM set to ";
	char pitch_msg[] = "Pitch set to ";
	char yaw_msg[] = "Yaw set to ";
	char roll_msg[] = "Roll set to ";
	char coll_msg[] = "Collective set to ";
	
	
	unsigned short hover_alt = 0;
	char val[5] = "";
		
	switch (vdata[0])
	{
		
		case 0x54:		// Testing/Tuning Commands
		{
			switch (vdata[1])
			{
				case 0x45:		// Engine RPM
				{
					engRPM = vdata[2];
					ACK(vdata, len);
					break;
				}
				
				case 0x50:		// Pitch
				{
					pitch = vdata[2];
					ACK(vdata, len);
					break;
				}
			
				case 0x52:		// Roll
				{
					roll = vdata[2];
					ACK(vdata, len);
					break;
				}

				case 0x51:		// Yaw
				{
					yaw = vdata[2];
					ACK(vdata, len);
					break;
				}	

				case 0x43:		// Collective
				{
					coll = vdata[2];
					ACK(vdata, len);
					break;
				}
				
				case 0x5A:		// Set Zero Points
				{
					engRPMsp[0] = vdata[2];
					pitchsp[0] = vdata[3];
					rollsp[0] = vdata[4];
					collsp[0] = vdata[5];
					yawsp[0] = vdata[6];
					ACK(vdata, len);
					break;	
				}
				
				case 0x46:		// Set 50% Points
				{
					engRPMsp[1] = vdata[2];
					pitchsp[1] = vdata[3];
					rollsp[1] = vdata[4];
					collsp[1] = vdata[5];
					yawsp[1] = vdata[6];
					ACK(vdata, len);
					break;	
				}
				
				case 0x4D:		// Set MAX Points
				{
					engRPMsp[2] = vdata[2];
					pitchsp[2] = vdata[3];
					rollsp[2] = vdata[4];
					collsp[2] = vdata[5];
					yawsp[2] = vdata[6];
					ACK(vdata, len);
					break;	
				}
			}
		break;
		}
		case 0x46:		// Flight Operations commands
		{
			switch(vdata[1])
			{
				case 0x45:	// Engage Engine
				{
					ACK(vdata, len);
					break;
				}	
				
				case 0x46: 	// Hover
				{
					switch(vdata[2])
					{
						case 0x50:	// Use preset altitude
						{
							ACK(vdata, len);
							break;
						}
						
						case 0x43: // hover at current altitude
						{
							ACK(vdata, len);
							break;	
						}
						
						case 0x4D: // hover at following altitude
						{
							hover_alt = vdata[3] * 256 + vdata[4];
							ACK(vdata, len);
							break;
						}
					}
					break;	
				}
			}
			break;	
		}
		
		case 0x43:		// Communications
		{
			switch (vdata[1])
			{
				case 0x48:
				{
					TX_packet(handshake, 2);
					hs = 1;	
					break;
				}	
				
				case 0x54:
				{
					hs = 0;	
				}
			}
			break;	
		}
		
	}
		
}

void ACK( char vdata[MAXPACKLEN], char len )
{
	char ack[MAXPACKLEN] = "";
	unsigned char lcv = 0;
	unsigned short chksum = 0;

	ack[0] = 0xA5;
	ack[1] = 0x5A;
	ack[2] = len + 1;
	chksum = ack[2];
	ack[3] = 0x06;
	for (lcv = 4; lcv < len + 4; lcv++)
	{
		ack[lcv] = vdata[lcv - 4];
		chksum += ack[lcv];
	}
	ack[lcv] = (chksum & 0xFF00) >> 8;
	ack[lcv + 1] = (chksum & 0x00FF);
	ack[lcv + 2] = 0xCC;
	ack[lcv + 3] = 0x33;
	
	TX_packet(ack, len + 8);
}

void __attribute__(( interrupt, no_auto_psv )) _U1RXInterrupt(void)
{
	IEC0bits.U1RXIE = 0;	// disable the receive interrupt
	IFS0bits.U1RXIF = 0;	// clear the receive interrupt flag
	bytercvd = 1;			// indicate a byte was received
	dump = U1RXREG;			// read the byte from the receive register
	IEC0bits.U1RXIE = 1;	// re-enable the receive interrupt
	
}

void i2c_setup( void )
{
	OpenI2C(1,1);
}
