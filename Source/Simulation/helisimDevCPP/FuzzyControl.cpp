#include "FuzzyControl.h"
#include <Math.h>
#include <stdio.h>
#include <string.h>

float pitch_param[NUM_RANGE][3]={	//degrees member function parameters 
      {-1.00f, -1.0f, -0.5f},
      {-0.8f, -0.5f, -0.3f},
      {-0.01, 0, 0.01f},
      {0.3f, 0.50f, 0.80f},
      {0.50f, 1.0f, 1.0f}
};

float roll_param[NUM_RANGE][3]={	//degrees member function parameters
      {-1.00f, -1.0f, -0.5f},
      {-0.8f, -0.5f, -0.3f},
      {-0.1, 0, 0.1f},
      {0.3f, 0.50f, 0.80f},
      {0.50f, 1.0f, 1.0f}
};			

float tilt_rate_param[NUM_RANGE][3] = {	//rate member function parameters 
      {-1.00f, -1.0f, -0.5f},
      {-0.8f, -0.5f, -0.3f},
      {-0.1, 0, 0.1f},
      {0.3f, 0.50f, 0.80f},
      {0.50f, 1.0f, 1.0f}
      
};

float yaw_param[NUM_RANGE][3]={	//degrees member function parameters
      {-2.0f, -1.8f, -1.6f},
      {-1.8f, -1.2f, -1.0f},
      {-1.2f, 0, 1.2f},
      {1.0f, 1.2f, 1.8f},
      {1.2f, 1.8f, 2.0f}
};

float yaw_rate_param[NUM_RANGE][3]={	//degrees member function parameters
      {2.0f, 1.8f, 1.2f},
      {1.8f, 1.2f, 1.0f},
      {1.2f, 0, -1.2f},
      {-1.0f, -1.2f, -1.8f},
      {-1.6f, -1.8f, -2.0f}


};

float collective_param[NUM_RANGE][3]={	//degrees member function parameters
      {-2.0f, -1.8f, -1.6f},
      {-1.8f, -1.2f, -1.0f},
      {-1.2f, 0, 1.2f},
      {1.0f, 1.2f, 1.8f},
      {1.2f, 1.8f, 2.0f}
};

float collective_rate_param[NUM_RANGE][3]={	//degrees member function parameters
      {-2.0f, -1.8f, -1.6f},
      {-1.8f, -1.2f, -1.0f},
      {-1.2f, 0, 1.2f},
      {1.0f, 1.2f, 1.8f},
      {1.2f, 1.8f, 2.0f}
};

#define NL_WEIGHT (-10)         // output weightings
#define N_WEIGHT (-1)
#define Z_WEIGHT (0)
#define P_WEIGHT (1)
#define PL_WEIGHT (10)

/************************************************************************
 *                                                                      *
 * Equ_line()                                                           *
 *  ______________...___ y1        y1,y2 _..._____________              *
 * |    /\              |     ~         |          /   ^  |             *
 * |   /  \             |     OR        |         /    |  |             *
 * |  /   ^\            |      ~        |        /     |  |             *
 * |_/____|_\_____...___|y2             |_...___/______|__|             *
 *  x1    x x2                                 x1      x  x2            *
 *                                                                      *
 * Params - x = x-value, the input which is being used to determine the *
 *              degree of membership                                    *
 *         x1 = the left edge of the slope                              *
 *         x2 = the right edge of the slope                             *
 *         y1 = the beginning of the slope                              *
 *         y2 = the end of the slope                                    *
 *                                                                      *
 * Description -  This function calculates the degree of membership     *
 *                for an input range. It does so by using the equation  *
 *                of a line to determine the y-value based on a given   *
 *                x-value.                                              *
 *                                                                      *
 *                Refer to the diagrams above to determine which values *
 *                to send for the parameters.                           *
 *                                                                      *
 ************************************************************************/	
float Equ_line( float x, float x1, float x2, float y1, float y2)
{
		return (y2-y1)/(x2-x1)*(x - x1) + y1;		
}

/*
 *
 * Fuzzification()
 *
 */
void Fuzzification( float input_param[][3], fMember *input_mf)
{
   input_mf->nl = 0;
   input_mf->n  = 0;
   input_mf->z  = 0;
   input_mf->p  = 0;
   input_mf->pl = 0;
   
   //~~~~~~~~~~~~~~~~~~~~~~ Negative Large Triangle ~~~~~~~~~~~~~~~~~~~
   if( input_mf->sensor < input_param[NL][1] )
   {
      input_mf->nl=Equ_line((float)input_mf->sensor, input_param[NL][0],
                                   input_param[NL][1], 1, 1);
   }
   if( input_mf->sensor < input_param[NL][2] &&
       input_mf->sensor >= input_param[NL][1] )
   {
      input_mf->nl=Equ_line((float)input_mf->sensor, input_param[NL][1],
                                   input_param[NL][2], 1, 0);
   }
   
   //~~~~~~~~~~~~~~~~~~~~~~~~~~ Negative Triangle ~~~~~~~~~~~~~~~~~~~~~
   if( input_mf->sensor < input_param[N][1] &&
       input_mf->sensor >= input_param[N][0] )
   {
      input_mf->n =Equ_line((float)input_mf->sensor, input_param[N][0],  
                                   input_param[N][1], 0, 1);		         
   }																																		 
   if( input_mf->sensor < input_param[N][2] &&
       input_mf->sensor >= input_param[N][1] )
   {
      input_mf->n = Equ_line((float)input_mf->sensor, input_param[N][1],
                                    input_param[N][2], 1, 0);														
   }

   //~~~~~~~~~~~~~~~~~~~~~~~~~ Zero Triangle ~~~~~~~~~~~~~~~~~~~~~~~~~~
   if( input_mf->sensor < input_param[Z][1] &&
   	 input_mf->sensor >= input_param[Z][0] )
   {
      input_mf->z =Equ_line((float)input_mf->sensor, input_param[Z][0],
                                   input_param[Z][1], 0, 1);	
   }
   if( input_mf->sensor < input_param[Z][2] &&
       input_mf->sensor >= input_param[Z][1] )
   {

      input_mf->z = Equ_line((float)input_mf->sensor, input_param[Z][1],
                                    input_param[Z][2], 1, 0);													
   }
	
	//~~~~~~~~~~~~~~~~~~~~~~~~	Positive Triangle ~~~~~~~~~~~~~~~~~~~~~~~
	if( input_mf->sensor < input_param[P][1] &&
   	 input_mf->sensor >= input_param[P][0] )
   {
      input_mf->p =Equ_line((float)input_mf->sensor, input_param[P][0],
                                   input_param[P][1], 0, 1);	
   }
   if( input_mf->sensor < input_param[P][2] &&
       input_mf->sensor >= input_param[P][1] )
   {	
      input_mf->p = Equ_line((float)input_mf->sensor, input_param[P][1],
                                    input_param[P][2], 1, 0); 												
   }
   
   //~~~~~~~~~~~~~~~~~~~~~ Positive Large Triangle ~~~~~~~~~~~~~~~~~~~~
   if( input_mf->sensor < input_param[PL][1] &&
       input_mf->sensor >= input_param[PL][0] )
   {
      input_mf->pl=Equ_line((float)input_mf->sensor, input_param[PL][0],
                                   input_param[PL][1], 0, 1);
   }
   if( input_mf->sensor >= input_param[PL][1] )
   {
      input_mf->pl=Equ_line((float)input_mf->sensor, input_param[PL][1],
                                   input_param[PL][2], 1, 1);
   }
}
//
// The rule set for the fuzzy logic unit
//
char PitchRule[] =                          // SAMPLE RULE SET
{ 
   ANGLE, _NL, RATE, _NL, _THEN, _NL,
   ANGLE, _NEG,  RATE, _NL, _THEN, _NL,
   ANGLE, _ZERO,  RATE, _NL, _THEN, _NL,
   ANGLE, _POS,  RATE, _NL, _THEN, _NEG,
   ANGLE, _PL, RATE, _NL, _THEN, _NEG,
   
   ANGLE, _NL, RATE, _NEG,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _POS,  RATE, _NEG,  _THEN, _POS,
   ANGLE, _PL, RATE, _NEG,  _THEN, _POS,
  
   ANGLE, _NL, RATE, _ZERO,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _ZERO,  _THEN, _NEG, 
   ANGLE, _ZERO,  RATE, _ZERO,  _THEN, _ZERO,
   ANGLE, _POS,  RATE, _ZERO,  _THEN, _POS,
   ANGLE, _PL, RATE, _ZERO,  _THEN, _PL,
  
   ANGLE, _NL, RATE, _POS,  _THEN, _NEG,
   ANGLE, _NEG,  RATE, _POS,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _POS,  _THEN, _POS,
   ANGLE, _POS,  RATE, _POS,  _THEN, _POS,
   ANGLE, _PL, RATE, _POS,  _THEN, _PL,
   
   ANGLE, _NL, RATE, _PL, _THEN, _POS,
   ANGLE, _NEG,  RATE, _PL, _THEN, _POS,
   ANGLE, _ZERO,  RATE, _PL, _THEN, _PL,
   ANGLE, _POS,  RATE, _PL, _THEN, _PL,
   ANGLE, _PL, RATE, _PL, _THEN, _PL
};

//
// The rule set for the fuzzy logic unit
//
char RollRule[] =                          // SAMPLE RULE SET
{ 
   ANGLE, _NL, RATE, _NL, _THEN, _NL,
   ANGLE, _NEG,  RATE, _NL, _THEN, _NL,
   ANGLE, _ZERO,  RATE, _NL, _THEN, _NL,
   ANGLE, _POS,  RATE, _NL, _THEN, _NEG,
   ANGLE, _PL, RATE, _NL, _THEN, _NEG,
   
   ANGLE, _NL, RATE, _NEG,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _POS,  RATE, _NEG,  _THEN, _POS,
   ANGLE, _PL, RATE, _NEG,  _THEN, _POS,
  
   ANGLE, _NL, RATE, _ZERO,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _ZERO,  _THEN, _NEG, 
   ANGLE, _ZERO,  RATE, _ZERO,  _THEN, _ZERO,
   ANGLE, _POS,  RATE, _ZERO,  _THEN, _POS,
   ANGLE, _PL, RATE, _ZERO,  _THEN, _PL,
  
   ANGLE, _NL, RATE, _POS,  _THEN, _NEG,
   ANGLE, _NEG,  RATE, _POS,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _POS,  _THEN, _POS,
   ANGLE, _POS,  RATE, _POS,  _THEN, _POS,
   ANGLE, _PL, RATE, _POS,  _THEN, _PL,
   
   ANGLE, _NL, RATE, _PL, _THEN, _POS,
   ANGLE, _NEG,  RATE, _PL, _THEN, _POS,
   ANGLE, _ZERO,  RATE, _PL, _THEN, _PL,
   ANGLE, _POS,  RATE, _PL, _THEN, _PL,
   ANGLE, _PL, RATE, _PL, _THEN, _PL
};

//
// The rule set for the fuzzy logic unit
//
char YawRule[] =                          // SAMPLE RULE SET
{ 
   ANGLE, _NL, RATE, _NL, _THEN, _NL,
   ANGLE, _NEG,  RATE, _NL, _THEN, _NL,
   ANGLE, _ZERO,  RATE, _NL, _THEN, _NL,
   ANGLE, _POS,  RATE, _NL, _THEN, _NEG,
   ANGLE, _PL, RATE, _NL, _THEN, _NEG,
   
   ANGLE, _NL, RATE, _NEG,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _POS,  RATE, _NEG,  _THEN, _POS,
   ANGLE, _PL, RATE, _NEG,  _THEN, _POS,
  
   ANGLE, _NL, RATE, _ZERO,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _ZERO,  _THEN, _NEG, 
   ANGLE, _ZERO,  RATE, _ZERO,  _THEN, _ZERO,
   ANGLE, _POS,  RATE, _ZERO,  _THEN, _POS,
   ANGLE, _PL, RATE, _ZERO,  _THEN, _PL,
  
   ANGLE, _NL, RATE, _POS,  _THEN, _NEG,
   ANGLE, _NEG,  RATE, _POS,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _POS,  _THEN, _POS,
   ANGLE, _POS,  RATE, _POS,  _THEN, _POS,
   ANGLE, _PL, RATE, _POS,  _THEN, _PL,
   
   ANGLE, _NL, RATE, _PL, _THEN, _POS,
   ANGLE, _NEG,  RATE, _PL, _THEN, _POS,
   ANGLE, _ZERO,  RATE, _PL, _THEN, _PL,
   ANGLE, _POS,  RATE, _PL, _THEN, _PL,
   ANGLE, _PL, RATE, _PL, _THEN, _PL
};

char CollectiveRule[] =                          // SAMPLE RULE SET
{ 
   ANGLE, _NL, RATE, _NL, _THEN, _NL,
   ANGLE, _NEG,  RATE, _NL, _THEN, _NL,
   ANGLE, _ZERO,  RATE, _NL, _THEN, _NL,
   ANGLE, _POS,  RATE, _NL, _THEN, _NEG,
   ANGLE, _PL, RATE, _NL, _THEN, _NEG,
   
   ANGLE, _NL, RATE, _NEG,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _NEG,  _THEN, _NEG,
   ANGLE, _POS,  RATE, _NEG,  _THEN, _POS,
   ANGLE, _PL, RATE, _NEG,  _THEN, _POS,
  
   ANGLE, _NL, RATE, _ZERO,  _THEN, _NL,
   ANGLE, _NEG,  RATE, _ZERO,  _THEN, _NEG, 
   ANGLE, _ZERO,  RATE, _ZERO,  _THEN, _ZERO,
   ANGLE, _POS,  RATE, _ZERO,  _THEN, _POS,
   ANGLE, _PL, RATE, _ZERO,  _THEN, _PL,
  
   ANGLE, _NL, RATE, _POS,  _THEN, _NEG,
   ANGLE, _NEG,  RATE, _POS,  _THEN, _NEG,
   ANGLE, _ZERO,  RATE, _POS,  _THEN, _POS,
   ANGLE, _POS,  RATE, _POS,  _THEN, _POS,
   ANGLE, _PL, RATE, _POS,  _THEN, _PL,
   
   ANGLE, _NL, RATE, _PL, _THEN, _POS,
   ANGLE, _NEG,  RATE, _PL, _THEN, _POS,
   ANGLE, _ZERO,  RATE, _PL, _THEN, _PL,
   ANGLE, _POS,  RATE, _PL, _THEN, _PL,
   ANGLE, _PL, RATE, _PL, _THEN, _PL
};

/************************************************************************
 *                                                                      *
 * doRules()                                                            *
 *                                                                      *
 * Returns - the necessary output (-100%~100%) to keep the unit balance *
 *                                                                      *
 ************************************************************************/
float doRules( fMember *all_mf, char *Rules )
{
   float RSS[NUM_RANGE];
   char rIndex = 0, rNum = 0;
   float *curInput;
   float ruleVal, wSum;
   int inum;
   fMember vout_mf;	// Kyle
   
   float temp;
	 //
	 // Make sure that the RSS array starts at zero
	 //   
   memset( RSS, 0, sizeof(float)*NUM_RANGE );
   
   //
   // Doing testing - Display the fuzzy numbers
   //
   //displayVals( angle_mf, 0 );
   //displayVals( rate_mf, 1 );

   //
   // Process the ruleset like it was processor machine code
   //
   while( rNum < NUM_RULES )
   {
      //
      // Determine if the current rule is true
      //
      ruleVal = 1;
      while( Rules[rIndex] != _THEN )
      {
         //
         // Determine which input is being used
         //
         temp = all_mf[ Rules[rIndex++] ].nl;
         curInput = &temp;
         
         //
         // Determine if the condition contains the lowest value for the 
         //    statement
         //
         if( *(curInput + Rules[rIndex]) < ruleVal )
         {
            ruleVal = *(curInput + Rules[rIndex]);
         }
         
         rIndex++;     
      }

      //
      // Doing testing - display the rule results
      //
      //dispRule( ruleVal, rNum );

      //
      // Add the value to the running RSS
      //
      rIndex++;
      rNum++;
      inum = Rules[rIndex++];
      RSS[ inum ] = RSS[ inum ] + ( ruleVal * ruleVal );
   }
   
   //
   // Done processing the rules, complete the RSS equation to get the 
   //    output linguistics
   //
   curInput = &vout_mf.nl;
   for( rIndex = 0; rIndex < NUM_RANGE; rIndex++ )
   {
      //*curInput = doSqrt( RSS[rIndex] );
      *curInput = sqrtf( RSS[rIndex] );
      curInput++;
   }

   //
   // Defuzzification
   //
   // Out = ( out.n*(-100) + out.z*(0) + out.p*(100) ) /
   // 			         ( out.n + out.z + out.p )
   //
   wSum = 0;

   wSum =  vout_mf.nl * NL_WEIGHT;
   wSum += vout_mf.n  * N_WEIGHT;
   wSum += vout_mf.z  * Z_WEIGHT;
   wSum += vout_mf.p  * P_WEIGHT;
   wSum += vout_mf.pl * PL_WEIGHT;
   wSum /= ( vout_mf.nl + vout_mf.n + vout_mf.z + vout_mf.p + vout_mf.pl );

    return wSum;
  // inum = (int)wSum;
 //  return inum;
}

