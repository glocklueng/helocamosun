#define MAXPACKLEN 32

// 4431 SPI Slave defines:
#define uCSS			LATEbits.LATE2
#define uCSSTris		TRISEbits.TRISE2

// EEPROM SPI Slave defines:
#define	eepromSS		LATEbits.LATE4
#define eepromSSTris	TRISEbits.TRISE4

// Temperature Sensor SPI Slave defines:
#define tempSS			LATEbits.LATE5
#define	tempSSTris		TRISEbits.TRISE5

void SPI_init ( char mode16, char falling );
// Initialize the SPI module

void SPI_tx_byte ( char );
// transmit 1 byte on the SPI bus

void SPI_tx_command ( unsigned char cmd[], char len );
//transmit a string of character on the SPI bus, expect no response

char SPI_tx_req ( unsigned char req[], unsigned char data[] );
//transmit a request for info on the SPI bus, record the response in "data"

int SPI_readTemp1 ( void );
// returns the temperature read from the temperature sensor

// These are the arrays that store data retrieved from the 4431
// over the SPI bus. 
extern unsigned char GSPI_AccData[6];
extern unsigned char GSPI_CompData[2];
extern unsigned char GSPI_AcousticData[2];
extern unsigned char GSPI_VoltData[4];
extern unsigned char GSPI_TempData[2];
extern unsigned char GSPI_RpmData[2];
extern unsigned char GSPI_StatusData[2];
extern unsigned char GSPI_TimeData[6];
extern unsigned char GSPI_LatData[9];
extern unsigned char GSPI_LongData[10];
extern unsigned char GSPI_AltData[7];
extern unsigned char GSPI_SatData[2];
extern unsigned char GSPI_3GyroData[6];




