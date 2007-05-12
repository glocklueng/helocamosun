// Accelerometer.h

/* Define Section */
#define LCD_LINE1	0x80
#define LCD_LINE2	0xC0
#define LCD_LINE3	0x94
#define LCD_LINE4	0xD4

/*
Function: SerialInit
Description: Sets up serial communications for 115200 baud, 8 N 1
*/
void SerialInit(void)
{
	TXSTAbits.TX9 = 0;		// 8 bit mode
	TXSTAbits.TXEN = 1;		// Transmit enabled
	TXSTAbits.SYNC = 0;		// Asynchoronous mode
	TXSTAbits.TRMT = 1;		// TSR empty
		
		// Receive Status and Control
	RCSTAbits.SPEN = 1; 	// Serial port enabled
	RCSTAbits.RX9 = 0;		// 8 bit mode
	RCSTAbits.CREN = 1;		// Continuous receive
	TXSTAbits.BRGH = 1;		// High speed
//	SPBRG = 0x40;			// 19 200 KBaud @ 20 MHz
	SPBRG = 0x0A;			// 115 200 KBaud @ 20 MHz
	PIE1bits.RCIE = 1;		// enable receiver interrupt
	IPR1bits.RCIP = 0;		// High priority interrupt
//	Delay10TCYx(50); // the fastest ammount of time between transmissions of chars
// 	if(PIR1bits.RCIF)
//	{
//		data = RCREG;// used to receive characters RCIF is cleared on read
//	}

}

/*
Function: OpenLCD
Description: Opens communications with the LCD.
Conditions: RB5 must be connected.
			delays.h must be called
*/
/*
void OpenLCD(void)
{
	// OpenXLCD initiallzes the LCD
	OpenXLCD(FOUR_BIT & LINES_5X7 & SHIFT_CUR_RIGHT & DON & BLINK_OFF);
	while(BusyXLCD());     	// every command that is sent to the LCD must have 
	                       	// this statement
	WriteCmdXLCD(LCD_LINE1);// write over whatever is on line 1 of the LCD
	while(BusyXLCD());	
}

void DelayXLCD(void)
{
	char delay_loop;
	char delay;
	for(delay_loop = 100; delay_loop > 0; delay_loop--)
	{
		for(delay = 10; delay > 0; delay--)
		{
			Nop();
		}
	}
}

void DelayFor18TCY(void)
{
	char delay_loop;
	for(delay_loop = 1; delay_loop > 0; delay_loop--)
	{
		Nop();
	}
	Nop();
}

void DelayPORXLCD(void)
{
	char delay;
	char delay_loop;
	for(delay_loop = 46; delay_loop > 0; delay_loop--)
	{
		for(delay = 100; delay > 0; delay--)
		{
			Nop();
		}
	}
}
*/
