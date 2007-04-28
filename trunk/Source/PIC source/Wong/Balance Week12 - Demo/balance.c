
/*********************************************************************
*
*				Author:	Eric desjardins and Chris Howard
*				Date: 	January 15, 2004
*				File:	balance.c
*				Project: HTV
*				
**********************************************************************/

#include <p30fxxxx.h>

#include <math.h>
#include "fuzz.h"
#include "comm.h"
//-------------------------------------------------------------------

fMember pitch_mf[3];
fMember *pitch_angle_mf = &pitch_mf[0], 
		*pitch_rate_mf  = &pitch_mf[1], 
		*pitch_accel_mf = &pitch_mf[2];
		
fMember roll_mf[3];
fMember *roll_angle_mf = &roll_mf[0], 
		*roll_rate_mf  = &roll_mf[1], 
		*roll_accel_mf = &roll_mf[2];
		
fMember yaw_mf[3];
fMember *yaw_angle_mf = &yaw_mf[0], 
		*yaw_rate_mf  = &yaw_mf[1], 
		*yaw_accel_mf = &yaw_mf[2];

fMember collective_mf[3];
fMember *collective_height_mf = &collective_mf[0], 
		*collective_rate_mf  = &collective_mf[1], 
		*collective_accel_mf = &collective_mf[2];

double negative, zero, positive;

int main(void)
{
   int   output = 0, reference, steer, out_steer;

	while(1)
	{

		pitch_angle_mf->sensor = 420;
		pitch_rate_mf->sensor = 513;
		
		roll_angle_mf->sensor = 330;
		roll_rate_mf->sensor = 200;
		
		yaw_angle_mf->sensor = 200;
		yaw_rate_mf->sensor = 203;
		
		collective_height_mf->sensor = 320;
		collective_rate_mf->sensor = 500;
		
		Fuzzification( pitch_param, pitch_angle_mf);
		Fuzzification( tilt_rate_param, pitch_rate_mf);
		
		Fuzzification( roll_param, roll_angle_mf);
		Fuzzification( tilt_rate_param, roll_rate_mf);
		
		Fuzzification( yaw_param, yaw_angle_mf);
		Fuzzification( yaw_rate_param, yaw_rate_mf);
		
		Fuzzification( collective_param, collective_height_mf);
		Fuzzification( collective_rate_param, collective_rate_mf);
		
        output = doRules(pitch_mf, PitchRule);	// Kyle - changed doRules
        output = doRules(roll_mf, RollRule);	// Kyle - changed doRules
        output = doRules(yaw_mf, YawRule);	// Kyle - changed doRules
        output = doRules(collective_mf, CollectiveRule);	// Kyle - changed doRules
      
	}
}

