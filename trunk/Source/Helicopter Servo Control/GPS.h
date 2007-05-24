// GPS.h
#include <p18f4431.h>

#define GPSTIME		  0x01
#define GPSLATITUDE	  0x02
#define GPS_N_S		  0x03
#define GPSLONGITUDE  0x04
#define GPS_W_E		  0x05
#define GPSSATELLITES 0x07
#define GPSALTITUDE   0x09
#define GPS_SPEED 	  0x06

typedef struct
{
	unsigned char speed:1;
	unsigned char rxdata:1;
	unsigned char DataReady:1;
}CONTROL;

extern CONTROL GPSflag;

extern char GPS_TIME[6];
extern char GPS_LATITUDE[9];
extern char GPS_LONGITUDE[10];
extern char GPSSPEED[5];
//extern char GPS_HEMI[2];
extern char GPS_SATELLITES[2];
extern char GPS_ALTITUDE[7];


void GetGPSString(char GPSdata);
