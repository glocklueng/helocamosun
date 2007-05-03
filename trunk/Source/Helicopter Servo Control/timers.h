/*
timers.h
*/

#define TIMER1FLAG	PIR1bits.C

typedef struct 
{
	unsigned char newTickFlag:1;
	unsigned char new100msTickFlag:1;
	unsigned char new1sTickFlag:1;
}TIMEFLAGS;

typedef struct
{
	unsigned char rxBuf:1;
	unsigned char debug:1;
	unsigned char bist:1;
	unsigned char startup:1;
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
