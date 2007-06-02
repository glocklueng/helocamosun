//Fuzz.h

#ifndef __FUZZ_H
#define __FUZZ_H

#define NUM_RULES 23
#define NUM_RANGE 5

#define NL Z-2
#define N Z-1
#define Z 2
#define P Z+1
#define PL Z+2

// Inputs
#define ANGLE  0x00
#define RATE   0x01
#define ACCEL  0x02

// Linguistics
#define _NL    0x00
#define _NEG   0x01
#define _ZERO  0x02
#define _POS   0x03
#define _PL    0x04

// Logic delimeter
#define _THEN  0x59

//Struct to store weighted strengths of each i/o member function
typedef struct
{
		float nl;                     // Accel backward
		float n;                      // Balance backward
		float z;                      // Zero
		float p;                      // Balance forward
		float pl;                     // Accel forward
		float sensor;
} fMember;

extern char PitchRule[];
extern char RollRule[];
extern char YawRule[];
extern char CollectiveRule[];

extern float pitch_param[NUM_RANGE][3];	//degrees member function parameters
		
extern float roll_param[NUM_RANGE][3];

extern float tilt_rate_param[NUM_RANGE][3];	//rate member function parameters

extern float yaw_param[NUM_RANGE][3];

extern float yaw_rate_param[NUM_RANGE][3];

extern float collective_param[NUM_RANGE][3];

extern float collective_rate_param[NUM_RANGE][3];

//float doSqrt( float num );	//Function that square roots floats between 0 and 6.5
float Equ_line( float x, float x1, float x2, float y1, float y2);		//Equation of a line y=mx+b
void Fuzzification( float input_param[][3], fMember *input_mf);		//Fuzzify sensor values
//int doRules(void);  //Applies the rule set defined in the array Rules[]
float  doRules(fMember *all_mf, char *Rule);  //Applies the rule set defined in the array Rules[]

#endif
