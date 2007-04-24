#include <p30fxxxx.h>
#include <string.h>
#include "../Libraries/RFprotocol.h"

#define FCY			1842500 // Instruction cycle freq = xtal / 4

unsigned char GP_bytercvd = 0;  	// 0 = no byte in buffer, 1 = byte in buffer
unsigned char GP_datavalid = 0;	// 0 = no valid data ready, 1 = valid data ready
char GP_data[MAXPACKLEN] = "";
char GP_data_len = 0;
unsigned char GP_engRPM = 0;
unsigned char GP_pitch = 0;
unsigned char GP_roll = 0;
unsigned char GP_yaw = 0;
unsigned char GP_coll = 0;
unsigned char GP_engRPMsp[3] = "";	// Set Points. 0 = Zero, 1 = 50%, 2 = 100%
unsigned char GP_pitchsp[3] = "";
unsigned char GP_rollsp[3] = "";
unsigned char GP_yawsp[3] = "";
unsigned char GP_collsp[3] = "";
char GP_errorSOT;
char GP_errorEOT;
unsigned char GP_dump;
unsigned char GP_hs = 0;
GPT_goto_position GP_goto;

char GP_err_chksum[] = "Error: Checksum incorrect\n";
char GP_err_SOT[] = "Error: SOT Invalid\n";
char GP_err_EOT[] = "Error: EOR Invalid\n";
char GP_it_works[] = "It works!";
char GP_debug_dv[] = "Data is Valid\n";
char GP_handshake[]= { 0xa5, 0x5a, 0x02, 0x43, 0x06, 0x00, 0x45, 0xCC, 0x33 } ;




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

void GP_state_machine ( void )
{
	static unsigned short state = 0;
	static unsigned short chksum = 0;
	static unsigned short packet_length = 0;
	static unsigned char packet_cnt = 0;
	static char packet[MAXPACKLEN] = "";
	static unsigned short checksum[2];
	static unsigned short check = 0;
	short lcv; 

	if (GP_bytercvd)
	{
		
		GP_bytercvd = 0;
	
		switch (state)
		{
		
			case 0:
			{
				if (GP_dump == 0xA5)
				{
					chksum = 0;
					state++;
					LATDbits.LATD1 ^= 1;
					//GP_TX_packet(GP_err_SOT, strlen(GP_err_SOT));
				}
			
				else
				{
					GP_errorSOT = 1;
					//GP_TX_packet(GP_err_SOT, strlen(GP_err_SOT));
				}
				break;
			}

			case 1:
			{
				if (GP_dump == 0x5A)
				{
					state++;
					//LATDbits.LATD1 ^= 1;
				}

				else 
				{
					GP_errorSOT = 1;
					state = 0;
				}
				break;
			}
			
			case 2:
			{
				chksum += GP_dump;
				packet_length = GP_dump;
				state++;
				break;
			}

			case 3:
			{				
				if (packet_cnt < packet_length)
				{
					chksum += GP_dump;
					packet[packet_cnt] = GP_dump;
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
				checksum[0] = GP_dump;
				
				state++;
				break;
			}
			case 5:
			{
				checksum[1] = GP_dump;
				check = checksum[0] * 256 + checksum[1];
				LATDbits.LATD2 ^= 1;
				
				state++;
				break;
			}

			case 6:
			{
				if (GP_dump == 0xCC) 
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
					//GP_TX_packet( GP_err_EOT, strlen(GP_err_EOT) );
				}
				break;
			}
			
			

			case 7:
			{
				if (GP_dump == 0x33) 
				{
					if (check == chksum)
					{	
						strcpy(GP_data, packet);
						GP_datavalid = 1;
						LATDbits.LATD3 ^= 1;
						GP_data_len = packet_length;
						//GP_TX_packet( GP_it_works, strlen(GP_it_works) );
					}
					else
					{
						// The calculated checksum did not match the transmitted one:
						GP_TX_packet(GP_err_chksum, strlen(GP_err_chksum));
					
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
					//GP_TX_packet( GP_err_EOT, strlen(GP_err_EOT) );
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

void GP_parse_data ( char vdata[MAXPACKLEN], char len )
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
					GP_engRPM = vdata[2];
					GP_ACK(vdata, len);
					break;
				}
				
				case 0x50:		// Pitch
				{
					GP_pitch = vdata[2];
					GP_ACK(vdata, len);
					break;
				}
			
				case 0x52:		// Roll
				{
					GP_roll = vdata[2];
					GP_ACK(vdata, len);
					break;
				}

				case 0x51:		// Yaw
				{
					GP_yaw = vdata[2];
					GP_ACK(vdata, len);
					break;
				}	

				case 0x43:		// Collective
				{
					GP_coll = vdata[2];
					GP_ACK(vdata, len);
					break;
				}
				
				case 0x5A:		// Set Zero Points
				{
					GP_engRPMsp[0] = vdata[2];
					GP_pitchsp[0] = vdata[3];
					GP_rollsp[0] = vdata[4];
					GP_collsp[0] = vdata[5];
					GP_yawsp[0] = vdata[6];
					GP_ACK(vdata, len);
					break;	
				}
				
				case 0x46:		// Set 50% Points
				{
					GP_engRPMsp[1] = vdata[2];
					GP_pitchsp[1] = vdata[3];
					GP_rollsp[1] = vdata[4];
					GP_collsp[1] = vdata[5];
					GP_yawsp[1] = vdata[6];
					GP_ACK(vdata, len);
					break;	
				}
				
				case 0x4D:		// Set MAX Points
				{
					GP_engRPMsp[2] = vdata[2];
					GP_pitchsp[2] = vdata[3];
					GP_rollsp[2] = vdata[4];
					GP_collsp[2] = vdata[5];
					GP_yawsp[2] = vdata[6];
					GP_ACK(vdata, len);
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
					GP_ACK(vdata, len);
					break;
				}	
				
				case 0x46: 	// Hover
				{
					switch(vdata[2])
					{
						case 0x50:	// Use preset altitude
						{
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x43: // hover at current altitude
						{
							GP_ACK(vdata, len);
							break;	
						}
						
						case 0x4D: // hover at following altitude
						{
							hover_alt = vdata[3] * 256 + vdata[4];
							GP_ACK(vdata, len);
							break;
						}
					}
					break;	
				}
				
				case 0x47:	// GOTO
				{
					GP_goto.latitude.deg = vdata[2];
					GP_goto.latitude.min = vdata[3];
					GP_goto.latitude.sec = vdata[4] * 256 + vdata[5];
					GP_goto.latitude.hemi = vdata[6];
					
					GP_goto.longitude.deg = vdata[7];
					GP_goto.longitude.min = vdata[8];
					GP_goto.longitude.sec = vdata[9] * 256 + vdata[10];
					GP_goto.longitude.hemi = vdata[11];	
					
					GP_goto.action = vdata[12];
					
					switch(vdata[12])	
					{
						case 0x48:		// Hover at altitude	
						{
							GP_goto.data = vdata[13] * 256 + vdata[14];
							break;	
						}
						
						case 0x53:		// conduct search pattern	
						{
							GP_goto.data = vdata[13];
							break;	
						}
					}
				
					break;	
				}
				
				case 0x52:	// Return to Base
				{
					GP_ACK(vdata, len);
					break;	
				}
				
				case 0x50:	// Return to Base
				{
					GP_ACK(vdata, len);
					break;	
				}
				
				case 0x4D:	// Discreet movement control
				{
					switch(vdata[2])
					{
						case 0x46:	// Forward
						{
							// Move forward one "unit"
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x52:	// Reverse
						{
							// Move backward one "unit"
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x50:	// left
						{
							// Move left one "unit"
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x53:	// right
						{
							// Move right one "unit"
							GP_ACK(vdata, len);
							break;
						}
					}
					break;	
				}
				
				case 0x49:	// Request for info
				{
					switch(vdata[2])
					{
						case 0x4C:	// location
						{
							// Send location packet
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x48:	// Heading/Speed/Altitude
						{
							// send HSA packet
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x5A:	// attitude
						{
							// send attitude packet
							GP_ACK(vdata, len);
							break;
						}
						
						case 0x42:	// battery status
						{
							// send battery status packet
							GP_ACK(vdata, len);
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
					GP_TX_packet(GP_handshake, 2);
					GP_hs = 1;	
					break;
				}	
				
				case 0x54:
				{
					GP_hs = 0;	
				}
			}
			break;	
		}
		
	}
		
}

void GP_ACK( char vdata[MAXPACKLEN], char len )
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
	
	GP_TX_packet(ack, len + 8);
}


