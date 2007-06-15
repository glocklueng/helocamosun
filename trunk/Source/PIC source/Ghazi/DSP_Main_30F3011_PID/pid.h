void UpdateSensorValues(float Heading);

unsigned char CollectiveCorrection(float CorrectAltitude);
unsigned char YawCorrection(float CorrectYawAngle);
unsigned char PitchCorrection(float CorrectPitchAngle);
unsigned char RollCorrection(float CorrectRollAngle);

extern float CurrentAltitude;
extern float AltitudeRate;

extern float Heading_Angle;
extern float Yaw_Rate;

extern float Pitch_Angle;
extern float Pitch_Rate;

extern float Roll_Angle;
extern float Roll_Rate;
