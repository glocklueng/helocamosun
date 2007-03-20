#include "simlib.h"


class HelicopterController
{
      
               
 public:
         HelicopterController();
         void UpdateSensorValues(sixdof_fe_state_def  sensors);
         double RollCorrection(double CorrectValue);
         double PitchCorrection(double CorrectValue);
         double YawCorrection(double CorrectValue);
         
 private:
         double ForwardBackward_Accelerometer;
         double LeftRight_Accelerometer;
         double UpDown_Accelerometer;       
         
         double Roll_Gyro;
         double Pitch_Gyro;
         double Yaw_Gyro;  
         
         double Compass;

      
      
};
