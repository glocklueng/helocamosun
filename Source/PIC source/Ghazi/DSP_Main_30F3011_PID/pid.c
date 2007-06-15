float CurrentAltitude;
float AltitudeRate;

float Heading_Angle;
float Yaw_Rate;

float Pitch_Angle;
float Pitch_Rate;

float Roll_Angle;
float Roll_Rate;

void UpdateSensorValues(float Heading)
{
	Heading_Angle = Heading;	
}

unsigned char CollectiveCorrection(float CorrectAltitude) 
{
  static float integral = 0;
  float PropConst = 0.15;
  float IntConst = 0.08;
  float DerConst = 0.05;
  float proportion;
  float derivative;
  float result;
  
  proportion = -PropConst * (CurrentAltitude  - CorrectAltitude);
  integral += -IntConst * (CurrentAltitude - CorrectAltitude); 
  derivative = DerConst * AltitudeRate;

  result = proportion + integral + derivative;
  
}

unsigned char YawCorrection(float CorrectYawAngle)
{
  static float integral = 0;
  float PropConst = 0.5;
  float IntConst = 0.0;
  float DerConst = 0.0;
  float proportion;
  float derivative;
  float result;
  char returnval;
  
  proportion = -PropConst * (Heading_Angle  - CorrectYawAngle);
  integral += -IntConst * (Heading_Angle - CorrectYawAngle); 
  derivative = -DerConst * AltitudeRate;

  result = proportion + integral + derivative;
  
  if((char)(result*10.0)%10 >= 5)
  {
	returnval = (char)result + 1;	  
  }
  else
  {
	returnval = (char)result;	  
  }
  if(returnval<=-50)
  {
	return 0;
  }
  else if(returnval>=50)
  {
	return 100;	  
  }
  else
  {
	return returnval + 50;
  }
 
}

unsigned char PitchCorrection(float CorrectPitchAngle)
{
  static float integral = 0;
  float PropConst = 12;
  float IntConst = 20;
  float DerConst = 4;
  float proportion;
  float derivative;
  float result;
  
  proportion = PropConst * (Pitch_Angle - CorrectPitchAngle);
  integral += IntConst * (Pitch_Angle - CorrectPitchAngle); 
  derivative = DerConst * Pitch_Rate;

  result = proportion + integral + derivative;
  
}

unsigned char RollCorrection(float CorrectRollAngle)
{
  static float integral = 0;
  float PropConst = 10;
  float IntConst = 20;
  float DerConst = 4;
  float proportion;
  float derivative;
  float result;
    
  proportion = -PropConst * (Roll_Angle - CorrectRollAngle);
  integral += -IntConst * (Roll_Angle - CorrectRollAngle); 
  derivative = DerConst * Roll_Rate;

  result = proportion + integral + derivative;
  
}
