#include "PIDcontrol.h"

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
     
     Altitude = 0;
     AltitudeRate = 0;
     North = 0;
     NorthRate = 0;
     East = 0;
     EastRate = 0;                        
}
void HelicopterController::UpdateSensorValues(sixdof_fe_state_def  s)
{
     
     ForwardBackward_Accelerometer = s.THETA[1] * C_RAD2DEG;
     LeftRight_Accelerometer = s.THETA[0] * C_RAD2DEG;
     UpDown_Accelerometer = s.accel[UPDOWN_ACCELEROMETER];             
    
     Roll_Gyro = s.rate[ROLLRATE_GYRO] * C_RAD2DEG;
     Pitch_Gyro = s.rate[PITCHRATE_GYRO] * C_RAD2DEG;
     Yaw_Gyro = s.rate[YAWRATE_GYRO] * C_RAD2DEG;
     
     Compass = s.THETA[COMPASS] * C_RAD2DEG;    
     
     Altitude = -s.NED[DOWN] * C_FT2M;
     AltitudeRate = s.Ve[DOWN] * C_FT2M;
     North = s.NED[NORTH];
     NorthRate = s.Ve[NORTH];
     East = s.NED[EAST];
     EastRate = s.Ve[EAST];                            
}
double HelicopterController::RollCorrection(double CorrectRollAngle)
{
  static double integral = 0;
  double PropConst = 0.3;
  double IntConst = 1.58;
  double DerConst = 0.06;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (LeftRight_Accelerometer - CorrectRollAngle);
  integral += -IntConst * (LeftRight_Accelerometer - CorrectRollAngle)* CalcPeriod; 
  derivative = DerConst * Roll_Gyro;

  return  proportion + integral + derivative;
}
double HelicopterController::PitchCorrection(double CorrectPitchAngle)
{
  static double integral = 0;
  double PropConst = 0.3;
  double IntConst = 1.58;
  double DerConst = 0.06;
  double proportion;
  double derivative;
  
  proportion = PropConst * (ForwardBackward_Accelerometer - CorrectPitchAngle);
  integral += IntConst * (ForwardBackward_Accelerometer - CorrectPitchAngle)* CalcPeriod; 
  derivative = DerConst * Pitch_Gyro;

  return  proportion + integral + derivative;
}

double HelicopterController::YawCorrection(double CorrectYawAngle)
{
  static double integral = 0;
  double PropConst = 0.3;
  double IntConst = 0.8;
  double DerConst = 0.07;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (Compass  - CorrectYawAngle);
  integral += -IntConst * (Compass - CorrectYawAngle)* CalcPeriod; 
  derivative = -DerConst * Yaw_Gyro;

  return  proportion + integral + derivative;
}

double HelicopterController::CollectiveCorrection(double CorrectAltitude)
{
  static double integral = 0;
  double PropConst = 15;
  double IntConst = 8;
  double DerConst = 5;
  double proportion;
  double derivative;
  
  proportion = -PropConst * (Altitude  - CorrectAltitude);
  integral += -IntConst * (Altitude - CorrectAltitude)* CalcPeriod; 
  derivative = DerConst * AltitudeRate;

  return  proportion + integral + derivative;
}
