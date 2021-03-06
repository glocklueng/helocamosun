#include <p30fxxxx.h>
#include <string.h>
#include "../Libraries/RFprotocol.h"
#include "../Libraries/SPI.h"

#define FCY			1842500 // Instruction cycle freq = xtal / 4

unsigned char GP_bytercvd = 0;  	// 0 = no byte in buffer, 1 = byte in buffer
unsigned char GP_datavalid = 0;	// 0 = no valid data ready, 1 = valid data ready
char GP_data[MAXPACKLEN] = "";	// the stored data portion of the packet (length to checksum exclusive)
char GP_data_len = 0;			// length of GP_data

/*unsigned char GP_engRPM = 0;	// motor RPM stored on DSP
unsigned char GP_pitch = 0;		// pitch servo PWM (0-100)
unsigned char GP_roll = 0;		// roll servo PWM (0-100)
unsigned char GP_yaw = 0;		// yaw servo PWM (0-100)
unsigned char GP_coll = 0;		// collective servo PWM (0-100)*/

unsigned char GP_engRPMsp[3] = "";	// Set Points. 0 = Zero, 1 = 50%, 2 = 100%
unsigned char GP_pitchsp[3] = "";
unsigned char GP_rollsp[3] = "";
unsigned char GP_yawsp[3] = "";
unsigned char GP_collsp[3] = "";
char GP_errorSOT;
char GP_errorEOT;
unsigned char GP_dump;				// holds the byte received on the UART
unsigned char GP_hs = 0;			// 1 = we are in handshake mode


extern GPT_helicopter GP_helicopter;		// global helicopter structure
unsigned char GP_engON = 0;


char GP_err_chksum[] = "Error: Checksum incorrect";
char GP_err_SOT[] = "Error: SOT Invalid";
char GP_err_EOT[] = "Error: EOR Invalid";
char GP_it_works[] = "It works!";
char GP_debug_dv[] = "Data is Valid";
char GP_handshake[]= { 0xa5, 0x5a, 0x02, 0x43, 0x06, 0x00, 0x45, 0xCC, 0x33 } ;
char GP_unk_AA[]= "Error: Unknown AoA";

//char error[] = { 0x01, 0x02, 0x03, 0x04 };

unsigned char newPWM = 0;


void GP_init_UART( unsigned int baud )
{
	// GP_init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
	// at the baud rate passed to 'baud'
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

{
	// transmit 1 byte on the open UART port 
	while ( !U1STAbits.TRMT );	// wait until the Transmit Shift Register is not full
													
	U1STAbits.UTXEN = 1;		// enable the transmitter
	U1TXREG = ch;				// load the transmit shift register
}

void GP_TX_error ( char code )

{
	// Builds and transmits an error packet
	char packet[10] = "";
	
	packet[0] = 0xA5;
	packet[1] = 0x5A;
	packet[2] = 0x03;
	packet[3] = 0x74;
	packet[4] = 0x45;
	packet[5] = code;
	packet[6] = ( (0x03+0x74+0x45 + code) & 0xff00) >> 8; // chksumH
	packet[7] = (0x03+0x74+0x45+ code) & 0x00ff;		  // chksumL
	packet[8] = 0xCC;
	packet[9] = 0x33;
	
	GP_TX_packet(packet, 10);
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
				}
			
				else
				{
					GP_errorSOT = 1;
				}
				break;
			}

			case 1:
			{
				if (GP_dump == 0x5A)
				{
					state++;
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
					GP_data[packet_cnt] = GP_dump;
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
					
						GP_datavalid = 1;
						LATDbits.LATD3 ^= 1;
						GP_data_len = packet_length;
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
					state = 0;
					packet_length = 0;
					packet_cnt = 0;
					chksum = 0;
					checksum[0] = 0;
					checksum[1] = 0;
					check = 0;
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
	unsigned short k;
	
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
					GP_helicopter.pwm.engRPM = vdata[2];
					GP_ACK(vdata, len);
					break;
				}
				
				case 0x50:		// Pitch
				{
					GP_helicopter.pwm.pitch = vdata[2];
					GP_ACK(vdata, len);
					newPWM = 1;
					break;
				}
			
				case 0x52:		// Roll
				{
					GP_helicopter.pwm.roll = vdata[2];
					GP_ACK(vdata, len);
					newPWM = 1;
					break;
				}

				case 0x51:		// Yaw
				{
					GP_helicopter.pwm.yaw = vdata[2];
					GP_ACK(vdata, len);
					newPWM = 1;
					break;
				}	

				case 0x43:		// Collective
				{
					GP_helicopter.pwm.coll = vdata[2];
					GP_ACK(vdata, len);
					newPWM = 1;
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
					GP_engON = 1;
					break;
				}	
				
				case 0x48: 	// Hover
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
					GP_helicopter.goto_position.latitude.deg = vdata[2];
					GP_helicopter.goto_position.latitude.min = vdata[3];
					GP_helicopter.goto_position.latitude.sec = vdata[4] * 256 + vdata[5];
					GP_helicopter.goto_position.latitude.hemi = vdata[6];
					
					GP_helicopter.goto_position.longitude.deg = vdata[7];
					GP_helicopter.goto_position.longitude.min = vdata[8];
					GP_helicopter.goto_position.longitude.sec = vdata[9] * 256 + vdata[10];
					GP_helicopter.goto_position.longitude.hemi = vdata[11];	
					
					GP_helicopter.goto_position.action = vdata[12];
					
					switch(vdata[12])	
					{
						case 0x48:		// Hover at altitude	
						{
							GP_helicopter.goto_position.data = vdata[13] * 256 + vdata[14];
							GP_ACK(vdata, len);
							break;	
						}
						
						case 0x53:		// conduct search pattern	
						{
							GP_helicopter.goto_position.data = vdata[13];
							GP_ACK(vdata, len);
							break;	
						}
						default:
							GP_TX_packet(GP_unk_AA, strlen(GP_unk_AA));
							break;
					}
				
					break;	
				}
				
				case 0x52:	// Return to Base
				{
					GP_ACK(vdata, len);
					break;	
				}
				
				case 0x50:	// Request Pre-flight Packet
				{
					//GP_ACK(vdata, len);
					GP_TX_telemetry(vdata[1]);
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
						case 0x48:	// Heading/Speed/Altitude
						case 0x5A:	// attitude
						case 0x42:	// battery status
						case 0x52:	// motor RPM
						{
							
							GP_TX_telemetry(vdata[2]);
							break;
						}
						default:
						{
							// SEND ERROR
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
					//GP_TX_packet(GP_handshake, 9);
					GP_hs = 1;	
					TMR1 = 0;
					T1CONbits.TON = 1;
					break;
				}	
				
				case 0x54:
				{
					GP_hs = 0;	
					TMR1 = 0;
					T1CONbits.TON = 0;
				}
			}
			break;	
		}
		
	}
	
	for (k = 0; k <= 255; k++)
	{
		vdata[k] = 1;	
	}
	
}


void GP_TX_telemetry( unsigned char type )
{
	unsigned char packet[MAXPACKLEN] = "";
	unsigned char cnt = 0;
	unsigned short chksum = 0;
	
	packet[0] = 0xa5;
	packet[1] = 0x5a;
	packet[3] = 0x74;
	packet[4] = type;
	
	switch (type)
	{
		case 0x4C:	// location
		{
			packet[2] = 12;
			
			packet[5] = GP_helicopter.position.latitude.deg;
			packet[6] = GP_helicopter.position.latitude.min;
			packet[7] = (GP_helicopter.position.latitude.sec & 0xFF00) >> 8;
			packet[8] = (GP_helicopter.position.latitude.sec & 0x00FF);
			packet[9] = GP_helicopter.position.latitude.hemi;
			
			packet[10] = GP_helicopter.position.longitude.deg;
			packet[11] = GP_helicopter.position.longitude.min;
			packet[12] = (GP_helicopter.position.longitude.sec & 0xFF00) >> 8;
			packet[13] = (GP_helicopter.position.longitude.sec & 0x00FF);
			packet[14] = GP_helicopter.position.longitude.hemi;
			
			for (cnt = 2; cnt < 15; cnt++)
			{
				chksum += packet[cnt];	
			}
			
			packet[15] = (chksum & 0xFF00) >> 8;
			packet[16] = chksum & 0x00FF;
			
			packet[17] = 0xCC;
			packet[18] = 0x33;
			
			GP_TX_packet(packet, 19);
			break;
		}
		
		case 0x48:	// Heading/Speed/Altitude	
		{
			packet[2] = 7;
			
			packet[5] = (GP_helicopter.hsa.heading & 0xff00) >> 8;
			packet[6] = GP_helicopter.hsa.heading & 0x00ff;
			packet[7] = GP_helicopter.hsa.speed;
			packet[8] = (GP_helicopter.hsa.altitude & 0xff00) >> 8;
			packet[9] = GP_helicopter.hsa.altitude & 0x00ff;
			
			for (cnt = 2; cnt < 10; cnt++)
			{
				chksum += packet[cnt];	
			}
			
			packet[10] = (chksum & 0xFF00) >> 8;
			packet[11] = chksum & 0x00FF;
			
			packet[12] = 0xCC;
			packet[13] = 0x33;
			
			GP_TX_packet(packet, 14);
			break;
		}
		case 0x5A:	// Attitude
		{
			packet[2] = 8;
			
			//packet[5] = (GP_helicopter.attitude.roll & 0xff00) >> 8;
			//packet[6] = GP_helicopter.attitude.roll & 0x00ff;
			//packet[7] = (GP_helicopter.attitude.pitch & 0xff00) >> 8;
			//packet[8] = GP_helicopter.attitude.pitch & 0x00ff;
			
			
			
			packet[5] = GSPI_AccData[0];
			packet[6] = GSPI_AccData[1];
			
			packet[7] = GSPI_AccData[2];
			packet[8] = GSPI_AccData[3];
			
			packet[9] = GSPI_CompData[0];
			packet[10] = GSPI_CompData[1];
			
			//packet[9] = (GP_helicopter.attitude.yaw & 0xff00) >> 8;
			//packet[10] = GP_helicopter.attitude.yaw & 0x00ff;
			
			for (cnt = 2; cnt < 11; cnt++)
			{
				chksum += packet[cnt];	
			}
			
			packet[11] = (chksum & 0xFF00) >> 8;
			packet[12] = chksum & 0x00FF;
			
			packet[13] = 0xCC;
			packet[14] = 0x33;
			
			GP_TX_packet(packet, 15);
			break;
		}
		case 0x42:	// Battery Status
		{
			packet[2] = 8;
		
			packet[5] = (GP_helicopter.batterystatus.voltage & 0xff00) >> 8;
			packet[6] = GP_helicopter.batterystatus.voltage & 0x00ff;
			packet[7] = (GP_helicopter.batterystatus.current & 0xff00) >> 8;
			packet[8] = GP_helicopter.batterystatus.current & 0x00ff;
			packet[9] = (GP_helicopter.batterystatus.temp & 0xff00) >> 8;
			packet[10] = GP_helicopter.batterystatus.temp & 0x00ff;
			
			for (cnt = 2; cnt < 11; cnt++)
			{
				chksum += packet[cnt];	
			}
			
			packet[11] = (chksum & 0xFF00) >> 8;
			packet[12] = chksum & 0x00FF;
			
			packet[13] = 0xCC;
			packet[14] = 0x33;
			
			GP_TX_packet(packet, 15);
			break;	
		}
		
		case 0x50:	// Pre-flight packet
		{
			packet[2] = 21;
			
			packet[5] = GP_helicopter.position.latitude.deg;
			packet[6] = GP_helicopter.position.latitude.min;
			packet[7] = (GP_helicopter.position.latitude.sec & 0xFF00) >> 8;
			packet[8] = (GP_helicopter.position.latitude.sec & 0x00FF);
			packet[9] = GP_helicopter.position.latitude.hemi;
			
			packet[10] = GP_helicopter.position.longitude.deg;
			packet[11] = GP_helicopter.position.longitude.min;
			packet[12] = (GP_helicopter.position.longitude.sec & 0xFF00) >> 8;
			packet[13] = (GP_helicopter.position.longitude.sec & 0x00FF);
			packet[14] = GP_helicopter.position.longitude.hemi;
			
			packet[15] = (GP_helicopter.GPS_alt & 0xFF00) >> 8;
			packet[16] = (GP_helicopter.GPS_alt & 0x00FF);
			
			packet[17] = (GP_helicopter.SON_alt & 0xFF00) >> 8;
			packet[18] = (GP_helicopter.SON_alt & 0x00FF);
			
			packet[19] = (GP_helicopter.batterystatus.voltage & 0xff00) >> 8;
			packet[20] = GP_helicopter.batterystatus.voltage & 0x00ff;
			
			packet[21] = (GP_helicopter.batterystatus.temp & 0xff00) >> 8;
			packet[22] = GP_helicopter.batterystatus.temp & 0x00ff;
			
			packet[23] = GP_helicopter.sensors;
			
			for (cnt = 2; cnt < 24; cnt++)
			{
				chksum += packet[cnt];	
			}
			
			packet[24] = (chksum & 0xFF00) >> 8;
			packet[25] = chksum & 0x00FF;
			
			packet[26] = 0xCC;
			packet[27] = 0x33;
			
			GP_TX_packet(packet, 28);
			break;	
		}
		case 0x52:	//motor RPM
		{
			packet[2] = 4;
			
			packet[5] = (GP_helicopter.motorRPM & 0xff00) >> 8;
			packet[6] = GP_helicopter.motorRPM & 0x00ff;

			
			for (cnt = 2; cnt < 7; cnt++)
			{
				chksum += packet[cnt];	
			}
			
			packet[7] = (chksum & 0xFF00) >> 8;
			packet[8] = chksum & 0x00FF;
			
			packet[9] = 0xCC;
			packet[10] = 0x33;
			
			GP_TX_packet(packet, 11);
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

void GP_init_chopper( void )
{
	GP_helicopter.goto_position.latitude.deg = 48;
	GP_helicopter.goto_position.latitude.min = 30;
	GP_helicopter.goto_position.latitude.sec = 0;
	GP_helicopter.goto_position.latitude.hemi = 0x4E;
	
	GP_helicopter.goto_position.longitude.deg = 124;
	GP_helicopter.goto_position.longitude.min = 30;
	GP_helicopter.goto_position.longitude.sec = 0;
	GP_helicopter.goto_position.longitude.hemi = 0x57;
	
	GP_helicopter.position.latitude.deg = 48;
	GP_helicopter.position.latitude.min = 30;
	GP_helicopter.position.latitude.sec = 0;
	GP_helicopter.position.latitude.hemi = 0x4E;
	
	GP_helicopter.position.longitude.deg = 124;
	GP_helicopter.position.longitude.min = 30;
	GP_helicopter.position.longitude.sec = 0;
	GP_helicopter.position.longitude.hemi = 0x57;
	
	GP_helicopter.hsa.heading = 0;
	GP_helicopter.hsa.speed = 0;
	GP_helicopter.hsa.altitude = 0;
	
	GP_helicopter.attitude.pitch = 0;
	GP_helicopter.attitude.roll = 0;
	GP_helicopter.attitude.yaw = 0;
	
	GP_helicopter.batterystatus.voltage = 100;
	GP_helicopter.batterystatus.current = 0;
	GP_helicopter.batterystatus.temp = 20;
	
	GP_helicopter.GPS_alt = 0;
	GP_helicopter.SON_alt = 0;
	GP_helicopter.sensors = 0xFF;
}
