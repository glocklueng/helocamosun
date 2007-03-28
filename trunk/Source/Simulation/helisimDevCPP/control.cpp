#include "control.h"

#define CalcPeriod 0.02 //50hz

#define FORWARDBACKWARD_ACCELEROMETER 0
#define LEFTRIGHT_ACCELEROMETER 1
#define UPDOWN_ACCELEROMETER 2

#define ROLLRATE_GYRO 0
#define PITCHRATE_GYRO 1
#define YAWRATE_GYRO 2

#define COMPASS 2

#define NORTH 0
#define EAST 1
#define DOWN 2


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
     
     ForwardBackward_Accelerometer = s.THETA[1];//s.accel[FORWARDBACKWARD_ACCELEROMETER];
     LeftRight_Accelerometer = s.THETA[0];//s.accel[FORWARDBACKWARD_ACCELEROMETER];
     UpDown_Accelerometer = s.accel[UPDOWN_ACCELEROMETER];             
    
     Roll_Gyro = s.rate[ROLLRATE_GYRO];
     Pitch_Gyro = s.rate[PITCHRATE_GYRO];
     Yaw_Gyro = s.rate[YAWRATE_GYRO];
     
     Compass = s.THETA[COMPASS];    
     
     Altitude = -s.NED[DOWN];
     AltitudeRate = s.Vb[2];
     North = s.NED[NORTH];
     East = s.NED[EAST];                              
}
double HelicopterController::RollCorrection(double CorrectRollAngle)
{
  static double integral = 0;
  double PropConst = 10;
  double IntConst = 20;
  double DerConst = 4;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (LeftRight_Accelerometer - CorrectRollAngle);
  integral += -IntConst * (LeftRight_Accelerometer - CorrectRollAngle)* CalcPeriod; 
  derivative = DerConst * Roll_Gyro;

  return  proportion + derivative + integral;    
}
double HelicopterController::PitchCorrection(double CorrectPitchAngle)
{
  static double integral = 0;
  double PropConst = 12;
  double IntConst = 20;
  double DerConst = 4;
  double proportion;
  double derivative;
  
  proportion = PropConst * (ForwardBackward_Accelerometer - CorrectPitchAngle);
  integral += IntConst * (ForwardBackward_Accelerometer - CorrectPitchAngle)* CalcPeriod; 
  derivative = DerConst * Pitch_Gyro;

  return  proportion + derivative + integral;    
}

double HelicopterController::YawCorrection(double CorrectYawAngle)
{
  static double integral = 0;
  double PropConst = 20;
  double IntConst = 25;
  double DerConst = 6;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (Compass  - CorrectYawAngle);
  integral += -IntConst * (Compass - CorrectYawAngle)* CalcPeriod; 
  derivative = -DerConst * Yaw_Gyro;

  return  proportion + derivative + integral;    
}

double HelicopterController::CollectiveCorrection(double CorrectAltitude)
{
  static double integral = 0;
  double PropConst = 0.06;
  double IntConst = 0.03;
  double DerConst = 0.03;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (Altitude  - CorrectAltitude);
  integral += -IntConst * (Altitude - CorrectAltitude)* CalcPeriod; 
  derivative = DerConst * AltitudeRate;

  return  proportion + derivative + integral;    
}
