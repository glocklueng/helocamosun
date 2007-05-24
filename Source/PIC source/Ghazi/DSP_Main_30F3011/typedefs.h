
typedef struct
{
	char deg;
	char min;
	short sec;
	char hemi;
} GPT_latlong;

typedef struct
{
     GPT_latlong latitude;
     GPT_latlong longitude;
     char action;
     short data;	// either an altitude to hover at, or a search pattern to conduct
} GPT_goto_position;

typedef struct
{
	unsigned short heading;
	unsigned char speed;
	unsigned short altitude;	
} GPT_HSA;

typedef struct
{
	signed short pitch, roll, yaw;	
} GPT_attitude;

typedef struct
{
	signed short voltage, current, temp;	
} GPT_batterystatus;

typedef struct
{
	GPT_latlong latitude, longitude;	
} GPT_position;

typedef struct
{
	unsigned char pitch, roll, yaw, coll, engRPM;	
} GPT_pwm;

typedef struct
{
	GPT_goto_position goto_position;
	GPT_HSA hsa;
	GPT_attitude attitude;
	GPT_batterystatus batterystatus;
	GPT_position position;	
	unsigned short motorRPM;
	unsigned short GPS_alt, SON_alt;
	unsigned char sensors;
	GPT_pwm pwm;
	
} GPT_helicopter;