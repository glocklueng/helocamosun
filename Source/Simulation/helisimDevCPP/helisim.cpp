/******************************************************************
*
*	This is the main file for the control of the new X-Cell simulator.
*
*	Author: Aaron Kahn
*
*******************************************************************/

#include "helisim.h"
#include <stdio.h>

#define FUZZYCONTROL

//#define PIDCONTROL

#ifdef PIDCONTROL
#include "PIDcontrol.h"
#endif

#ifdef FUZZYCONTROL
#include "FuzzyControl.h"
#endif

double windows_dt = 0.05;

int simWindow;
int hudWindow;


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
    static double rollAngle = 0;
	int n;
	double U[4];
        

#ifdef PIDCONTROL

    rollAngle += 1;
    if(rollAngle>45.0)
    {
     rollAngle = -45.0;   
    }
    
    HC.UpdateSensorValues(xcell.sixdofX);
	U[0] = HC.CollectiveCorrection(10);			                // main rotor collective
	U[1] = HC.RollCorrection(rollAngle*C_DEG2RAD)*C_DEG2RAD;			// A1 (roll)
	U[2] = HC.PitchCorrection(0*C_DEG2RAD)*C_DEG2RAD;		    // B1 (pitch)
	U[3] = HC.YawCorrection(1)*C_DEG2RAD;			            // tail rotor collective
#endif

#ifdef FUZZYCONTROL
    

    	pitch_angle_mf->sensor = xcell.sixdofX.THETA[1];
		pitch_rate_mf->sensor = xcell.sixdofX.rate[0];
		
		roll_angle_mf->sensor = 50;
		roll_rate_mf->sensor = 50;
		
		yaw_angle_mf->sensor = 50;
		yaw_rate_mf->sensor = 41;
		
		collective_height_mf->sensor = 47;
		collective_rate_mf->sensor = 50;
		
		Fuzzification( pitch_param, pitch_angle_mf);
		Fuzzification( tilt_rate_param, pitch_rate_mf);
		
		Fuzzification( roll_param, roll_angle_mf);
		Fuzzification( tilt_rate_param, roll_rate_mf);
		
		Fuzzification( yaw_param, yaw_angle_mf);
		Fuzzification( yaw_rate_param, yaw_rate_mf);
		
		Fuzzification( collective_param, collective_height_mf);
		Fuzzification( collective_rate_param, collective_rate_mf);
		
        U[2] = 0;//doRules(pitch_mf, PitchRule);    
        U[1] = 0;//doRules(roll_mf, RollRule);  
        U[3] = .105; //doRules(yaw_mf, YawRule);	   
        U[0] = .1645;    //doRules(collective_mf, CollectiveRule);	 
      
#endif


	printf("YAW: \tRATE: %f, \tANGLE: %f, \tCORRECTION: %f\n",xcell.sixdofX.rate[2]* C_FT2M, xcell.sixdofX.THETA[2],U[3]);
	printf("PITCH: \tRATE: %f, \tANGLE: %f, \tCORRECTION: %f\n",xcell.sixdofX.rate[1]* C_FT2M,xcell.sixdofX.THETA[1],U[2]);	    
   	printf("ROLL: \tRATE: %f, \tANGLE: %f, \tCORRECTION: %f\n",xcell.sixdofX.rate[0]* C_FT2M,xcell.sixdofX.THETA[0],U[1]);
  	printf("COLL: \tRATE: %f, \tALTITUDE: %f, \tCORRECTION: %f\n",-xcell.sixdofX.Ve[2], -xcell.sixdofX.NED[2], U[0]);
   	printf("ACCEL0: %f, ACCEL1: %f, ACCEL2: %f\n",xcell.sixdofX.accel[0]* C_FT2M,xcell.sixdofX.accel[1]* C_FT2M,xcell.sixdofX.accel[2]* C_FT2M);        	
    for(n=0; n<(int)(windows_dt/model_dt); ++n)
	{
       
        xcell.sixdofIn.hold_u   = 0;	//	hold X-axis body vel. constant (1 hold, 0 free)
        xcell.sixdofIn.hold_v	= 0;	//	hold Y-axis body vel. constant (1 hold, 0 free)
        xcell.sixdofIn.hold_w	= 0;	//	hold Z-axis body vel. constant (1 hold, 0 free)
        xcell.sixdofIn.hold_p	= 0;	//	hold X-axis body rate constant (1 hold, 0 free) side to side roll
        xcell.sixdofIn.hold_q	= 0;	//	hold Y-axis body rate constant (1 hold, 0 free) forward backward roll
        xcell.sixdofIn.hold_r	= 0;	//  hold Z-axis body rate constant (1 hold, 0 free) yaw
			
		ModelGO(U);
	}

}


int
main(int argc, char** argv)
{

	// Initialize the helicopter model
	ModelInit();

	// setting a timer to do the dynamics calcs
	// at 50 Hz.
	SetTimer(NULL, NULL, (int)(1000*windows_dt), (TIMERPROC)Dynamics);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	
	SimWindowInit();

	glutMainLoop();

	return(0);
}
