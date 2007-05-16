// USART.h

//#define USART_DEBUG

/*
Function: SerialInit
Description: Sets up serial communications for 115200 baud, 8 N 1
*/
void SerialInit(void);

#ifdef USART_DEBUG

void ClearScreen(void);
void putsUSART (char *data);
void WriteUSART (char data);
char ReadUSART (void);
void prepscreen(void);
void UpdateAccelerometer(void);
void UpdateCompass(void);
void UpdateServos(void);
void UpdateADC(void);
void setCursor( char x, char y);

#endif

#define DataRdyUSART( ) (PIR1bits.RCIF)
#define getcUSART ReadUSART
#define BusyUSART( ) (!TXSTAbits.TRMT)
#define putcUSART WriteUSART


