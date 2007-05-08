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
char pwmCommand[] = { 'P', 0x01, 0x01, 0x01, 0x01 };

extern unsigned char newPWM;
extern char GSPI_AccData[];
//*******************************   MAIN   **********************************
int main ( void )
{
	unsigned char dummy;
	long i = 0, q = 0;
	
	setupTRIS();
	init_GVars();
	init_T1();
	init_LEDs();
	TRISBbits.TRISB0 = 0;
	LATBbits.LATB0 = 1;
	
	SPI_init();	
	GP_init_UART(19200);
	
	GP_init_chopper();
	q = 0;

	GSPI_CompData[0] = 0;
	while(1)
	{
		q++;
		
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
		
		//if(!PORTAbits.RA12)
		if (q > 100000)
		{
			//while(!PORTAbits.RA12);
			q = 0;
			
			//SPI_tx_req(	GSPI_AccReq, GSPI_AccData );
			//GP_TX_packet(GSPI_AccData, 6); // DEBUG
			GP_helicopter.attitude.pitch = GSPI_AccData[0] * 256 + GSPI_AccData[1];
			GP_helicopter.attitude.roll = GSPI_AccData[2] * 256 + GSPI_AccData[3];
			//GP_TX_char('\n');
			
			for (i = 0; i < 10000; i++);
			
			//SPI_tx_req(	GSPI_CompReq, GSPI_CompData );
			//GP_TX_packet(GSPI_CompData, 2);
			//GP_TX_char('\n');
			
			for (i = 0; i < 10000; i++);
			
			//SPI_tx_req(	GSPI_AcousticReq, GSPI_AcousticData );
			//GP_TX_packet(GSPI_AcousticData, 2);
			//GP_TX_char('\n');
		}
		
		if(!PORTAbits.RA12)
		{
			while(!PORTAbits.RA12);
			SPI_readYawGyro();
		}

		
		
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
	GP_helicopter.pwm.pitch = 0;
	GP_helicopter.pwm.roll = 0;
	GP_helicopter.pwm.yaw = 0;
	GP_helicopter.pwm.coll = 0;
	GP_helicopter.pwm.engRPM = 0;
	GP_errorSOT = 0;
	GP_errorEOT = 0;
	GP_hs = 0;	
}

void init_T1 ( void )
{
	PR1 = 65535;		 		// 35.6ms
	IEC0bits.T1IE = 1;
	T1CONbits.TCS = 0;   		//1.8425 MHz = 542.7 ns / tick
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
}

void __attribute__(( interrupt, no_auto_psv )) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;
	if (GP_hs)						
	{
		// When the timer expires and a handshake terminate has not been 
		// received we need to ACK the handshake
		GP_TX_packet(GP_handshake, 9);	
	}
}

void Init_PWM( void )
{
	//PTCONbits.PTEN = 0;
		
}
