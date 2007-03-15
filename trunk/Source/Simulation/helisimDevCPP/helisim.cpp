/******************************************************************
*
*	This is the main file for the control of the new X-Cell simulator.
*
*	Author: Aaron Kahn
*
*******************************************************************/

#include "helisim.h"


double windows_dt = 0.05;

int simWindow;
int hudWindow;


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

	for(n=0; n<(int)(windows_dt/model_dt); ++n)
	{
		U[0] = 10.5*C_DEG2RAD;			// main rotor collective
		U[1] = 0.0*C_DEG2RAD;			// A1 (roll)
		U[2] = 0.0*C_DEG2RAD;			// B1 (pitch)
		U[3] = 6.0*C_DEG2RAD;			// tail rotor collective

// t
//_________________________________________________________
			
		ModelGO(U);

// t+dt
	}
		
	/****** FLIGHT CONTROL CODE GOES HERE ******/

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
