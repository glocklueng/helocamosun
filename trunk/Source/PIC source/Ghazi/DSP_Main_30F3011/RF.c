#include <p30fxxxx.h>
#include <string.h>

#include "RF.h"
#include "SPI.h"

unsigned char GP_bytercvd = 0;  	// 0 = no byte in buffer, 1 = byte in buffer
unsigned char GP_datavalid = 0;	// 0 = no valid data ready, 1 = valid data ready
char GP_data[MAXPACKLEN] = "";	// the stored data portion of the packet (length to checksum exclusive)
char GP_data_len = 0;			// length of GP_data

unsigned char GP_engRPMsp[3] = "";	// Set Points. 0 = Zero, 1 = 50%, 2 = 100%
unsigned char GP_pitchsp[3] = "";
unsigned char GP_rollsp[3] = "";
unsigned char GP_yawsp[3] = "";
unsigned char GP_collsp[3] = "";

unsigned char GP_dump;				// holds the byte received on the UART
unsigned char GP_hs = 0;			// 1 = we are in handshake mode

GPT_helicopter GP_helicopter;		// global helicopter structure
unsigned char GP_engON = 0;
 
char modeFuzzy = 1;					// default to Testing/Tuning control

signed short latcorr = 0, longcorr = 0, altcorr = 0, seconds = 0;
//char GP_handshake[]= { 0xa5, 0x5a, 0x02, 0x43, 0x06, 0x00, 0x45, 0xCC, 0x33 };

unsigned char newPWM = 0;


void GP_TX_char ( char ch )

{
	// transmit 1 byte on the open UART port 
	while ( !U2STAbits.TRMT );	// wait until the Transmit Shift Register is not full
													
	U2STAbits.UTXEN = 1;		// enable the transmitter
	U2TXREG = ch;				// load the transmit shift register
}

void GP_TX_error ( char code )

{
	// Builds and transmits an error packet
	unsigned char packet[10] = "";
	
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

void GP_TX_packet ( unsigned char packet[], unsigned short len )
{
	unsigned short lcv;
	IEC1bits.U2RXIE = 0;
	for (lcv = 0; lcv < len; lcv++)
	{
		GP_TX_char ( packet[lcv] );
	}
	IEC1bits.U2RXIE = 1;
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
	char n;
	
//	GP_TX_char(GP_dump);
	
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
				//	GP_errorSOT = 1;
					state = 0;
					//GP_TX_char(0x01);
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
					//GP_TX_char(0x06);
					
				}
				break;
			}
			
			

			case 7:
			{
			
				if (GP_dump == 0x33) 
				{
					if (check == chksum)
					{	
						LATBbits.LATB4 ^= 1;
						GP_datavalid = 1;
						GP_data_len = packet_length;
					}
					else
					{
						// The calculated checksum did not match the transmitted one:
						//GP_TX_char(0x07);
					
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
	n = n;
}

void GP_parse_data ( char vdata[], char len )
{
	unsigned short k;
	
	unsigned short hover_alt = 0;
	char val[5] = "";
	unsigned char seconds;
	float Ton;
	int pwm;
		
	switch (vdata[0])
	{
		
		case 0x54:		// Testing/Tuning Commands
		{
			switch (vdata[1])
			{
				case 0x45:		// Engine RPM
				{
					GP_helicopter.pwm.engRPM = vdata[2];
					
					//Ton = (float)GP_helicopter.pwm.engRPM / 100000.0 + 0.001;
					
					Ton = (float)vdata[2] / 100000.0 + 0.001;
					PDC1 = (int)(Ton * 156250); 
//					PTCONbits.PTEN = 1;	// Enable PWM Time Base	
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
				
				case 0x66:		// Set Ops Mode
				{
					if (vdata[2])
					{
						modeFuzzy = 1;	
					}
					else
					{
						modeFuzzy = 0;
					}
					GP_ACK(vdata, len);
				}
			}
		break;
		}
		case 0x46:		// Flight Operations commands
		{
			switch(vdata[1])
			{
				case 0x43:	// GPS Correction Factor
				{
					//LATBbits.LATB0 ^= 1;
					GP_ACK(vdata, len);
					latcorr = (vdata[2] << 8) + vdata[3];
					longcorr = (vdata[4] << 8) + vdata[5];
					altcorr = (vdata[6] << 8) + vdata[7];
					seconds = vdata[8];
					//corr_GPSlatlongalt( latcorr, longcorr, altcorr, seconds);
					break;
				}
				
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
					GP_helicopter.goto_position.latitude.min_frac = vdata[4] * 256 + vdata[5];
					GP_helicopter.goto_position.latitude.hemi = vdata[6];
					
					GP_helicopter.goto_position.longitude.deg = vdata[7];
					GP_helicopter.goto_position.longitude.min = vdata[8];
					GP_helicopter.goto_position.longitude.min_frac = vdata[9] * 256 + vdata[10];
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
							//GP_TX_packet(GP_unk_AA, strlen(GP_unk_AA));
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
					GP_TX_packet(GP_handshake, 9);
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
}


void GP_TX_GeneralPurposePacket( unsigned char* data, unsigned char len)
{
	unsigned char packet[MAXPACKLEN] = "";
	unsigned char cnt = 0;
	unsigned short chksum = 0;
	
	packet[0] = 0xa5;
	packet[1] = 0x5a;
	packet[2] = len + 2;
	packet[3] = 0x54;
	packet[4] = 0xDD;	
	
	for (cnt = 5; cnt <= len + 4; cnt++)
	{
		packet[cnt] = data[cnt - 5];
	}
	
	for (cnt = 2; cnt <= len + 4; cnt++)
	{
		chksum += packet[cnt];	
	}
	
	packet[len+5] = (chksum & 0xFF00) >> 8;
	packet[len+6] = chksum & 0x00FF;
	
	packet[len+7] = 0xCC;
	packet[len+8] = 0x33;
	
	GP_TX_packet(packet, len + 9);
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
			packet[7] = (GP_helicopter.position.latitude.min_frac & 0xFF00) >> 8;
			packet[8] = (GP_helicopter.position.latitude.min_frac & 0x00FF);
			packet[9] = GP_helicopter.position.latitude.hemi;
			
			packet[10] = GP_helicopter.position.longitude.deg;
			packet[11] = GP_helicopter.position.longitude.min;
			packet[12] = (GP_helicopter.position.longitude.min_frac & 0xFF00) >> 8;
			packet[13] = (GP_helicopter.position.longitude.min_frac & 0x00FF);
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
//			packet[8] = (GP_helicopter.hsa.altitude & 0xff00) >> 8;
//			packet[9] = GP_helicopter.hsa.altitude & 0x00ff;
			
			packet[8] = GSPI_AcousticData[0];
			packet[9] = GSPI_AcousticData[1];
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
			
			packet[5] = (char)((GP_helicopter.attitude.roll & 0xff00) >> 8);
			packet[6] = (char)(GP_helicopter.attitude.roll & 0x00ff);
			packet[7] = (char)((GP_helicopter.attitude.pitch & 0xff00) >> 8);
			packet[8] = (char)(GP_helicopter.attitude.pitch & 0x00ff);
			
		//	packet[5] = GSPI_AccData[0];
		//	packet[6] = GSPI_AccData[1];
			
		//	packet[7] = GSPI_AccData[2];
		//	packet[8] = GSPI_AccData[3];
			
			packet[9] = (char)((GP_helicopter.attitude.yaw & 0xff00) >> 8);   //GSPI_CompData[0];
			packet[10] = (GP_helicopter.attitude.yaw & 0x00ff);             //GSPI_CompData[1];
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
		
		case 0x50:		// Pre-flight packet
		{
			packet[2] = 21;
			
			packet[5] = GP_helicopter.position.latitude.deg;
			packet[6] = GP_helicopter.position.latitude.min;
			packet[7] = (GP_helicopter.position.latitude.min_frac & 0xFF00) >> 8;
			packet[8] = (GP_helicopter.position.latitude.min_frac & 0x00FF);
			packet[9] = GP_helicopter.position.latitude.hemi;
			
			packet[10] = GP_helicopter.position.longitude.deg;
			packet[11] = GP_helicopter.position.longitude.min;
			packet[12] = (GP_helicopter.position.longitude.min_frac & 0xFF00) >> 8;
			packet[13] = (GP_helicopter.position.longitude.min_frac & 0x00FF);
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
void GP_ACK( char vdata[], char len )
{
	unsigned char ack[MAXPACKLEN] = "";
	unsigned char lcv = 0;
	unsigned short chksum = 0;
	
	//LATBbits.LATB4 ^= 1;	// DEBUG
	// packet header:
	ack[0] = 0xA5;
	ack[1] = 0x5A;
	ack[2] = len + 1;
	chksum = ack[2];
	ack[3] = 0x06;
	
	// copy the data into the data portion of the ACK packet
	for (lcv = 4; lcv < len + 4; lcv++)
	{
		ack[lcv] = vdata[lcv - 4];
		chksum += ack[lcv];
	}
	
	// checksum:
	ack[lcv] = (chksum & 0xFF00) >> 8;
	ack[lcv + 1] = (chksum & 0x00FF);
	
	// packet EOT
	ack[lcv + 2] = 0xCC;
	ack[lcv + 3] = 0x33;
	
	GP_TX_packet(ack, len + 8);
	//LATBbits.LATB4 ^= 1; // DEBUG
}


// ---------------------------------------------------------------------------
// Accessor Functions

void GP_init_chopper( void )
{
	// Destination Position:
	GP_helicopter.goto_position.latitude.deg = 48;
	GP_helicopter.goto_position.latitude.min = 30;
	GP_helicopter.goto_position.latitude.min_frac = 0;
	GP_helicopter.goto_position.latitude.hemi = 0x4E;
	
	GP_helicopter.goto_position.longitude.deg = 123;
	GP_helicopter.goto_position.longitude.min = 30;
	GP_helicopter.goto_position.longitude.min_frac = 0;
	GP_helicopter.goto_position.longitude.hemi = 0x57;
	
	// Current Position:
	GP_helicopter.position.latitude.deg = 48;
	GP_helicopter.position.latitude.min = 30;
	GP_helicopter.position.latitude.min_frac = 0;
	GP_helicopter.position.latitude.hemi = 0x4E;
	
	GP_helicopter.position.longitude.deg = 123;
	GP_helicopter.position.longitude.min = 30;
	GP_helicopter.position.longitude.min_frac = 0;
	GP_helicopter.position.longitude.hemi = 0x57;
	
	// HSA:
	GP_helicopter.hsa.heading = 0;
	GP_helicopter.hsa.speed = 0;
	GP_helicopter.hsa.altitude = 0;
	
	// Attitude:
	GP_helicopter.attitude.pitch = 0;		// Accelerometer data[0]
	GP_helicopter.attitude.roll = 0;		// Accelerometer data[1]
	GP_helicopter.attitude.yaw = 0;			// Compass
	
	// Power Status:
	GP_helicopter.batterystatus.voltage = 100;
	GP_helicopter.batterystatus.current = 0;
	GP_helicopter.batterystatus.temp = 20;
	
	// Sensors:
	GP_helicopter.GPS_alt = 0;
	GP_helicopter.SON_alt = 0;
	GP_helicopter.sensors = 0xFF;
	
	// PWMs
	GP_helicopter.pwm.pitch = 50;
	GP_helicopter.pwm.roll = 50;
	GP_helicopter.pwm.yaw = 50;
	GP_helicopter.pwm.coll = 50;
	GP_helicopter.pwm.engRPM = 0;
}

void set_PRY(short pitch, short roll, short yaw)
{

		GP_helicopter.attitude.pitch = pitch;
		GP_helicopter.attitude.roll = roll;
		GP_helicopter.attitude.yaw =yaw;
}

void set_GPSlat(char deg, char min, short min_frac)
{
		GP_helicopter.position.latitude.deg = deg;
		GP_helicopter.position.latitude.min = min;
		GP_helicopter.position.latitude.min_frac = min_frac;
//		corr_GPSlatlongalt(latcorr, longcorr, altcorr, seconds);
}

void set_GPSlong(char deg, char min, short min_frac)
{
		GP_helicopter.position.longitude.deg = deg;
		GP_helicopter.position.longitude.min = min;
		GP_helicopter.position.longitude.min_frac = min_frac;
//		corr_GPSlatlongalt(latcorr, longcorr, altcorr, seconds);
}

void set_Gyros(signed short pitch, signed short roll, signed short yaw )
{
		GP_helicopter.gyros.pitch = pitch;
		GP_helicopter.gyros.roll = roll;
		GP_helicopter.gyros.yaw = yaw;
}

void corr_GPSlatlongalt( signed short latc, signed short longc, signed short altc, unsigned short seconds)
{
	signed long templat = GP_helicopter.position.latitude.min_frac + latc ;
	signed long templong = GP_helicopter.position.longitude.min_frac + longc;
	
	if (templat > 9999)
	{
		GP_helicopter.position.latitude.min++;
		GP_helicopter.position.latitude.min_frac = (short)(templat - 10000);
	} 
	else if (templat < 0)
	{
		GP_helicopter.position.latitude.min--;
		GP_helicopter.position.latitude.min_frac = (short)(templat + 10000);
	} 
	else
	{
		GP_helicopter.position.latitude.min_frac = (short) templat;	
	}

	if (templong > 9999)
	{
		GP_helicopter.position.longitude.min++;
		GP_helicopter.position.longitude.min_frac = (short)(templong - 10000);
	} 
	else if (templat < 0)
	{
		GP_helicopter.position.longitude.min--;
		GP_helicopter.position.longitude.min_frac = (short)(templong + 10000);
	}	
	else
	{
		GP_helicopter.position.longitude.min_frac = (short) templong;	
	}
	GP_helicopter.GPS_alt += altc;
	
//	GP_helicopter.position.latitude.min_frac = 9999;//(short) templat;
}
