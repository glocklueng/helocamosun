#include <p18f4431.h>

void SPI_Init (void);
void SPI_Close(void);
unsigned char SPI_Write(unsigned char out);
unsigned char SPI_State_Machine(unsigned char Input);
unsigned char SPI_Read_Byte(void);
