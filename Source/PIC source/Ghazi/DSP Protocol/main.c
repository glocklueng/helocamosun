// testmain

#include <p30fxxxx.h>
//#include "LCD Display.h"
//#include "common.h"

int main ( void )
{
	LATBbits.LATB1 = 1;
	U1MODEbits.UARTEN = 1;
	return 0;
}
