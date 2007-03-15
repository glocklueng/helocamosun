
/*************************************************
*
*	This defines some values for the graphics of helicopters.
*
*	Author: Aaron Kahn
*
**************************************************/

#include "graphics.h"

// initial Pilot View Point
float PVP_x = -2.0;
float PVP_y = 1.0;
float PVP_z = 2.0;

double alpha = atan2(PVP_z, sqrt(PVP_x*PVP_x + PVP_y*PVP_y));
double beta = atan2(PVP_y,PVP_x);
double l = sqrt(PVP_x*PVP_x + PVP_y*PVP_y + PVP_z*PVP_z);

void DrawScene(void)
{
	// this is the variable declerations
	int			e;
	float		X, Y, Z;
	
	

	// all of this stuff is just for the lighting of the 
	// scene.
	GLfloat groundAmbient[4]={ 0.02f, 0.3f, 0.1f, 1.0f };
	
	GLfloat local_ambient[4]={ 0.7f, 0.7f, 0.7f, 1.0f };
	
	GLfloat ambient0[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diffuse0[4]={ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular0[4]={ 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat position0[4]= { 2.0f, 100.5f, 1.5f, 1.0f };
	
	GLfloat ambient1[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diffuse1[4]={ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular1[4]={ 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat position1[4]= { -2.0f, 100.5f, 1.0f, 0.0f };
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, local_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	
	
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	// end of lighting
	

	X = xcell.cg.NED[0];		// North = Xcomputer frame
	Y = -xcell.cg.NED[2];		// Down = -Ycomputer frame
	Z = xcell.cg.NED[1];		// East = Zcomputer frame

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// R/C Pilot's point of view
	gluLookAt(1.0, 6.0, 30.0, X, Y, Z, 0.0, 1.0, 0.0);
	
	glEnable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();

	//this is the ground
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundAmbient);
	
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(31200.0f, -0.0f, -31200.0f);
	glVertex3f(-31200.0f, -0.0f, -31200.0f);
	glVertex3f(-31200.0f, -0.0f, 31200.0f);
	glVertex3f(31200.0f, -0.0f, 31200.0f);
	glEnd();
	
	glDisable(GL_LIGHTING);
	for(e=-1000; e<=1000; e+=50)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex3f(e, 0.8, -1000.0f);
		glVertex3f(e, 0.8, 1000.0f);
		
		glVertex3f(-1000.0f, 0.8, e);
		glVertex3f(1000.0f, 0.8, e);
		glEnd();
	}
	glEnable(GL_LIGHTING);
	
	glPushMatrix();
	glTranslatef(X, Y, Z);
	glRotatef(xcell.cg.THETA[2]*C_RAD2DEG, 0.0f, -1.0f, 0.0);
	glRotatef(xcell.cg.THETA[1]*C_RAD2DEG, 0.0f, 0.0f, 1.0f);
	glRotatef(xcell.cg.THETA[0]*C_RAD2DEG, 1.0f, 0.0f, 0.0f);
	DrawXcellModel();
	glPopMatrix();
	glPushMatrix();
	
	// Draw the shadows
	// make the neccessary squashing matrix
	float mat[16];
	mat[0]=1.0;
	mat[1]=0.0;
	mat[2]=0.0;
	mat[3]=0.0;
	
	mat[4]=0.0;
	mat[5]=0.0;
	mat[6]=0.0;
	mat[7]=0.0;
	
	mat[8]=0.0;
	mat[9]=0.0;
	mat[10]=1.0;
	mat[11]=0.0;
	
	mat[12]=0.0;
	mat[13]=0.0;
	mat[14]=0.0;
	mat[15]=1.0;
	
	glMultMatrixf(mat);
	// Everything now is flattened onto the ground
	glTranslatef(X, 0.01, Z);
	glRotatef(xcell.cg.THETA[2]*C_RAD2DEG, 0.0f, -1.0f, 0.0);
	glRotatef(xcell.cg.THETA[1]*C_RAD2DEG, 0.0f, 0.0f, 1.0f);
	glRotatef(xcell.cg.THETA[0]*C_RAD2DEG, 1.0f, 0.0f, 0.0f);
	DrawXcellShadowModel();
	glPopMatrix();

}


void DrawXcellModel(void)
{
	GLfloat material1Ambient[4]={ 0.4f, 0.0f, 0.0f, 1.0f };
	GLfloat material2Ambient[4]={ 0.4f, 0.4f, 0.0f, 1.0f };
	GLfloat material3Ambient[4]={ 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat material4Ambient[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	
	GLfloat materialSpecular[4]={ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialShininess[1]={ 10.0f };
	GLfloat material4Shininess[1]={ 100.0f };

	float k;
	float lastx1, lasty1, lastz1, newx1, newy1, newz1;
	float lastx2, lasty2, lastz2, newx2, newy2, newz2;

	glScalef(SF, SF, SF);

	// this is beginning of the helicopter model
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material1Ambient);
	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.20f, 0.10f);
	glVertex3f(0.0f, 0.1f, 0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 0.0f, 1.0f);	
	glVertex3f(0.0f, 0.1f, 0.20f);
	glVertex3f(0.0f, -0.1f, 0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, -0.5f, 0.5f);
	glVertex3f(0.0f, -0.1f, 0.20f);
	glVertex3f(0.0f, -0.20f, 0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.20f, 0.1f);
	glVertex3f(0.0f, -0.20f, -0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, -0.5f, -0.5f);
	glVertex3f(0.0f, -0.20f, -0.1f);
	glVertex3f(0.0f, -0.1f, -0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, -0.1f, -0.20f);
	glVertex3f(0.0f, 0.1f, -0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 0.5f, -0.5f);
	glVertex3f(0.0f, 0.1f, -0.20f);
	glVertex3f(0.0f, 0.20f, -0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.20f, -0.1f);
	glVertex3f(0.0f, 0.20f, 0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glEnd();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material4Ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material4Shininess);
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.20f, 0.1f);
	glVertex3f(0.0f, 0.1f, 0.20f);
	glVertex3f(0.0f, -0.1f, 0.20f);
	glVertex3f(0.0f, -0.20f, 0.1f);
	glVertex3f(0.0f, -0.20f, -0.1f);
	glVertex3f(0.0f, -0.1f, -0.20f);
	glVertex3f(0.0f, 0.1f, -0.20f);
	glVertex3f(0.0f, 0.20f, -0.1f);
	glEnd();
	
	//for the tail boom
	//glColor3f(0.4f, 0.4f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material2Ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.02f, 0.02f);
	glVertex3f(-1.4f, 0.02f, 0.02f);
	glVertex3f(-1.4f, -0.02f, 0.02f);
	glVertex3f(0.0f, -0.02f, 0.02f);
	
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.02f, -0.02f);
	glVertex3f(-1.4f, 0.02f, -0.02f);
	glVertex3f(-1.4f, -0.02f, -0.02f);
	glVertex3f(0.0f, -0.02f, -0.02f);
	
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.02f, 0.02f);
	glVertex3f(-1.4f, 0.02f, 0.02f);
	glVertex3f(-1.4f, 0.02f, -0.02f);
	glVertex3f(0.0f, 0.02f, -0.02f);
	
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.02f, 0.02f);
	glVertex3f(-1.4f, -0.02f, 0.02f);
	glVertex3f(-1.4f, -0.02f, -0.02f);
	glVertex3f(0.0f, -0.02f, -0.02f);
	
	glEnd();
	
	// now to draw the tail rotor
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material3Ambient);
	lastx1=0.12-1.4;
	lasty1=0.0;
	lastz1=0.02;
	//glColor3f(0.4f, 0.4f, 0.4f);
	for(k=-C_PI/12; k<=2*C_PI; k+=C_PI/12)
	{
		newx1=-1.4+0.12*cos(k);
		newy1=0.12*sin(k);
		newz1=0.02;
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.4f, 0.0f, 0.02f);
		glVertex3f(lastx1, lasty1, lastz1);
		glVertex3f(newx1, newy1, newz1);
		glEnd();
		lastx1=newx1;
		lasty1=newy1;
		lastz1=newz1;
	}
	
	glPushMatrix();
	// now to draw the main rotor
	// YAW
	glRotatef(xcell.cg.THETA[0]*C_RAD2DEG, 0.0f, -1.0f, 0.0f);
	// PITCH
	glRotatef(xcell.m.a1*C_RAD2DEG, 0.0f, 0.0f, 1.0f);
	// ROLL
	glRotatef(xcell.m.b1*C_RAD2DEG, 1.0f, 0.0f, 0.0f);	

	lastx2=0.0;
	lastx1=0.0;
	lasty1=0.34;
	lasty2=0.34;
	lastz1=0.90;
	lastz2=1.0;
	//glColor4f(0.4f, 0.4f, 0.4f, 0.5f);
	for(k=-C_PI/12; k<=2*C_PI; k=k+C_PI/12)
	{
		newx1=1.0*sin(k);
		newx2=0.90*sin(k);
		newy1=0.34;
		newy2=0.34;
		newz1=1.0*cos(k);
		newz2=0.90*cos(k);
		
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material3Ambient);
		glBegin(GL_POLYGON);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(lastx2, lasty2, lastz2);
		glVertex3f(lastx1, lasty1, lastz1);
		glVertex3f(newx1, newy1, newz1);
		glVertex3f(newx2, newy2, newz2);
		glEnd();
		
		lastx1=newx1;
		lasty1=newy1;
		lastz1=newz1;
		lastx2=newx2;
		lasty2=newy2;
		lastz2=newz2;
	}	
	glPopMatrix();

	//now for the main mast
	glColor3f(0.4f, 0.7f, 0.2f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.34f, 0.0f);
	glEnd();
	
	// now for the skids
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.2f, -0.2f, 0.1f);
	glVertex3f(0.2f, -0.3f, 0.2f);
	
	glVertex3f(0.2f, -0.2f, -0.1f);
	glVertex3f(0.2f, -0.3f, -0.2f);
	
	glVertex3f(0.0f, -0.2f, 0.1f);
	glVertex3f(0.0f, -0.3f, 0.2f);
	
	glVertex3f(0.0f, -0.2f, -0.1f);
	glVertex3f(0.0f, -0.3f, -0.2f);
	
	glVertex3f(-0.1f, -0.3f, 0.2f);
	glVertex3f(0.4f, -0.3f, 0.2f);
	
	glVertex3f(-0.1f, -0.3f, -0.2f);
	glVertex3f(0.4f, -0.3f, -0.2f);
	glEnd();
	glEnable(GL_LIGHTING);
}



void DrawXcellShadowModel(void)
{
	GLfloat material1Ambient[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material2Ambient[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material3Ambient[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material4Ambient[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	
	GLfloat materialSpecular[4]={ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialShininess[1]={ 10.0f };
	GLfloat material4Shininess[1]={ 100.0f };

	float k;
	float lastx1, lasty1, lastz1, newx1, newy1, newz1;
	float lastx2, lasty2, lastz2, newx2, newy2, newz2;
	glScalef(SF, SF, SF);


	// this is beginning of the helicopter model
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material1Ambient);
	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.20f, 0.10f);
	glVertex3f(0.0f, 0.1f, 0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 0.0f, 1.0f);	
	glVertex3f(0.0f, 0.1f, 0.20f);
	glVertex3f(0.0f, -0.1f, 0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, -0.5f, 0.5f);
	glVertex3f(0.0f, -0.1f, 0.20f);
	glVertex3f(0.0f, -0.20f, 0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.20f, 0.1f);
	glVertex3f(0.0f, -0.20f, -0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, -0.5f, -0.5f);
	glVertex3f(0.0f, -0.20f, -0.1f);
	glVertex3f(0.0f, -0.1f, -0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, -0.1f, -0.20f);
	glVertex3f(0.0f, 0.1f, -0.20f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 0.5f, -0.5f);
	glVertex3f(0.0f, 0.1f, -0.20f);
	glVertex3f(0.0f, 0.20f, -0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.20f, -0.1f);
	glVertex3f(0.0f, 0.20f, 0.1f);
	glVertex3f(0.60f, -0.1f, 0.0f);
	
	glEnd();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material4Ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material4Shininess);
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.20f, 0.1f);
	glVertex3f(0.0f, 0.1f, 0.20f);
	glVertex3f(0.0f, -0.1f, 0.20f);
	glVertex3f(0.0f, -0.20f, 0.1f);
	glVertex3f(0.0f, -0.20f, -0.1f);
	glVertex3f(0.0f, -0.1f, -0.20f);
	glVertex3f(0.0f, 0.1f, -0.20f);
	glVertex3f(0.0f, 0.20f, -0.1f);
	glEnd();
	
	//for the tail boom
	//glColor3f(0.4f, 0.4f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material2Ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.02f, 0.02f);
	glVertex3f(-1.4f, 0.02f, 0.02f);
	glVertex3f(-1.4f, -0.02f, 0.02f);
	glVertex3f(0.0f, -0.02f, 0.02f);
	
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.02f, -0.02f);
	glVertex3f(-1.4f, 0.02f, -0.02f);
	glVertex3f(-1.4f, -0.02f, -0.02f);
	glVertex3f(0.0f, -0.02f, -0.02f);
	
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.02f, 0.02f);
	glVertex3f(-1.4f, 0.02f, 0.02f);
	glVertex3f(-1.4f, 0.02f, -0.02f);
	glVertex3f(0.0f, 0.02f, -0.02f);
	
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.02f, 0.02f);
	glVertex3f(-1.4f, -0.02f, 0.02f);
	glVertex3f(-1.4f, -0.02f, -0.02f);
	glVertex3f(0.0f, -0.02f, -0.02f);
	
	glEnd();
	
	// now to draw the tail rotor
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material3Ambient);
	lastx1=0.12-1.4;
	lasty1=0.0;
	lastz1=0.02;
	//glColor3f(0.4f, 0.4f, 0.4f);
	for(k=-C_PI/12; k<=2*C_PI; k+=C_PI/12)
	{
		newx1=-1.4+0.12*cos(k);
		newy1=0.12*sin(k);
		newz1=0.02;
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.4f, 0.0f, 0.02f);
		glVertex3f(lastx1, lasty1, lastz1);
		glVertex3f(newx1, newy1, newz1);
		glEnd();
		lastx1=newx1;
		lasty1=newy1;
		lastz1=newz1;
	}
	// now to draw the main rotor
	lastx2=0.0;
	lastx1=0.0;
	lasty1=0.34;
	lasty2=0.34;
	lastz1=0.90;
	lastz2=1.0;
	//glColor4f(0.4f, 0.4f, 0.4f, 0.5f);
	for(k=-C_PI/12; k<=2*C_PI; k=k+C_PI/12)
	{
		newx1=1.0*sin(k);
		newx2=0.90*sin(k);
		newy1=0.34;
		newy2=0.34;
		newz1=1.0*cos(k);
		newz2=0.90*cos(k);
		
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material3Ambient);
		glBegin(GL_POLYGON);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(lastx2, lasty2, lastz2);
		glVertex3f(lastx1, lasty1, lastz1);
		glVertex3f(newx1, newy1, newz1);
		glVertex3f(newx2, newy2, newz2);
		glEnd();
		
		lastx1=newx1;
		lasty1=newy1;
		lastz1=newz1;
		lastx2=newx2;
		lasty2=newy2;
		lastz2=newz2;
	}	
	
	//now for the main mast
	glColor3f(0.4f, 0.7f, 0.2f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.34f, 0.0f);
	glEnd();
	
	// now for the skids
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.2f, -0.2f, 0.1f);
	glVertex3f(0.2f, -0.3f, 0.2f);
	
	glVertex3f(0.2f, -0.2f, -0.1f);
	glVertex3f(0.2f, -0.3f, -0.2f);
	
	glVertex3f(0.0f, -0.2f, 0.1f);
	glVertex3f(0.0f, -0.3f, 0.2f);
	
	glVertex3f(0.0f, -0.2f, -0.1f);
	glVertex3f(0.0f, -0.3f, -0.2f);
	
	glVertex3f(-0.1f, -0.3f, 0.2f);
	glVertex3f(0.4f, -0.3f, 0.2f);
	
	glVertex3f(-0.1f, -0.3f, -0.2f);
	glVertex3f(0.4f, -0.3f, -0.2f);
	glEnd();
	glEnable(GL_LIGHTING);
}

