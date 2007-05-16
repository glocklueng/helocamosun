// USART.h

/*
Function: SerialInit
Description: Sets up serial communications for 115200 baud, 8 N 1
*/
void SerialInit(void);

#define DataRdyUSART( ) (PIR1bits.RCIF)
#define getcUSART ReadUSART
#define BusyUSART( ) (!TXSTAbits.TRMT)
#define putcUSART WriteUSART
