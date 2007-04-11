#include <p30fxxxx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#define FCY 		10000000
#define FCY			14740000		// instruction frequency (7.37MHz * 8x PLL / 4 clock per inst
#define MAXPACKLEN 	256

void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);

void init_UART( unsigned short );
// init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'

void TX_char ( char );
// transmit 1 byte on the open UART port 

void TX_packet ( char *packet, unsigned short len );
// transmit a packet of length 'len'

void state_machine ( void );
// advances the state machine through a received packet

void parse_data ( char vdata[MAXPACKLEN] );
// parses the data portion of a valid packet

char *itoa(char *buffer, int i);

//-------------------------- Global Variables -------------------------------
unsigned char bytercvd = 0;  	// 0 = no byte in buffer, 1 = byte in buffer
unsigned char datavalid = 0;	// 0 = no valid data ready, 1 = valid data ready
char data[MAXPACKLEN] = "";
unsigned char engRPM = 0;
unsigned char pitch = 0;
unsigned char roll = 0;
unsigned char yaw = 0;
unsigned char coll = 0;
//--------------------------- Error Messages --------------------------------
char err_chksum[] = "Error: Checksum incorrect\n";
char err_SOT[] = "Error: SOT Invalid\n";
char err_EOT[] = "Error: EOR Invalid\n";

char debug_dv[] = "Data is Valid\n";


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
	TRISBbits.TRISB0 = 0;
	while(1)
	{
		
		//LATBbits.LATB0 ^= 1;
		state_machine();
		if (datavalid)
		{
			//LATBbits.LATB0 ^= 1;
			datavalid = 0;
			//TX_packet( debug_dv, strlen(debug_dv) );
			parse_data(data);
			//TX_packet ( data, strlen(data) );
		}
	}
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
	
	while (!U1STAbits.TRMT);	// wait until the Transmit Shift Register is not full 
	U1STAbits.UTXEN = 1;
	U1TXREG = ch;
}

void TX_packet ( char packet[256], unsigned short len )
{
	unsigned short lcv;
	for (lcv = 0; lcv < len; lcv++)
	{
		TX_char ( packet[lcv] );
	}
	// THIS IS FOR DEBUGGING ONLY
	TX_char('\n');
}

void state_machine ( void )
{
	static unsigned short state = 0;
	static unsigned short chksum = 0;
	static unsigned short packet_length = 0;
	static unsigned char packet_cnt = 0;
	static char packet[256] = "";
	static unsigned short checksum[2];
	static unsigned short check = 0;
	short lcv; 

	unsigned char dump;
	//LATBbits.LATB0 ^= 1;
	if (bytercvd)
	{
		LATBbits.LATB0 ^= 1;
		bytercvd = 0;
		dump = U1RXREG;
		switch (state)
		{
			case 0:
			{
				if (dump == 0xA5)
				{
					chksum = 0;
					state++;
					
				}
			
				else
				{
					//TX_packet(err_SOT, strlen(err_SOT) );
				}
				break;
			}

			case 1:
			{
				if (dump == 0x5A)
				{
					state++;
					
				}

				else 
				{
					//TX_packet(err_SOT, strlen(err_SOT) );
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
				state++;
				break;
			}

			case 6:
			{
				if (dump = 0xCC) 
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
				//chksum += dump;
				if (dump == 0x33) 
				{
					if (check == chksum)
					{
						// The calculated checksum matched the transmitted one:
						//TX_packet(packet, packet_length);	
						strcpy(data, packet);
						datavalid = 1;
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

		}
	}	
}

void parse_data ( char vdata[MAXPACKLEN] )
{
	char msg[256] = "";
	char packet_msg[] = "Packet Received: ";
	char engRPM_msg[] = "Engine RPM set to ";
	char pitch_msg[] = "Pitch set to ";
	char yaw_msg[] = "Yaw set to ";
	char roll_msg[] = "Roll set to ";
	char coll_msg[] = "Collective set to ";
	char val[5] = "";
	switch (data[0])
	{
		
		case 0x54:		// Testing/Tuning Commands
		{
			switch (data[1])
			{
				case 0x45:		// Engine RPM
				{
					engRPM = data[2];
					sprintf( val, "%d", data[2] ); 
					strcat(msg, packet_msg);
					strcat(msg, engRPM_msg);
					strcat(msg, val);
					TX_packet( msg, strlen(msg) );
					break;
				}
				
				case 0x50:		// Pitch
				{
					pitch = data[2];
					sprintf( val, "%d", data[2] ); 
					strcat(msg, packet_msg);
					strcat(msg, pitch_msg);
					strcat(msg, val);
					TX_packet( msg, strlen(msg) );
					break;
				}
			
				case 0x52:		// Roll
				{
					roll = data[2];
					sprintf( val, "%d", data[2] ); 
					strcat(msg, packet_msg);
					strcat(msg, roll_msg);
					strcat(msg, val);
					TX_packet( msg, strlen(msg) );
					break;
				}

				case 0x51:		// Yaw
				{
					yaw = data[2];
					sprintf( val, "%d", data[2] ); 
					strcat(msg, packet_msg);
					strcat(msg, yaw_msg);
					strcat(msg, val);
					TX_packet( msg, strlen(msg) );
					break;
				}	

				case 0x43:		// Collective
				{
					coll = data[2];
					sprintf( val, "%d", data[2] ); 
					strcat(msg, packet_msg);
					strcat(msg, coll_msg);
					strcat(msg, val);
					TX_packet( msg, strlen(msg) );
					break;
				}
			}
		break;
		}
		
	}
		
}

//void _ISR _U1RXInterrupt ( void )
void __attribute__(( interrupt, no_auto_psv )) _U1RXInterrupt(void)
{
	//LATBbits.LATB0 ^= 1;
	IEC0bits.U1RXIE = 0;
	IFS0bits.U1RXIF = 0;
	bytercvd = 1;
	IEC0bits.U1RXIE = 1;
}
