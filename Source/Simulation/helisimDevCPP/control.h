#include "simlib.h"


class HelicopterController
{
      
               
 public:
         HelicopterController();
         void UpdateSensorValues(sixdof_fe_state_def  sensors);
         double RollCorrection(double CorrectRollAngle);
         double PitchCorrection(double CorrectPitchAngle);
         double YawCorrection(double CorrectYawAngle);
         double CollectiveCorrection(double CorrectAltitude);
         
 private:
         double ForwardBackward_Accelerometer;
         double LeftRight_Accelerometer;
         double UpDown_Accelerometer;       
         
         double Roll_Gyro;
         double Pitch_Gyro;
         double Yaw_Gyro;  
         
         double Compass;
         
         double Altitude;
         double AltitudeRate;
         double North;
         double East;

      
      
};
