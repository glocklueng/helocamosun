#include <USART.h>
#include "SPIComms.h"
#include "GPS.h"
#include "variables.h"
#include "timers.h"
#include "pwm.h"

#define WAITING_FOR_COMMAND_STATE 	0
#define ACCELEROMETER_COMMAND_STATE 1
#define TWO_AXIS_GYRO_COMMAND_STATE 2
#define COMPASS_COMMAND_STATE 		3
#define ACCOUSTIC_COMMAND_STATE 	4
#define VOLTAGES_COMMAND_STATE 		5
#define TEMPERATURE_COMMAND_STATE 	6
#define RPM_COMMAND_STATE 			7
#define STATUS_COMMAND_STATE 		8
#define PWM_COMMAND_STATE			9
#define GPS_TIME_STATE				10
#define GPS_LATITUDE_STATE			11
#define GPS_LONGITUDE_STATE			12
#define GPS_SATELLITES_STATE		13
#define GPS_ALTITUDE_STATE			14

#define ACCELEROMETER_COMMAND 	'A'
#define TWO_AXIS_GYRO_COMMAND 	'G'
#define COMPASS_COMMAND 		'C'
#define ACCOUSTIC_COMMAND 		'O'
#define VOLTAGES_COMMAND 		'V'
#define TEMPERATURE_COMMAND 	'T'
#define RPM_COMMAND 			'R'
#define STATUS_COMMAND 			'S'
#define PWM_COMMAND				'P'
#define	GPS_TIME_COMMAND		'M'	// Change these values
#define	GPS_LATITUDE_COMMAND	'L'	// 		|
#define	GPS_LONGITUDE_COMMAND	'D' //		|
#define	GPS_SATELLITES_COMMAND	'B' //		v
#define GPS_ALTITUDE_COMMAND	'U' // ------------------


char dummydata[11] ={"0123456789"};
char Temperature[2];
char command[3];

void SPI_Init (void)
{

	TRISCbits.TRISC6 = 1;//Slave Select (SS) – RC6/TX/CK/SS
	TRISDbits.TRISD3 = 1;//Serial Clock (SCK) – RD3/INT2/SCK/SCL
	TRISDbits.TRISD1 = 0;//Serial Data Out (SDO) – RD1/RX/DT/SDO
	TRISDbits.TRISD2 = 1;//Serial Data In (SDI) – RD2/INT1/SDI/SDA
	
	//	SSPM3:SSPM0: Synchronous Serial Port Mode Select bits
	//	0000 = SPI Master mode, clock = FOSC/4
	//	0001 = SPI Master mode, clock = FOSC/16
	//	0010 = SPI Master mode, clock = FOSC/64
	//	0011 = SPI Master mode, clock = TMR2 output/2
	//	0100 = SPI Slave mode, clock = SCK pin. SS pin control enabled.
	//	0101 = SPI Slave mode, clock = SCK pin. SS pin control disabled. SS can be used
	//	0110 = I2C Slave mode, 7-bit address
	//	0111 = I2C Slave mode, 10-bit address
	//	1011 = I2C Firmware Controlled Master mode (slave Idle)
	//	1110 = I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
	//	1111 = I2C Slave mode, 10-bit address with Start and Stop bit interrupts enabled
		
	SSPCONbits.SSPM3 = 0;
	SSPCONbits.SSPM2 = 1;
	SSPCONbits.SSPM1 = 0;
	SSPCONbits.SSPM0 = 1;


	SSPSTATbits.SMP = 0;	//	SMP: SPI Data Input Sample Phase bit
							//	SPI Master mode:
							//	1 = Input data sampled at end of data output time
							//	0 = Input data sampled at middle of data output time (Microwire®)
							//	SPI Slave mode:
							///SMP must be cleared when SPI is used in Slave mode

	SSPCONbits.CKP = 1;		//CKP:
							//1 = Idle state for clock is a high level (Microwire® default)
							//0 = Idle state for clock is a low level (Microwire® alternate)

	SSPSTATbits.CKE = 0;	//CKE: SPI Clock Edge Select bit (Figure 18-2, Figure 18-3, and Figure 18-4)
							//SPI mode, CKP = 0:
							//1 = Data transmitted on rising edge of SCK (Microwire® alternate)
							//0 = Data transmitted on falling edge of SCK
							//SPI mode, CKP = 1:
							//1 = Data transmitted on falling edge of SCK (Microwire® default)
							//0 = Data transmitted on rising edge of SCK

	PIE1bits.SSPIE = 1;	//interrupt enable
	PIR1bits.SSPIF = 0;	//interrupt flag

	IPR1bits.SSPIP = 1; //SSPIP: Synchronous Serial Port Interrupt Priority bit
						//1 = High priority
						//0 = Low priority


	SSPCONbits.SSPEN = 1;   //SSPEN:
							//1 = Enables serial port and configures SCK, SDO and SDI as serial port pins
							//0 = Disables serial port and configures these pins as I/O port pins
}

void SPI_Close(void)
{
	PIE1bits.SSPIE = 0;	//interrupt enable
	SSPCONbits.SSPEN = 0;   //SSPEN:
							//1 = Enables serial port and configures SCK, SDO and SDI as serial port pins
							//0 = Disables serial port and configures these pins as I/O port pins

}
//Write a byte to the SPI buffer.  Since the 4431 is being used as a slave, the byte will go out as the next byte is clocked into the SPI Buffer
unsigned char SPI_Write(unsigned char out)
{
	SSPBUF = out;
	if(SSPCONbits.WCOL)//test for write collision
	{
		//WCOL: Write Collision Detect bit
		//1 = The SSPBUF register is written while it is still transmitting the previous word
		//(must be cleared in software)
		//0 = No collision
		SSPCONbits.WCOL = 0;
		return 0;//error
	}
	else
	{
		return 1;//write succesful
	}
}


unsigned char SPI_Read_Byte(void)
{
	return SSPBUF;
}

//This function is called in the SPI byte recieved interrupt.  After checking for a command it will return the data requested.
unsigned char SPI_State_Machine(unsigned char Input)
{
	static unsigned char state = WAITING_FOR_COMMAND_STATE;
	static unsigned char ByteNum = 0;
	unsigned char ReturnValue;
	
	if(state == WAITING_FOR_COMMAND_STATE)
	{
		switch(Input)//check the incoming byte for a command
		{
			case ACCELEROMETER_COMMAND:
				ByteNum = 0;
				state = ACCELEROMETER_COMMAND_STATE;
				break;
			case TWO_AXIS_GYRO_COMMAND:
				ByteNum = 0;
				state = TWO_AXIS_GYRO_COMMAND_STATE;
				break;
			case COMPASS_COMMAND:
				ByteNum = 0;
				state = COMPASS_COMMAND_STATE;
				break;
			case ACCOUSTIC_COMMAND:
				ByteNum = 0;
				state = ACCOUSTIC_COMMAND_STATE;
				break;
			case VOLTAGES_COMMAND:
				ByteNum = 0;
				state = VOLTAGES_COMMAND_STATE;
				break;
			case TEMPERATURE_COMMAND:
				ByteNum = 0;
				state = TEMPERATURE_COMMAND_STATE;
				break;
			case RPM_COMMAND:
				ByteNum = 0;
				state = RPM_COMMAND_STATE;
				break;
			case STATUS_COMMAND:
				ByteNum = 0;
				state = STATUS_COMMAND_STATE;
				break;
			case PWM_COMMAND:
				ByteNum = 0;
				state = PWM_COMMAND_STATE;
				break;
			case GPS_TIME_COMMAND:
				ByteNum = 0;
				state = GPS_TIME_STATE;
				break;
			case GPS_LATITUDE_COMMAND:
				ByteNum = 0;
				state = GPS_LATITUDE_STATE;
				break;
			case GPS_LONGITUDE_COMMAND:
				ByteNum = 0;
				state = GPS_LONGITUDE_STATE;
				break;
			case GPS_SATELLITES_COMMAND:
				ByteNum = 0;
				state = GPS_SATELLITES_STATE;
				break;
			case GPS_ALTITUDE_COMMAND:
				ByteNum = 0;
				state = GPS_ALTITUDE_STATE;
				break;
			default://Invalid command
				state = WAITING_FOR_COMMAND_STATE;
				return 0xFF;
		}
	}
	switch(state)
	{
		case ACCELEROMETER_COMMAND_STATE:
				//send the accelerometer byte pointed to by bytenum
				ReturnValue = Accelerator[ByteNum];
				ByteNum++;
				if(ByteNum > 6)//6 bytes in a accelerometer packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case TWO_AXIS_GYRO_COMMAND_STATE:
				//send the accelerometer byte pointed to by bytenum
				ReturnValue = Gyro[ByteNum];
				ByteNum++;
				if(ByteNum > 4)//6 bytes in a accelerometer packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
			
		case COMPASS_COMMAND_STATE:
				//send the compass byte pointed to by bytenum
				ReturnValue = Compass[ByteNum];
				ByteNum++;
				if(ByteNum > 2)//2 bytes in a compass packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case ACCOUSTIC_COMMAND_STATE:
				//send the accoustic byte pointed to by bytenum
				ReturnValue = Accoustic[ByteNum];
				ByteNum++;
				if(ByteNum > 2)//2 bytes in a accoustic packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case VOLTAGES_COMMAND_STATE:
				//send the voltage byte pointed to by bytenum
				ReturnValue = Voltages[ByteNum];
				ByteNum++;
				if(ByteNum > 4)//4 bytes in a voltages packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case TEMPERATURE_COMMAND_STATE:
				//send the temperature byte pointed to by bytenum
				ReturnValue = Temperature[ByteNum];
				ByteNum++;
				if(ByteNum > 2)//2 bytes in a temperature packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case RPM_COMMAND_STATE:
				//send the RPM byte pointed to by bytenum
				ReturnValue = RPM[ByteNum];
				ByteNum++;
				if(ByteNum > 2)//2 bytes in an RPM packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case STATUS_COMMAND_STATE:
				//send the status byte pointed to by bytenum
				ReturnValue = command[ByteNum];
				command[ByteNum] = Input;
				ByteNum++;
				if(ByteNum > 2)//2 bytes in a status packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case PWM_COMMAND_STATE:
				ReturnValue = servos[ByteNum];
				servos[ByteNum] = Input;
				ByteNum++;
				if(ByteNum > 4)//4 bytes in a status packet
				{
					UpdatePWM();
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case GPS_TIME_STATE:
				ReturnValue = GPS_TIME[ByteNum];
				ByteNum++;
				if(ByteNum > 6)//6 bytes in a status packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case GPS_LATITUDE_STATE:
				ReturnValue = GPS_LATITUDE[ByteNum];
				ByteNum++;
				if(ByteNum > 9)//9 bytes in a status packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case GPS_LONGITUDE_COMMAND:
				ReturnValue = GPS_LONGITUDE[ByteNum];
				ByteNum++;
				if(ByteNum > 10)//10 bytes in a status packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case GPS_SATELLITES_COMMAND:
				ReturnValue = GPS_SATELLITES[ByteNum];
				ByteNum++;
				if(ByteNum > 2)//2 bytes in a status packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
		case GPS_ALTITUDE_COMMAND:
				ReturnValue = GPS_ALTITUDE[ByteNum];
				ByteNum++;
				if(ByteNum > 7)//7 bytes in a status packet
				{
					state = WAITING_FOR_COMMAND_STATE;
					return 0xFF;
				}
				break;
        default:
				state = WAITING_FOR_COMMAND_STATE;
				ReturnValue = 0xA5;				
				return 0xFF;
            break;
	}

	return ReturnValue;
}
