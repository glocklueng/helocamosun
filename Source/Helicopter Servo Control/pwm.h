/*
pwm.h
*/
#define PITCH_CORRECTION_FACTOR 	 2.36
#define ROLL_CORRECTION_FACTOR 		 2.51
#define YAW_CORRECTION_FACTOR 		 1
#define COLLECTIVE_CORRECTION_FACTOR 2.80
#define PITCH_MIN_VAL 				 0x014F
#define ROLL_MIN_VAL 				 0x0147
#define YAW_MIN_VAL 				 0x01CF
#define COLLECTIVE_MIN_VAL 			 0x0138

void PCPWMInit(void);
void UpdatePWM(void);

