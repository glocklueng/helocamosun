/******************************************************************
*
*	This is the main file for the control of the new X-Cell simulator.
*
*	Author: Aaron Kahn
*
*******************************************************************/

#include "helisim.h"
#include <stdio.h>
#include <fstream>

#define FUZZYCONTROL

//#define PIDCONTROL

#ifdef PIDCONTROL
#include "PIDcontrol.h"
#endif

#ifdef FUZZYCONTROL
#include "../fuzzy test/fuzz.h"
#endif

using namespace std;

double windows_dt = 0.05;

int simWindow;
int hudWindow;

ofstream myfile;

#ifdef PIDCONTROL
HelicopterController HC; //create a PID control object
#endif

#ifdef FUZZYCONTROL
   double negative, zero, positive;
    
        fMember pitch_mf[3];
    fMember *pitch_angle_mf = &pitch_mf[0], 
		*pitch_rate_mf  = &pitch_mf[1], 
		*pitch_accel_mf = &pitch_mf[2];
		
    fMember roll_mf[3];
    fMember *roll_angle_mf = &roll_mf[0], 
		*roll_rate_mf  = &roll_mf[1], 
		*roll_accel_mf = &roll_mf[2];
		
    fMember yaw_mf[3];
    fMember *yaw_angle_mf = &yaw_mf[0], 
		*yaw_rate_mf  = &yaw_mf[1], 
		*yaw_accel_mf = &yaw_mf[2];

    fMember collective_mf[3];
    fMember *collective_height_mf = &collective_mf[0], 
		*collective_rate_mf  = &collective_mf[1], 
		*collective_accel_mf = &collective_mf[2];
#endif
/********* SCOPE FUNCTIONS *************/
static void ResizeSimWindow(GLsizei w, GLsizei h);
static void ResizeHudWindow(GLsizei w, GLsizei h);
static void Animate(void);
static void FlightInstruments(void);
void SimWindowInit(void);
void openHUD(void);
void display(void);
void Dynamics(void);

static void Animate(void)
{
	DrawScene();
	display();

}

static void FlightInstruments(void)
{
	double speed;
	char txt[4];

	sprintf(txt, "MAN\0");
	speed = sqrt( SQR(xcell.cg.V[0]) + SQR(xcell.cg.V[1]) + SQR(xcell.cg.V[2]) )*C_FPS2KT;
	
	horizon(xcell.cg.THETA[1]*C_RAD2DEG, xcell.cg.THETA[0]*C_RAD2DEG, xcell.cg.THETA[2]*C_RAD2DEG, \
			-xcell.cg.NED[2], speed, txt);

	display();
}


static void ResizeSimWindow(GLsizei w, GLsizei h)
{
    h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 30.0, (GLfloat)w/(GLfloat)h, 0.1f, 2500.0f );
	// select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

static void ResizeHudWindow(GLsizei w, GLsizei h)
{
    h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 30.0, (GLfloat)w/(GLfloat)h, 0.1f, 2500.0f );
	// select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void openHUD(void)
{
	hudWindow = glutCreateWindow("HUD");
	glutSetWindow(hudWindow);
	glutPositionWindow(500, 30);
	glutReshapeWindow(240, 240);
	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(FlightInstruments); 
	glutReshapeFunc(ResizeHudWindow);

}

void SimWindowInit(void)
{
	simWindow = glutCreateWindow("X-Cell Sim");
	glutSetWindow(simWindow);
	glutInitWindowSize (320, 320); 
	glutInitWindowPosition (10, 10);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.49, 0.62, 0.9, 0.0);

	glutDisplayFunc(Animate); 
	glutReshapeFunc(ResizeSimWindow);

	openHUD();

}

void display(void)
{
	glutPostRedisplay();
	glutSwapBuffers();
}

//	This is the function to propogate the model and the other stuff
void Dynamics(void)
{

	int n;
	double U[4]; 

#ifdef PIDCONTROL
    static int iterationcounter = 0;
    static double rollAngle = 0;
    
    iterationcounter++;
    rollAngle += 1;
    if(rollAngle>15.0)
    {
        rollAngle = -15.0;   
    }

    if(iterationcounter > 75 && iterationcounter<100)
    {
    HC.UpdateSensorValues(xcell.sixdofX);
	U[0] = HC.CollectiveCorrection(2)*C_DEG2RAD;			                // main rotor collective
	U[1] = HC.RollCorrection(rollAngle)*C_DEG2RAD;			// A1 (roll)
	U[2] = HC.PitchCorrection(0)*C_DEG2RAD;		    // B1 (pitch)
	U[3] = HC.YawCorrection(2)*C_DEG2RAD;			            // tail rotor collective
    }
    else
    {
    HC.UpdateSensorValues(xcell.sixdofX);
	U[0] = HC.CollectiveCorrection(20)*C_DEG2RAD;			                // main rotor collective
	U[1] = HC.RollCorrection(1.9)*C_DEG2RAD;			// A1 (roll)
	U[2] = HC.PitchCorrection(0)*C_DEG2RAD;		    // B1 (pitch)
	U[3] = HC.YawCorrection(2)*C_DEG2RAD;			            // tail rotor collective
    }
 

#endif

#ifdef FUZZYCONTROL
    static int iteration_counter = 0;

    if(iteration_counter == 100)
    {
        int i, j;
        for(i = 0; i<5; i++)
        {
            for(j = 0; j<3; j++)
            {
                //yaw_offset = 30;
               yaw_param[i][j] += 20;    
            }
        }
    }
    if(iteration_counter == 200)
    {
        int i, j;
        for(i = 0; i<5; i++)
        {
            for(j = 0; j<3; j++)
            {
                //yaw_offset = 10;
               yaw_param[i][j] += 5;    
            }
        }
    }
    iteration_counter++;
    pitch_angle_mf->sensor = xcell.sixdofX.THETA[1]*C_RAD2DEG_F;        // THETA[1] = pitch
	pitch_rate_mf->sensor = 500;//+(xcell.sixdofX.rate[1]*C_FT2M);          // THETA[0] = roll

	roll_angle_mf->sensor = xcell.sixdofX.THETA[0]*C_RAD2DEG_F;
	roll_rate_mf->sensor = 500;//+(xcell.sixdofX.rate[0]*C_FT2M);

	yaw_angle_mf->sensor = (xcell.sixdofX.THETA[2]*C_RAD2DEG_F);
//	printf("\nYawoffset: %f\n",yaw_offset);
	
	yaw_rate_mf->sensor = xcell.sixdofX.rate[2]*C_RAD2DEG_F;

//		collective_height_mf->sensor = xcell.sixdofX.accel[2];
//		collective_rate_mf->500;//sensor = -xcell.sixdofX.NED[2];
//	
    	Fuzzification( pitch_param, roll_angle_mf);
		Fuzzification( tilt_rate_param, roll_rate_mf);
        
        U[1] = 0;//-doRules(roll_mf, Rule)*C_DEG2RAD_F;  
  	
		Fuzzification( pitch_param, pitch_angle_mf);
		Fuzzification( tilt_rate_param, pitch_rate_mf);
		
        U[2] = 0;//doRules(pitch_mf, Rule)*C_DEG2RAD_F; 
        	   
		Fuzzification( yaw_param, yaw_angle_mf);
		Fuzzification( yaw_rate_param, yaw_rate_mf);
		
    	U[3] = -doRules(yaw_mf, Rule)*C_DEG2RAD_F;

//		Fuzzification( pitch_param, collective_height_mf);
//		Fuzzification( tilt_rate_param, collective_rate_mf);
//	
//        U[0] = doRules(collective_mf, Rule)-50;
U[0] = 0.16;
        
#endif


	printf("YAW: \tRATE: %f, \tANGLE: %f, \tCORRECTION: %f\n",xcell.sixdofX.rate[2]*C_RAD2DEG, xcell.sixdofX.THETA[2]*C_RAD2DEG,U[3]*C_RAD2DEG);
    printf("PITCH: \tRATE: %f, \tANGLE: %f, \tCORRECTION: %f\n",xcell.sixdofX.rate[1]*C_RAD2DEG,xcell.sixdofX.THETA[1]*C_RAD2DEG,U[2]*C_RAD2DEG);	    
   	printf("ROLL: \tRATE: %f, \tANGLE: %f, \tCORRECTION: %f\n",xcell.sixdofX.rate[0]* C_RAD2DEG,xcell.sixdofX.THETA[0]*C_RAD2DEG_F,U[1]*C_RAD2DEG);
  	printf("COLL: \tRATE: %f, \tALTITUDE: %f, \tCORRECTION: %f\n",-xcell.sixdofX.Ve[2]* C_FT2M, -xcell.sixdofX.NED[2]* C_FT2M, U[0]*C_RAD2DEG);
   	printf("ACCEL0: %f, ACCEL1: %f, ACCEL2: %f\n",xcell.sixdofX.accel[0]* C_FT2M,xcell.sixdofX.accel[1]* C_FT2M,xcell.sixdofX.accel[2]* C_FT2M);        	
    
    //print out the parameters to the excel file
    myfile << xcell.sixdofX.THETA[2]*C_RAD2DEG << "\t";//yaw angle
    myfile << xcell.sixdofX.rate[2]*C_RAD2DEG << "\t";//yaw rate
    
    myfile << xcell.sixdofX.THETA[1]*C_RAD2DEG << "\t";//pitch angle
    myfile << xcell.sixdofX.rate[1]*C_RAD2DEG << "\t";//pitch rate

    myfile << xcell.sixdofX.THETA[0]*C_RAD2DEG << "\t";//roll angle    
    myfile << xcell.sixdofX.rate[0]*C_RAD2DEG << "\t";//roll rate
    
    myfile << -xcell.sixdofX.NED[2]* C_FT2M << "\t";//altitude
    myfile << xcell.sixdofX.Ve[2]*C_FT2M << "\t";//climb rate
    
    myfile << U[3] * C_RAD2DEG << "\t";//yaw correction
    myfile << U[2] * C_RAD2DEG << "\t";//pitch correction
    myfile << U[1] * C_RAD2DEG << "\t";//roll correction
    myfile << U[0] * C_RAD2DEG << "\n";//collective correction
        
                
    for(n=0; n<(int)(windows_dt/model_dt); ++n)
	{
       
        xcell.sixdofIn.hold_u   = 1;	//	hold X-axis body vel. constant (1 hold, 0 free)
        xcell.sixdofIn.hold_v	= 1;	//	hold Y-axis body vel. constant (1 hold, 0 free)
        xcell.sixdofIn.hold_w	= 0;	//	hold Z-axis body vel. constant (1 hold, 0 free)
        xcell.sixdofIn.hold_p	= 1;	//	hold X-axis body rate constant (1 hold, 0 free) side to side roll
        xcell.sixdofIn.hold_q	= 1;	//	hold Y-axis body rate constant (1 hold, 0 free) forward backward pitch
        xcell.sixdofIn.hold_r	= 1;	//  hold Z-axis body rate constant (1 hold, 0 free) yaw
			
		ModelGO(U);
	}

}



int
main(int argc, char** argv)
{


    myfile.open ("data.xls",ios::trunc);

    //print the titles on the excel file    
    myfile << "yaw angle (deg)" << "\t";
    myfile << "yaw rate (deg/s)" << "\t";
    
    myfile << "pitch angle (deg)" << "\t";
    myfile << "pitch rate (deg/s)" << "\t";
    
    myfile << "roll angle (deg)" << "\t";    
    myfile << "roll rate (deg/s)" << "\t";
    
    myfile << "altitude (m/s)" << "\t";
    myfile << "climb rate (m/s^2)" << "\t";
    
    myfile << "yaw correction (deg)" << "\t";
    myfile << "pitch correction (deg)" << "\t";
    myfile << "roll correction (deg)" << "\t";
    myfile << "collective correction (deg)" << "\n";
       
	// Initialize the helicopter model
	ModelInit();

	// setting a timer to do the dynamics calcs
	// at 50 Hz.
	SetTimer(NULL, NULL, (int)(1000*windows_dt), (TIMERPROC)Dynamics);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	
	SimWindowInit();

	glutMainLoop();

    myfile.close();
    
	return(0);
}
