#include <p30fxxxx.h>
#include <string.h>
#include "../Libraries/RFprotocol.h"
#include "../Libraries/SPI.h"

//#define FCY			1842500 // Instruction cycle freq = xtal / 4

void setupTRIS ( void );

void init_GVars ( void );

void init_T1 ( void );

void init_LEDs ( void );

void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);
// The UART1 RX ISR. Fires when a character is received on UART1

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void);
// Hopefully the TIMER1 UART

void fillpwmCommand ( void );

/*unsigned char GP_engRPM = 0;
unsigned char GP_pitch = 0;
unsigned char GP_roll = 0;
unsigned char GP_yaw = 0;
unsigned char GP_coll = 0;
unsigned char GP_engRPMsp[3] = "";	// Set Points. 0 = Zero, 1 = 50%, 2 = 100%
unsigned char GP_pitchsp[3] = "";
unsigned char GP_rollsp[3] = "";
unsigned char GP_yawsp[3] = "";
unsigned char GP_collsp[3] = "";*/

char pwmCommand[] = { 'P', 0x01, 0x01, 0x01, 0x01 };

extern unsigned char newPWM;

//*******************************   MAIN   **********************************
int main ( void )
{
	unsigned char dummy;
	long i = 0;
	
	setupTRIS();
	init_GVars();
	init_T1();
	init_LEDs();
	
	
	SPI_init();	
	GP_init_UART(19200);
	
	GP_init_chopper();
	
	while(1)
	{
		GP_state_machine();
		if (GP_datavalid)
		{
			GP_datavalid = 0;
			GP_parse_data(GP_data, GP_data_len);
		}
		
		if (newPWM)
		{
			newPWM = 0;
			fillpwmCommand();
			SPI_tx_command(pwmCommand, 5);	
		}
	}
	return 0;
}
//***************************************************************************
void fillpwmCommand ( void )
{
	pwmCommand[1] = GP_pitch;
	pwmCommand[2] = GP_roll;
	pwmCommand[3] = GP_yaw;
	pwmCommand[4] = GP_coll;
}

void setupTRIS ( void )
{	
	TRISAbits.TRISA12 = 1;
	TRISD = 0x00;
	TRISFbits.TRISF3 = 0;
	TRISFbits.TRISF6 = 0;	// SCLK = RF6
	TRISFbits.TRISF7 = 1;	// SDI = RF7
	TRISFbits.TRISF8 = 0;	// SDO = RF8
	TRISB = 0;
}

void init_GVars ( void )
{
	GP_pitch = 0;
	GP_roll = 0;
	GP_yaw = 0;
	GP_coll = 0;
	GP_engRPM = 0;
	GP_errorSOT = 0;
	GP_errorEOT = 0;
	GP_hs = 0;	
}

void init_T1 ( void )
{
	PR1 = 65535;				// 50us
	IEC0bits.T1IE = 1;
	T1CONbits.TCS = 0;   //1.8425 MHz = 542.7 ns / tick
	T1CONbits.TCKPS = 0b11;
	TMR1 = 0;
	T1CONbits.TON = 0;	
}

void init_LEDs ( void )
{
	// Turn off the LEDs on the dsPICDEM board
	LATDbits.LATD0 = 1;
	LATDbits.LATD1 = 1;
	LATDbits.LATD2 = 1;
	LATDbits.LATD3 = 1;	
}

void __attribute__(( interrupt, no_auto_psv )) _U1RXInterrupt(void)
{
	IEC0bits.U1RXIE = 0;	// disable the receive interrupt
	IFS0bits.U1RXIF = 0;	// clear the receive interrupt flag
	GP_bytercvd = 1;		// indicate a byte was received
	GP_dump = U1RXREG;		// read the byte from the receive register
	IEC0bits.U1RXIE = 1;	// re-enable the receive interrupt
	
	//LATDbits.LATD0 ^= 1;
}

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;
	if (GP_hs)
	{
		// have to use time slicing, and only send this at intervals
		GP_TX_packet(GP_handshake, 9);	
	}
}
