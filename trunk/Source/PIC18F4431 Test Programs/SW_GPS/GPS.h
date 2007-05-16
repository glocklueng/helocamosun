// GPS.h
#include <p18f4431.h>

#define GPSTIME		  0x01
#define GPSLATITUDE	  0x02
#define GPSLONGITUDE  0x04
#define GPSSATELLITES 0x07
#define GPSALTITUDE   0x09

typedef struct
{
	unsigned char rxBuf:1;
	unsigned char rxGPSdata:1;
	unsigned char GPSDataReady:1;
	unsigned char debug:1;
	unsigned char bist:1;
	unsigned char startup:1;
	unsigned char main:1;
}CONTROL;

extern CONTROL cFlag;

extern char GPS_TIME[6];
extern char GPS_LATITUDE[9];
extern char GPS_LONGITUDE[10];
extern char GPS_SATELLITES[2];
extern char GPS_ALTITUDE[6];

void GetGPSString(char GPSdata);
