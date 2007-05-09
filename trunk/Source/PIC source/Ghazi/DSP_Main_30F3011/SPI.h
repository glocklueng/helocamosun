
#define MAXPACKLEN 32

// 4431 SPI Slave defines:
#define uCSS			LATEbits.LATE2
#define uCSSTris		TRISEbits.TRISE2

// Yaw Gyro SPI Slave defines:
#define YawGyroSS		LATEbits.LATE3
#define YawGyroSSTris	TRISEbits.TRISE3

// EEPROM SPI Slave defines:
#define	eepromSS		LATEbits.LATE4
#define eepromSSTris	TRISEbits.TRISE4

// Temperature Sensor SPI Slave defines:
#define tempSS			LATEbits.LATE5
#define	tempSSTris		TRISEbits.TRISE5

void SPI_init ( void );
// Initialize the SPI module

void SPI_tx_byte ( char );
// transmit 1 byte on the SPI bus

void SPI_tx_command ( unsigned char[MAXPACKLEN], char len );
//transmit a string of character on the SPI bus, expect no response

void SPI_tx_req ( unsigned char[MAXPACKLEN], unsigned char data[MAXPACKLEN] );
//transmit a request for info on the SPI bus, record the response in "data"

void SPI_readYawGyro ( void );
// reads the Yaw gyro


// These are the arrays that store data retrieved from the 4431
// over the SPI bus. 
extern unsigned char GSPI_AccData[6];


extern unsigned char GSPI_CompData[2];
extern unsigned char GSPI_AcousticData[2];
extern unsigned char GSPI_VoltData[4];
extern unsigned char GSPI_TempData[2];
extern unsigned char GSPI_RpmData[2];
extern unsigned char GSPI_StatusData[2];

// These sensors are read directly into the DSP over the SPI bus

extern unsigned char GSPI_YawGyroData[2];
extern unsigned char GSPI_2GyroData[4];
