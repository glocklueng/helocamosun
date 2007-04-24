#include <p18f4431.h>
#include <delays.h>
#include <xlcd.h>
#include <adc.h>
#include <stdlib.h>
#include <usart.h>

#include "SPIComms.h"

/////LOW PRIORITY SPI ISR CODE///////////////

void SPI_isr (void);
#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
	if(PIR1bits.SSPIF)
	{
		_asm GOTO SPI_isr _endasm
	}
}
#pragma code // return to the default code section 
#pragma interrupt SPI_isr

/////////////////////////////////////////////


////HIGH PRIORITY SPI ISR CODE////////////////
/*
void SPI_isr (void);
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
	if(PIR1bits.SSPIF)
	{
		_asm GOTO SPI_isr _endasm
	}
}
#pragma code // return to the default code section 
#pragma interrupt SPI_isr
*/
//////////////////////////////////////////////



void SPI_isr (void)
{
	unsigned char Incoming_Data;
	unsigned char Reply;

	Incoming_Data = SPI_Read_Byte();

	putcUSART(Incoming_Data);//debugging
	
	Reply = SPI_State_Machine(Incoming_Data);

	SPI_Write(Reply);//fill the buffer with data back to the SPI master, it will go out the next time an SPI packet is received

	putcUSART(Reply);//debugging

	PIR1bits.SSPIF = 0;//clear the interrupt flag
}




void main (void)
{	
	TRISB = 0x00;

	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;
 	SPI_Init();

	//open the USART at 
	OpenUSART ( USART_TX_INT_OFF &
				USART_RX_INT_ON &
				USART_ASYNCH_MODE &
				USART_EIGHT_BIT &
				USART_CONT_RX &
				USART_BRGH_HIGH, 10);//10 for 115200,21 for 57600
	IPR1bits.RCIP = 1;//set UART interrupt to high priority


	while(1)
	{			
		Delay10KTCYx(200);
		LATBbits.LATB7 ^= 1;
	}
}
