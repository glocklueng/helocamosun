#define MAXPACKLEN 	256		// maximum packet length

void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);
// The UART1 RX ISR. Fires when a character is received on UART1

void GP_init_UART( unsigned int );
// GP_init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
// at the baud rate passed to 'baud'

void GP_TX_char ( char );
// transmit 1 byte on the open UART port 

void GP_TX_packet ( char *packet, unsigned short len );
// transmit a packet of length 'len'

void GP_state_machine ( void );
// advances the state machine through a received packet

void GP_parse_data ( char vdata[MAXPACKLEN], char len );
// parses the data portion of a valid packet

void GP_TX_telemetry( unsigned char type );
// transmits a telemetry packet IAW "type"

void GP_ACK( char vdata[MAXPACKLEN], char len );
// builds an acknowledgement packet based on the contents of data

void GP_init_chopper( void );
// initializes the helicopter variables

char *GP_itoa(char *buffer, int i);
// converts an integer into a string



typedef struct
{
	char deg;
	char min;
	short sec;
	char hemi;
} GPT_latlong;

typedef struct
{
     GPT_latlong latitude;
     GPT_latlong longitude;
     char action;
     short data;	// either an altitude to hover at, or a search pattern to conduct
} GPT_goto_position;

typedef struct
{
	unsigned short heading;
	unsigned char speed;
	unsigned short altitude;	
} GPT_HSA;

typedef struct
{
	signed short pitch, roll, yaw;	
} GPT_attitude;

typedef struct
{
	signed short voltage, current, temp;	
} GPT_batterystatus;

typedef struct
{
	GPT_latlong latitude, longitude;	
} GPT_position;

typedef struct
{
	unsigned char pitch, roll, yaw, coll, engRPM;	
} GPT_pwm;

typedef struct
{
	GPT_goto_position goto_position;
	GPT_HSA hsa;
	GPT_attitude attitude;
	GPT_batterystatus batterystatus;
	GPT_position position;	
	
	unsigned short GPS_alt, SON_alt;
	unsigned char sensors;
	GPT_pwm pwm;
	
} GPT_helicopter;

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
//--------------------------- Messages --------------------------------
extern char GP_err_chksum[];
extern char GP_err_SOT[];
extern char GP_err_EOT[];
extern char GP_it_works[];
extern char GP_debug_dv[];
extern char GP_handshake[];

//---------------------------------------------------------------------------
