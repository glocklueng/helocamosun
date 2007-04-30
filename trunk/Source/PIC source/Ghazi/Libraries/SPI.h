#include "defines.h"

void SPI_init ( void );
// Initialize the SPI module

void SPI_tx_byte ( char );
// transmit 1 byte on the SPI bus

void SPI_tx_command ( char[MAXPACKLEN], char len );
//transmit a string of character on the SPI bus, expect no response

void SPI_tx_req ( char[MAXPACKLEN], char data[MAXPACKLEN] );
//transmit a request for info on the SPI bus, record the response in "data"

void SPI_readYawGyro ( void );
// reads the Yaw gyro


// Info Requests
extern char GSPI_packet[MAXPACKLEN]; 
extern char GSPI_test[];

extern char GSPI_AccReq[];
extern char GSPI_AccData[6];

extern char GSPI_2GyroReq[];
extern char GSPI_2GyroData[4];
	
extern char GSPI_YawGyroData[2];

extern char GSPI_CompReq[];
extern char GSPI_CompData[2];

extern char GSPI_AcousticReq[];
extern char GSPI_AcousticData[2];

extern char GSPI_VoltReq[];
extern char GSPI_VoltData[4];

extern char GSPI_TempReq[];
extern char GSPI_TempData[2];

extern char GSPI_RpmReq[];
extern char GSPI_RpmData[2];

extern char GSPI_StatusReq[];
extern char GSPI_StatusData[2];


