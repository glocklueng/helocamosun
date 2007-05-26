#include "GPS.h"

CONTROL GPSflag;

char GPS_TIME[6];
char GPS_LATITUDE[9];
char GPS_LONGITUDE[10];
//char GPS_HEMI[2];
char GPSSPEED[5];
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
		case 'C':
//			if( GPS_STATE >= 5)
//			{
//				GPSflag.speed = 1;
//			}
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
//			GPSflag.speed = 1;
			break;
//		case 'N':
//		case 'S':
//			if(GPSflag.rxdata)
//			{
//				GPS_STATE = GPS_N_S;
//			}
//			break;
//		case 'W':
//		case 'E':
//			if(GPSflag.rxdata || GPSflag.speed)
//			{
//				GPS_STATE = GPS_W_E;
//			}
//			break;
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
//					case GPS_N_S:
//						GPS_HEMI[0];
//						break;
//					case GPS_W_E:
//						GPS_HEMI[1];
//						break;
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
//			if(GPSflag.speed)
//			{
//				switch(GPS_STATE)
//				{
//					case GPS_SPEED:
//						GPSSPEED[GPS_STRING++] = GPSdata;
//						break;
//				}
//			}
			break;
	}
}
