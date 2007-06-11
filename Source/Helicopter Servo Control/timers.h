/*
timers.h
*/

//#define TIMER1FLAG PIR1bits.TMR1IF
#define TIMER1FLAG PIR1bits.C
#define TIMER0FLAG INTCONbits.T0IF

#define STATUS_LED1		LATDbits.LATD4
#define SET_STATUS_LED1 TRISDbits.TRISD4
#define STATUS_LED2 	LATBbits.LATB0
#define SET_STATUS_LED2 TRISBbits.TRISB0
#define ANTI_COLL_LED 	LATDbits.LATD5
#define SET_ANTI_COLL_LED 	TRISDbits.TRISD5

typedef struct 
{
	unsigned char newTickFlag:1;
	unsigned char new100msTickFlag:1;
	unsigned char new1sTickFlag:1;
}TIMEFLAGS;

typedef struct
{
	unsigned char debug:1;
	unsigned char main:1;
}CONTROL;

typedef struct
{
	unsigned char delay100ms;
	unsigned char delay1s;
}TIMERS;

extern TIMEFLAGS tFlag;
extern CONTROL cFlag;
extern TIMERS tDelay;

void TimerInit(void);
void TimeKeeping(void);
void LedStates(void);
