#include "typedefs.h"

#define MAXPACKLEN 32		// maximum packet length

#define ERR_COMPASS_CONN		0x01;
#define ERR_2GYRO_CONN			0x02;
#define	ERR_YGYRO_CONN			0x03;
#define ERR_ACC_CONN			0x04;
#define ERR_GPS_CONN			0x05;
#define ERR_MCU_CONN			0x06;


// -------------------- Transmitter Functions --------------------------
void GP_TX_char ( char );
// transmit 1 byte on the open UART port 

void GP_TX_packet ( unsigned char packet[], unsigned short len );
// transmit a packet of length 'len'

void GP_TX_telemetry( unsigned char type );
// transmits a telemetry packet IAW "type"

void GP_ACK( char vdata[], char len );
// builds an acknowledgement packet based on the contents of data

void GP_TX_error ( char code );
// build and transmit an error packet;


// ------------------- State Machine Functions -------------------------------
void GP_state_machine ( void );
// advances the state machine through a received packet

void GP_parse_data ( char vdata[], char len );
// parses the data portion of a valid packet


// ------------------ General Functions -------------------------------------
void GP_init_chopper( void );
// initializes the helicopter variables

char *GP_itoa(char *buffer, int i);
// converts an integer into a string



//-------------------------- Global Variables -------------------------------
extern unsigned char GP_bytercvd;  	// 0 = no byte in buffer, 1 = byte in buffer
extern unsigned char GP_datavalid;	// 0 = no valid data ready, 1 = valid data ready
extern char GP_data[];
extern char GP_data_len;
extern unsigned char GP_engRPMsp[];	// Set Points. 0 = Zero, 1 = 50%, 2 = 100%
extern unsigned char GP_pitchsp[];
extern unsigned char GP_rollsp[];
extern unsigned char GP_yawsp[];
extern unsigned char GP_collsp[];
extern char GP_errorSOT;
extern char GP_errorEOT;
extern unsigned char GP_dump;
extern unsigned char GP_hs;
extern unsigned char GP_engON;
extern GPT_helicopter GP_helicopter; // global helicopter structure
//--------------------------- Messages --------------------------------
extern char GP_err_chksum[];
extern char GP_err_SOT[];
extern char GP_err_EOT[];
extern char GP_it_works[];
extern char GP_debug_dv[];
extern unsigned char GP_handshake[];

extern char error[];

extern char modeFuzzy;
//---------------------------------------------------------------------

// Accessor functions
void set_variables(short pitch, short roll, short yaw);
void set_GPSlat(char deg, char min, short min_frac);
void set_GPSlong(char deg, char min, short min_frac);
void set_Gyros(signed short pitch, signed short roll, signed short yaw );
void set_Altitude ( unsigned short alt );
void set_PRY(short pitch, short roll, short yaw);

void corr_GPSlatlongalt( signed short latc, signed short longc, signed short altc, unsigned short seconds);
// adjusts the saved GPS coordinates by the transmitted correction factor
// <<< unimplemented ATT  >>>
