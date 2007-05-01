#include <cstdlib>
#include <iostream>
#include <math.h>
#include "fuzz.h"

using namespace std;

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

///stuff added for this test program////////////
char run = 1;

int pitchAngle = 0;
int pitchRate = 0;

int rollAngle = 0;
int rollRate = 0;

int yawAngle = 0;
int  yawRate = 0;

int  collectiveHeight = 0;
int  collectiveRate = 0;


//////////////////////////////////////////////

int main(int argc, char *argv[])
{

    int   output = 0, reference, steer, out_steer;

	while(run)
	{
        
        ///PITCH
        cout<<"Enter pitch angle\n";
        cin>>pitchAngle;
        cout<<"Enter pitch rate\n";
        cin>>pitchAngle;
		pitch_angle_mf->sensor = pitchAngle;
		pitch_rate_mf->sensor = pitchRate;
		
		Fuzzification( pitch_param, pitch_angle_mf);
		Fuzzification( tilt_rate_param, pitch_rate_mf);
		
        output = doRules(pitch_mf, PitchRule);	// Kyle - changed doRules
        
		cout<<"Correction Value: " << output << "\n";
         
         
        ///ROLL 
        cout<<"Enter roll angle\n";
        cin>>rollAngle;
        cout<<"Enter roll rate\n";
        cin>>rollRate;
              	
		roll_angle_mf->sensor = rollAngle;
		roll_rate_mf->sensor = rollRate;
		
		Fuzzification( roll_param, roll_angle_mf);
		Fuzzification( tilt_rate_param, roll_rate_mf);
		
	    output = doRules(roll_mf, RollRule);	// Kyle - changed doRules
		
		cout<<"Correction Value: " << output << "\n";
        		
        ///YAW
        cout<<"Enter yaw angle\n";
        cin>>yawAngle;
        cout<<"Enter yaw rate\n";
        cin>>yawRate;
        
		yaw_angle_mf->sensor = yawAngle;
		yaw_rate_mf->sensor = yawRate;
		
        Fuzzification( yaw_param, yaw_angle_mf);
		Fuzzification( yaw_rate_param, yaw_rate_mf);
		
		output = doRules(yaw_mf, YawRule);	// Kyle - changed doRules
		
		cout<<"Correction Value: " << output << "\n";
	
    	///COLLECTIVE
        cout<<"Enter collective height\n";
        cin>>collectiveHeight;
        cout<<"Enter collective rate\n";
        cin>>collectiveRate;
        
		collective_height_mf->sensor = collectiveHeight;
		collective_rate_mf->sensor = collectiveRate;
		
		Fuzzification( collective_param, collective_height_mf);
		Fuzzification( collective_rate_param, collective_rate_mf);
		
        output = doRules(collective_mf, CollectiveRule);	// Kyle - changed doRules
	   
        cout<<"Correction Value: " << output << "\n";
	
        cout<<"Run again? Y/N";
        cin>>run;
        if(run == 'N' || run == 'n')
        {
         run = 0;   
        }
        else
        {
         run = 1;   
        }
	}
    
    system("PAUSE");
    return EXIT_SUCCESS;
}



