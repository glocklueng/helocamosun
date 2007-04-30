// USART.h

union USART
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};

extern union USART USART_Status;

/*
Function: SerialInit
Description: Sets up serial communications for 115200 baud, 8 N 1
*/
void SerialInit(void);
void ClearScreen(void);
void putsUSART (char *data);
void WriteUSART (char data);
char ReadUSART (void);
void prepscreen(void);
void SendVariables(void);
void setCursor( char x, char y);
void convertstring(char *string, int value);

#define DataRdyUSART( ) (PIR1bits.RCIF)
#define getcUSART ReadUSART
#define BusyUSART( ) (!TXSTAbits.TRMT)
#define putcUSART WriteUSART
