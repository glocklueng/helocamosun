
/*****************************************************************************
*
*	This is used to draw instruments that would be used in flight operations.
* These are all based on the GLUT library and the MATLIB library.
*
*	Author: Aaron Kahn
*
******************************************************************************/


#include "instruments.h"




/*	This will draw an artificial horzion for attitude and heading
* indication for a pilot.  The inputs are pitch, roll, and yaw of the
* vehicle.  Also, the altitude and speed for the side bars.  In addition
* to only displaying numbers, a three character message can be displayed
* as well.
*  The ranges are data are...
*	Pitch	-90 to 90 degrees
*	Roll	-180 to 180 degrees
*	Yaw		-180 to 180 degrees
*	Altitude	NO RANGE (ft)
*	Speed		NO RANGE (knots)
*	Message	3 characters
*/
void horizon(double pitch, double roll, double yaw, double altitude, double speed, char *message)
{
	int n;
	char buffer[20];
	char headlabels[37][4] = {"S\0", "19\0", "20\0", "21\0", "22\0", "23\0", "24\0",
								"25\0", "26\0", "W\0", "28\0", "29\0", "30\0", "31\0",
								"32\0", "33\0", "34\0", "35\0", "N\0", "1\0", "2\0",
								"3\0", "4\0", "5\0", "6\0", "7\0", "8\0", "E\0", "10\0",
								"11\0", "12\0", "13\0", "14\0", "15\0", "16\0", "17\0",
								"S\0"};
	char *txt;
	double temp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/******** ROLL TICK MARKS AND TICK MARKER **********/
	for(n=-30; n<=30; n+=15)
	{
		glPushMatrix();
		glRotatef(n, 0.0f, 0.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin( GL_LINES );
			glVertex3f(0.0f, 0.24f, -0.9f);
			glVertex3f(0.0f, 0.23f, -0.9f);
		glEnd();
		glPopMatrix();
	}
	glPushMatrix();
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glColor3f(0.85f, 0.5f, 0.1f);
	glBegin( GL_TRIANGLES );
		glVertex3f(0.0f, 0.23f, -0.9f);
		glVertex3f(0.01f, 0.21f, -0.9f);
		glVertex3f(-0.01f, 0.21f, -0.9f);
	glEnd();
	glPopMatrix();


	/******** CENTER MARK ***********************/

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin( GL_LINES );
		// right half
		glVertex3f(0.0f, 0.0f, -0.9f);
		glVertex3f(0.015f, -0.02f, -0.9f);

		glVertex3f(0.015f, -0.02f, -0.9f);
		glVertex3f(0.03f, 0.0f, -0.9f);
		
		glVertex3f(0.03f, 0.0f, -0.9f);
		glVertex3f(0.06f, 0.0f, -0.9f);
		// left half
		glVertex3f(0.0f, 0.0f, -0.9f);
		glVertex3f(-0.015f, -0.02f, -0.9f);

		glVertex3f(-0.015f, -0.02f, -0.9f);
		glVertex3f(-0.03f, 0.0f, -0.9f);
		
		glVertex3f(-0.03f, 0.0f, -0.9f);
		glVertex3f(-0.06f, 0.0f, -0.9f);
	glEnd();
	
	glPopMatrix();

	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(-yaw*C_DEG2RAD, 0.0f, 0.0f);
	/******** HORIZON AND YAW TICK LINE **********/
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin( GL_LINES );
		glVertex3f(-(180.0+15)*C_DEG2RAD, 0.0f, -0.9f);
		glVertex3f((180.0+15)*C_DEG2RAD, 0.0f, -0.9f);
	glEnd();

	for(n=0; n<37; ++n)
	{
		glBegin( GL_LINES );
			glVertex3f( (double)(n*10 - 180)*C_DEG2RAD, 0.015f, -0.9f);
			glVertex3f( (double)(n*10 - 180)*C_DEG2RAD, 0.0f, -0.9f);
		glEnd();
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.9f);
		txt = (char *)&headlabels[n][0];
		showMessage((double)(n*10 - 180)*C_DEG2RAD-0.01f, 0.02, txt, 1.2);
		glPopMatrix();
	}
	// Extra tick mark past S (going W) for overview
	glBegin( GL_LINES );
		glVertex3f( 190.0*C_DEG2RAD, 0.02f, -0.9f);
		glVertex3f( 190.0*C_DEG2RAD, 0.0f, -0.9f);
	glEnd();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.9f);
	showMessage( 190.0*C_DEG2RAD-0.015, 0.02, "19\0", 1.0);
	glPopMatrix();
	// Extra tick mark past S (going E) for overview
	glBegin( GL_LINES );
		glVertex3f( -190.0*C_DEG2RAD, 0.02f, -0.9f);
		glVertex3f( -190.0*C_DEG2RAD, 0.0f, -0.9f);
	glEnd();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.9f);
	showMessage( -190.0*C_DEG2RAD-0.015, 0.02, "17\0", 1.0);
	glPopMatrix();


	glPushMatrix();
	glLoadIdentity();
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -pitch*C_DEG2RAD, 0.0f);

	/******** COLORED PART OF DISPLAY ************/
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin( GL_QUADS );
		glVertex3f(-(180.0+15)*C_DEG2RAD, (90.0+15.0)*C_DEG2RAD, -1.0f);
		glVertex3f((180.0+15)*C_DEG2RAD, (90.0+15.0)*C_DEG2RAD, -1.0f);
		glVertex3f((180.0+15)*C_DEG2RAD, 0.0f, -1.0f);
		glVertex3f(-(180.0+15)*C_DEG2RAD, 0.0f, -1.0f);
	glEnd();
	
	// bottom of display
	glColor3f(0.5f, 0.2f, 0.1f);
	glBegin( GL_QUADS );
		glVertex3f(-(180.0+15)*C_DEG2RAD, -(90.0+15.0)*C_DEG2RAD, -1.0f);
		glVertex3f((180.0+15)*C_DEG2RAD, -(90.0+15.0)*C_DEG2RAD, -1.0f);
		glVertex3f((180.0+15)*C_DEG2RAD, 0.0f, -1.0f);
		glVertex3f(-(180.0+15)*C_DEG2RAD, 0.0f, -1.0f);
	glEnd();

	/********* PITCH BARS *****************/
	for(n=0; n<9; ++n)
	{
		temp = (double)(n*10+10)*C_DEG2RAD;
		glColor3f(1.0f, 1.0f, 1.0f);
		// positive pitch lines
		glBegin( GL_LINES );
			glVertex3f(-0.1f, temp-0.01, -1.0f);
			glVertex3f(-0.1f, temp, -1.0f);

			glVertex3f(-0.1f, temp, -1.0f);
			glVertex3f(-0.03f, temp, -1.0f);

			glVertex3f(0.1f, temp-0.01, -1.0f);
			glVertex3f(0.1f, temp, -1.0f);

			glVertex3f(0.1f, temp, -1.0f);
			glVertex3f(0.03f, temp, -1.0f);
			
		glEnd();
		sprintf(buffer, "%d\0", n*10+10);
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -1.0f);
		showMessage(0.11f, temp-0.007, buffer, 1.0);
		showMessage(-0.13f, temp-0.007, buffer, 1.0);
		glPopMatrix();

		// negative pitch lines
		glBegin( GL_LINES );
			glVertex3f(-0.1f, -temp+0.01, -1.0f);
			glVertex3f(-0.1f, -temp, -1.0f);

			glVertex3f(-0.1f, -temp, -1.0f);
			glVertex3f(-0.03f, -temp, -1.0f);

			glVertex3f(0.1f, -temp+0.01, -1.0f);
			glVertex3f(0.1f, -temp, -1.0f);

			glVertex3f(0.1f, -temp, -1.0f);
			glVertex3f(0.03f, -temp, -1.0f);
		glEnd();
		sprintf(buffer, "%d\0", -(n*10+10));
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -1.0f);
		showMessage(0.11f, -temp, buffer, 1.0);
		showMessage(-0.14f, -temp, buffer, 1.0);
		glPopMatrix();
	}
	// +/- 5 degree tick marks
	glBegin( GL_LINES );
		glVertex3f(-0.05f, 5.0*C_DEG2RAD, -1.0f);
		glVertex3f(0.05f, 5.0*C_DEG2RAD, -1.0f);
	glEnd();
	glBegin( GL_LINES );
		glVertex3f(-0.05f, -5.0*C_DEG2RAD, -1.0f);
		glVertex3f(0.05f, -5.0*C_DEG2RAD, -1.0f);
	glEnd();
	
	glPopMatrix();

	/******** BOUNDARY LINES ON EDGES AND ALITUTDE/SPEED READOUT ******************/
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin( GL_QUADS );
		glVertex3f(0.18f, 0.16f, -0.8f);
		glVertex3f(0.1f, 0.16f, -0.8f);
		glVertex3f(0.1f, 0.13f, -0.8f);
		glVertex3f(0.18f, 0.13f, -0.8f);
		
		glVertex3f(-0.19f, 0.16f, -0.8f);
		glVertex3f(-0.1f, 0.16f, -0.8f);
		glVertex3f(-0.1f, 0.13f, -0.8f);
		glVertex3f(-0.19f, 0.13f, -0.8f);

		glVertex3f(0.18f, -0.18f, -0.8f);
		glVertex3f(0.14, -0.18f, -0.8f);
		glVertex3f(0.14f, -0.15f, -0.8f);
		glVertex3f(0.18f, -0.15f, -0.8f);
	glEnd();

	// altitude readout
	glColor3f(0.0f, 1.0f, 0.0f);
	sprintf(buffer, "%5d ft\0", (int)(altitude));
	glPushMatrix();
	glTranslatef(-0.19f, 0.140f, -0.8f);
	showMessage(0.0f, 0.0f, buffer, 1.0);
	glPopMatrix();
	
	// speed readout
	sprintf(buffer, "%4d kts\0", (int)(speed));
	glPushMatrix();
	glTranslatef(0.09f, 0.140f, -0.8f);
	showMessage(0.0f, 0.0f, buffer, 1.0);
	glPopMatrix();

	// message readout
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.8f);
	showMessage(0.145f, -0.17f, message, 1.0);
	glPopMatrix();
	

	glPopMatrix();


}


void drawText( char *message ) {

  /* draw each character in a stroke font */

  while( *message ) {
    glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, *message );
    message++;
  }

}


void showMessage( GLfloat x, GLfloat y, char *message, float scale ) {

  /* generic string drawing routine, 2-D */

  glPushMatrix();
  glTranslatef( x, y, 0.0 );
  glScalef( 0.0001, 0.0001, 0.0001 );
  glScalef( scale, scale, scale );
  drawText( message );
  glPopMatrix();

}
