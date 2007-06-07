#include "typedefs.h"

#define MAXPACKLEN 32		// maximum packet length

#define ERR_COMPASS_CONN		0x01;
#define ERR_2GYRO_CONN			0x02;
#define	ERR_YGYRO_CONN			0x03;
#define ERR_ACC_CONN			0x04;
#define ERR_GPS_CONN			0x05;
#define ERR_MCU_CONN			0x06;

//void GP_init_UART( unsigned int );
// GP_init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'

void GP_TX_char ( char );
// transmit 1 byte on the open UART port 

void GP_TX_packet ( unsigned char packet[], unsigned short len );
// transmit a packet of length 'len'

void GP_state_machine ( void );
// advances the state machine through a received packet

void GP_parse_data ( char vdata[], char len );
// parses the data portion of a valid packet

void GP_TX_telemetry( unsigned char type );
// transmits a telemetry packet IAW "type"

void GP_ACK( char vdata[], char len );
// builds an acknowledgement packet based on the contents of data

void GP_init_chopper( void );
// initializes the helicopter variables

char *GP_itoa(char *buffer, int i);
// converts an integer into a string

void GP_TX_error ( char code );
// build and transmit an error packet;

void corr_GPSlatlongalt( signed short latc, signed short longc, signed short altc, unsigned short seconds);

//-------------------------- Global Variables -------------------------------
extern unsigned char GP_bytercvd;  	// 0 = no byte in buffer, 1 = byte in buffer
extern unsigned char GP_datavalid;	// 0 = no valid data ready, 1 = valid data ready
extern char GP_data[MAXPACKLEN];
extern char GP_data_len;
extern unsigned char GP_engRPMsp[3];	// Set Points. 0 = Zero, 1 = 50%, 2 = 100%
extern unsigned char GP_pitchsp[3];
extern unsigned char GP_rollsp[3];
extern unsigned char GP_yawsp[3];
extern unsigned char GP_collsp[3];
extern char GP_errorSOT;
extern char GP_errorEOT;
extern unsigned char GP_dump;
extern unsigned char GP_hs;
extern unsigned char GP_engON;
extern unsigned char newHeadingFlag;
extern GPT_helicopter GP_helicopter; // global helicopter structure
//--------------------------- Messages --------------------------------
extern char GP_err_chksum[];
extern char GP_err_SOT[];
extern char GP_err_EOT[];
extern char GP_it_works[];
extern char GP_debug_dv[];
extern unsigned char GP_handshake[];

extern char error[];

extern unsigned char ERCMDflag;

extern char modeFuzzy;
//---------------------------------------------------------------------

//added by scott
void set_variables(short pitch, short roll, short yaw);
void set_GPSlat(char deg, char min, short min_frac);
void set_GPSlong(char deg, char min, short min_frac);
void set_Gyros(signed short pitch, signed short roll, signed short yaw );
