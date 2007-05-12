#include <p30fxxxx.h>
#include <string.h>

#include "typedefs.h"
#include "RF.h"
#include "SPI.h"

#include "commands.h"
#include "DataEEPROM.h"

#define FCY			5000000 // Instruction cycle freq = xtal / 4

void setupTRIS ( void );

void init_GVars ( void );

void init_T1 ( void );

void init_T2 ( void );

void init_LEDs ( void );

void GP_init_UART( unsigned int baud );

void Init_PWM( void );
// initialize the PWM module

void __attribute__ (( interrupt, no_auto_psv )) _U1RXInterrupt(void);
// The UART1 RX ISR. Fires when a character is received on UART1

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void);
// Timer1 ISR

void fillpwmCommand ( void );

GPT_helicopter GP_helicopter;
/*
GP_helicopter.position.latitude.deg 		(1)
GP_helicopter.position.latitude.min 		(1)
GP_helicopter.position.latitude.sec 		(2)

GP_helicopter.position.longitude.deg 		(1)
GP_helicopter.position.longitude.min 		(1)
GP_helicopter.position.longitude.sec 		(2)

GP_helicopter.goto_position.latitude.deg 	(1)
GP_helicopter.goto_position.latitude.min 	(1)
GP_helicopter.goto_position.latitude.sec 	(2)

GP_helicopter.goto_position.longitude.deg 	(1)
GP_helicopter.goto_position.longitude.min 	(1)
GP_helicopter.goto_position.longitude.sec 	(2)

GP_helicopter.hsa.heading 					(2)
GP_helicopter.hsa.speed 					(2)
GP_helicopter.hsa.altitude 					(2)

GP_helicopter.attitude.pitch				(2)
GP_helicopter.attitude.roll					(2)
GP_helicopter.attitude.yaw					(2)

GP_helicopter.batterystatus.voltage			(2)
GP_helicopter.batterystatus.current			(2)
GP_helicopter.batterystatus.temp			(2)
*/

//char pwmCommand[] = { 'P', 0x01, 0x01, 0x01, 0x01 };

extern unsigned char newPWM;
extern unsigned char GSPI_AccData[];
//extern unsigned char GP_hs;
unsigned char T2flag;
unsigned char clock;
//*******************************   MAIN   **********************************
int main ( void )
{
	unsigned char dummy, error;
	long i = 0;
	unsigned char state4_cnt = 0;
	
	int writeAddr;
	
	setupTRIS();
	LATBbits.LATB4 = 1;
	GP_init_UART(19200);

	
	init_GVars();
	init_T1();
	init_T2();
	Init_PWM();
	
	LATBbits.LATB0 = 1;
	
	SPI_init();	
	
	
	
	
	GP_init_chopper();
	GSPI_CompData[0] = 0;
	GSPI_CompData[0] = 0;
	//GP_hs = 1;
	LATBbits.LATB4 = 0;
	while(1)
	{

		if (GP_datavalid)
		{
			GP_datavalid = 0;
			GP_parse_data(GP_data, GP_data_len);
		}
//		
//		if (newPWM)
//		{
//			newPWM = 0;
//			fillpwmCommand();
//			SPI_tx_command(pwmCommand, 5);	
//		}
		
		//SPI_tx_req(	GSPI_AccReq, GSPI_AccData );
//		GP_helicopter.attitude.pitch = GSPI_AccData[0] * 256 + GSPI_AccData[1];
//		GP_helicopter.attitude.roll = GSPI_AccData[2] * 256 + GSPI_AccData[3];
//		
//		//SPI_tx_req(	GSPI_CompReq, GSPI_CompData );
//		GP_helicopter.hsa.heading = GSPI_CompData[0] * 256 + GSPI_CompData[1];
//		
		//SPI_tx_req(	GSPI_AcousticReq, GSPI_AcousticData );
	
		
	}
	return 0;
}
//***************************************************************************
void fillpwmCommand ( void )
{
	pwmCommand[1] = GP_helicopter.pwm.pitch;
	pwmCommand[2] = GP_helicopter.pwm.roll;
	pwmCommand[3] = GP_helicopter.pwm.yaw;
	pwmCommand[4] = GP_helicopter.pwm.coll;
}

void setupTRIS ( void )
{	
	TRISFbits.TRISF3 = 0;
	TRISFbits.TRISF6 = 0;	// SCLK = RF6
	TRISFbits.TRISF2 = 1;	// SDI = RF7
	TRISFbits.TRISF3 = 0;	// SDO = RF8
	TRISBbits.TRISB0 = 0;
	TRISBbits.TRISB4 = 0;
	TRISDbits.TRISD0 = 0;
	
	// SPI Slave Select lines:
	uCSSTris = 0;
	YawGyroSSTris = 0;
	eepromSSTris = 0;
	tempSSTris = 0;
	
	
}

void init_GVars ( void )
{
	GP_helicopter.pwm.pitch = 0;
	GP_helicopter.pwm.roll = 0;
	GP_helicopter.pwm.yaw = 0;
	GP_helicopter.pwm.coll = 0;
	GP_helicopter.pwm.engRPM = 0;
	//GP_errorSOT = 0;
	//GP_errorEOT = 0;
	GP_hs = 0;	
}

void init_T1 ( void )
{
	PR1 = 4096;		 		// 35.6ms
	IEC0bits.T1IE = 1;
	T1CONbits.TCS = 0;   		//1.8425 MHz = 542.7 ns / tick
	T1CONbits.TCKPS = 0b11;		
	TMR1 = 0;
	T1CONbits.TON = 0;	
}

void init_T2 ( void )
{
	PR2 = 120;		 	
	IEC0bits.T2IE = 1;
	T2CONbits.TCS = 0;   		//1.8425 MHz = 542.7 ns / tick
	T2CONbits.TCKPS = 0b00;		
	TMR2 = 0;
	T2flag = 0;
	T2CONbits.TON = 1;	
}

void init_LEDs ( void )
{
	// Turn off the LEDs on the dsPICDEM board
	//LATDbits.LATD0 = 1;
	//LATDbits.LATD1 = 1;
	//LATDbits.LATD2 = 1;
	//LATDbits.LATD3 = 1;	
}

void __attribute__(( interrupt, no_auto_psv )) _U2RXInterrupt(void)
{
	IEC1bits.U2RXIE = 0;	// disable the receive interrupt
	IFS1bits.U2RXIF = 0;	// clear the receive interrupt flag
	GP_bytercvd = 1;		// indicate a byte was received
	GP_dump = U2RXREG;		// read the byte from the receive register
	GP_state_machine();
	IEC1bits.U2RXIE = 1;	// re-enable the receive interrupt
}

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;
	LATDbits.LATD0 ^= 1;	// Toggle a pin for debugging
	if (GP_hs)						
	{
		// When the timer expires and a handshake terminate has not been 
		// received we need to ACK the handshake
		GP_TX_packet(GP_handshake, 9);	
	}
}

void __attribute__(( interrupt, no_auto_psv )) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0;
	T2flag = 1;
	GP_state_machine();	// Run the byte received through the state machine.
	//clock++;
	TMR2 = 0;
}

void Init_PWM( void )
{
	PDC1 = 234;			// PWM Duty Cycle
	PTPERbits.PTPER = 2344;	// Timebase period

	PTCONbits.PTSIDL = 0;
	PTCONbits.PTOPS = 0;	// Postscale = 1:1
	PTCONbits.PTCKPS = 0b11;	// Prescale = 1:64
	PTCONbits.PTMOD = 0;	// Free-running mode
	
	PTMRbits.PTMR = 0;		// Timebase register
	
	PWMCON1bits.PMOD1 = 1;	// PMW1 is in independant mode
	
	PWMCON1bits.PEN1H = 1;	// Enable PWM1H
	PWMCON1bits.PEN2H = 0;	// Disable PWM2H
	PWMCON1bits.PEN3H = 0;	// Disable PWM3H

	PWMCON1bits.PEN1L = 0;	// Disable PWM1L
	PWMCON1bits.PEN2L = 0;	// Disable PWM2L
	PWMCON1bits.PEN3L = 0;	// Disable PWM3L
	
	PTCONbits.PTEN = 1;	// Enable PWM Time Base	
}

void GP_init_UART( unsigned int baud )
{
	// GP_init_UART initializes UART1 to 8 data bits, no parity, 1 stop bit
	// at the baud rate passed to 'baud'
	U2MODEbits.ALTIO = 0;		// use U1TX and U1RX as tx/rx pins
	U2MODEbits.LPBACK = 0;		// disable loopback mode
	U2MODEbits.PDSEL = 0b00;	// 8 data bits, no parity
	U2MODEbits.STSEL = 0;		// 1 stop bit

	//U2BRG = ( FCY / (16 * baud) ); // calculate the BRG value for a
									   // given baud rate
	//U2BRG = 0x000F;
	U2BRG = 0x0020;
	U2MODEbits.UARTEN = 1;		// enable the UART
	IEC1bits.U2RXIE = 1;		// enable the UART RX interrupt
}
