#include "GPS.h"

CONTROL GPSflag;

char GPS_TIME[6];
char GPS_LATITUDE[9];
char GPS_LONGITUDE[10];
char GPS_SATELLITES[2];
char GPS_ALTITUDE[7];

void GetGPSString(char GPSdata)
{
	static char GPS_STATE = 0;
	static char GPS_STRING = 0;
	switch(GPSdata)
	{
		case '$':
		case 'G':
		case 'P':
			GPS_STATE++;
			GPSflag.rxdata = 0;
			break;
		case 'A':
			if(	GPS_STATE >= 5)
			{
				GPSflag.rxdata = 1;
			}
			GPS_STATE = 0;
			GPS_STRING = 0;
			break;
		case ',':
			GPS_STATE++;
			GPS_STRING = 0;
			break;
		case '*':
			GPS_STATE = 0;
			GPS_STRING = 0;
			GPSflag.rxdata = 0;
			break;
		default:
			if(GPSflag.rxdata )
			{
				switch(GPS_STATE)
				{
					case GPSTIME:
						GPS_TIME[GPS_STRING++] = GPSdata;
						break;
					case GPSLATITUDE:
						GPS_LATITUDE[GPS_STRING++] = GPSdata;
						break;
					case GPSLONGITUDE:
						GPS_LONGITUDE[GPS_STRING++] = GPSdata;
						break;
					case GPSSATELLITES:
						GPS_SATELLITES[GPS_STRING++] = GPSdata;
						break;
					case GPSALTITUDE:
						GPS_ALTITUDE[GPS_STRING++] = GPSdata;
						break;
					default:
						break;
				}	
			}
			break;
	}
}
