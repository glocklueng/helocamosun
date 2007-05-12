/*
	Title: 	PIC18F4431 SPI.c

	Author:	Kyle Wong

	Date:	15/04/07
	
	Description:	This program will initiate communications with an external EEPROM via
					the SPI port.

	Notes:	

	Compiled with MCC18 compiler (v8.01PL3)
	and MPLAB ver 7.5
*/
/************************** Include Section **************************/
#include <p18f4431.h>


/*************************** Program Control *************************/
#define LCD_DISPLAY 	0

/*************************** Defines *********************************/
#define TAA_CHANNEL0	0x03		// X-Axis
#define TAA_CHANNEL1	0x0B		// Z-Axis
#define TAA_CHANNEL2	0x13		// Y-Axis
#define DIO_SEND	LATBbits.LATB7
#define DIO_GET		PORTBbits.RB7
#define CLK			LATBbits.LATB6
#define TAA			LATBbits.LATB5
#define DIO_DIR		TRISBbits.TRISB7
#define CLK_DIR		TRISBbits.TRISB6
#define TAA_DIR		TRISBbits.TRISB5	// Tri-Axis Accellerometer
#define	DEBUG_DELAY	200

/*************************** Definitions *****************************/
typedef union
{
	char byte[2];
	int D_byte;
	long D_word;
}WORD;

/*************************** Global Variables ************************/
WORD Capture;

/*************************** Initialization Functions ****************/
void OpenLCD(void);
void SerialInit(void);
char ShiftIO(WORD byte_out, char out_bits, WORD *byte_in, char in_bits);

/*************************** Function Definitions ********************/


/*************************** Interrupt Section ***********************/
void CCPINT(void);
#pragma interrupt INTERRUPT
void INTERRUPT(void)
{
	if(PIR1bits.CCP1IF)
	{
		CCPINT();
	}
}

void CCPINT(void)
{
	PIR1bits.CCP1IF = 0; // clear the capture flag
	Capture.byte[0] = TMR1L;
	TMR1L = 0;	// Clear the timers
	Capture.byte[1] = TMR1H;
	TMR1H = 0;	// Clear the timers
}

#pragma code high_int_vector = 0x08
void interrupt_at_high_vector(void)
{
	_asm GOTO INTERRUPT _endasm
}
#pragma code /* return to default code section */

void main(void)
{
	char temp;
	TRISBbits.TRISB7 = 0;
	TRISDbits.TRISD1 = 0;	// SDO
	TRISDbits.TRISD2 = 1;	// SDI
	TRISDbits.TRISD3 = 0;	// SCK
	
	SSPSTATbits.SMP = 1;
	SSPSTATbits.CKE = 0;
	
	SSPCONbits.CKP = 0;
	SSPCONbits.SSPM3 = 0;	// Fosc/64
	SSPCONbits.SSPM2 = 0;
	SSPCONbits.SSPM1 = 1;
	SSPCONbits.SSPM0 = 0;
	
	SSPCONbits.SSPEN = 1;
	SerialInit();
	do
	{
		SSPBUF = 0x55;
		while(!PIR1bits.SSPIF);
		temp = SSPBUF;
		PIR1bits.SSPIF = 0;
		TXREG = temp;
		while(!TXSTAbits.TRMT);		
	}
	while(1);
}
/*
Function: SerialInit
Description: Sets up serial communications for 115200 baud, 8 N 1
*/
void SerialInit(void)
{
	TRISCbits.TRISC6 = 1;	// Set the used pins as inputs
	TRISCbits.TRISC7 = 1;	// USART will handle changing them from input to output
	
	TXSTAbits.TX9 = 0;		// 8 bit mode
	TXSTAbits.TXEN = 1;		// Transmit enabled
	TXSTAbits.SYNC = 0;		// Asynchoronous mode
//	TXSTAbits.TRMT = 1;		// TSR empty
	TXSTAbits.BRGH = 1;		// High speed
		
		// Receive Status and Control
	RCSTAbits.SPEN = 1; 	// Serial port enabled
	RCSTAbits.RX9 = 0;		// 8 bit mode
	RCSTAbits.CREN = 1;		// Continuous receive
	
//	SPBRG = 0x40;			// 19 200 KBaud @ 20 MHz
	SPBRG = 0x0A;			// 115 200 KBaud @ 20 MHz
	
		// Baud Rate control
//	BAUDCTLbits.BRG16 = 1;
	BAUDCTLbits.WUE = 0;
	BAUDCTLbits.ABDEN = 0;
	
//	PIE1bits.RCIE = 1;		// enable receiver interrupt
//	IPR1bits.RCIP = 0;		// High priority interrupt
// 	if(PIR1bits.RCIF)
//	{
//		data = RCREG;// used to receive characters RCIF is cleared on read
//	}

}
