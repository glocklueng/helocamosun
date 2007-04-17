/*
	Title: 	PIC18F4431 I2C.c

	Author:	Kyle Wong

	Date:	9/04/07
	
	Description:	This program will initiate communications with an external EEPROM via
					the I2C port.

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
	TRISCbits.TRISC4;
	TRISCbits.TRISC5;
	TRISDbits.TRISD2;
	TRISDbits.TRISD3;
	SSPCON = 0x3B;
	do
	{
		
		if(!SSPSTATbits.BF)
		{
			SSPSTATbits.D_A = 1;
			SSPBUF = 0x55;
		}
		while(SSPSTATbits.BF);
	}
	while(1);
}
