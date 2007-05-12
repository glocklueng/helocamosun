#include "GPS.h"

#define GetInfo		0x00
#define GetValid	0x01
#define GetSats		0x02
#define GetTime		0x03
#define GetDate		0x04
#define GetLat		0x05
#define GetLong		0x06
#define GetAlt		0x07
#define GetSpeed	0x08
#define GetHead		0x09


void SendGPSCommand(char GPS_COMMAND)
{
	WriteUSART('!');
	while(BusyUSART());
	WriteUSART('G');
	while(BusyUSART());
	WriteUSART('P');
	while(BusyUSART());
	WriteUSART('S');
	while(BusyUSART());
	
	switch(GPS_COMMAND)
	{
		case GetInfo:
			WriteUSART(GetInfo);
			while(BusyUSART());
			break;
		case GetValid:
			WriteUSART(GetValid);
			while(BusyUSART());
			break;
		case GetSats:
			WriteUSART(GetSats);
			while(BusyUSART());
			break;
		case GetTime:
			WriteUSART(GetTime);
			while(BusyUSART());
			break;
		case GetDate:
			WriteUSART(GetDate);
			while(BusyUSART());
			break;
		case GetLat:
			WriteUSART(GetLat);
			while(BusyUSART());
			break;
		case GetLong:
			WriteUSART(GetLong);
			while(BusyUSART());
			break;
		case GetAlt:
			WriteUSART(GetAlt);
			while(BusyUSART());
			break;
		case GetSpeed:
			WriteUSART(GetSpeed);
			while(BusyUSART());
			break;
		case GetHead:
			WriteUSART(GetHead);
			while(BusyUSART());
			break;
		default:
			break;
	}
}
