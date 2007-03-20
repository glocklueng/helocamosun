#include "control.h"

#define CalcPeriod 0.02 //50hz

#define FORWARDBACKWARD_ACCELEROMETER 0
#define LEFTRIGHT_ACCELEROMETER 1
#define UPDOWN_ACCELEROMETER 2

#define ROLLRATE_GYRO 0
#define PITCHRATE_GYRO 1
#define YAWRATE_GYRO 2

#define COMPASS 2


HelicopterController::HelicopterController()
{
    ForwardBackward_Accelerometer = 0;
    LeftRight_Accelerometer = 0;
    UpDown_Accelerometer = 0;     
    Roll_Gyro = 0;
    Pitch_Gyro = 0;
    Yaw_Gyro = 0;
    Compass = 0;                      
}
void HelicopterController::UpdateSensorValues(sixdof_fe_state_def  s)
{
     ForwardBackward_Accelerometer = s.accel[FORWARDBACKWARD_ACCELEROMETER];
     LeftRight_Accelerometer = s.accel[FORWARDBACKWARD_ACCELEROMETER];
     UpDown_Accelerometer = s.accel[UPDOWN_ACCELEROMETER];             
    
     Roll_Gyro = s.rate[ROLLRATE_GYRO];
     Pitch_Gyro = s.rate[PITCHRATE_GYRO];
     Yaw_Gyro = s.rate[YAWRATE_GYRO];
     
     Compass = s.THETA[COMPASS];                                    
}
double HelicopterController::RollCorrection(double CorrectValue)
{
  return 0.1;       
       
}
double HelicopterController::PitchCorrection(double CorrectValue)
{
  return 0.1;       
       
}

double HelicopterController::YawCorrection(double CorrectValue)
{
  static double integral = 0;
  double PropConst = 50;
  double IntConst = 12;
  double DerConst = 10;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (Compass  - CorrectValue);
  integral += -IntConst * (Compass - CorrectValue)* CalcPeriod; 
  derivative = -DerConst * Yaw_Gyro;

  return  proportion + derivative + integral;    
}
